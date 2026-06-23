/**
 * @file can_bridge.cpp
 * @brief C++ SocketCAN to ROS 2 Telemetry Bridge.
 * 
 * Subscribes to a Linux raw CAN socket, translates frames using DBC unpacking logic,
 * and publishes physical signals, raw frames, and legacy RPM values to ROS 2.
 */

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float32.hpp>
#include <lart_msgs/msg/can_frame.hpp>

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <linux/can.h>
#include <linux/can/raw.h>

#include <thread>
#include <atomic>
#include <string>
#include <chrono>
#include <algorithm>

#include "generated/can_bridge_impl.hpp"

class CanBridgeNode : public rclcpp::Node {
public:
    /**
     * @brief Construct the CAN bridge node, initializing publishers and parameters.
     */
    CanBridgeNode() : Node("can_bridge"), sock_(-1), running_(false) {
        // Declare interface and legacy RPM parameters
        this->declare_parameter<std::string>("can_interface", "can0");
        this->declare_parameter<int>("rpm_can_id", 291); // 0x123
        this->declare_parameter<int>("rpm_start_byte", 0);
        this->declare_parameter<int>("rpm_length_bytes", 2);
        this->declare_parameter<double>("rpm_scale", 1.0);
        this->declare_parameter<double>("rpm_offset", 0.0);

        // Retrieve parameters
        this->get_parameter("can_interface", iface_name_);
        this->get_parameter("rpm_can_id", rpm_can_id_);
        this->get_parameter("rpm_start_byte", rpm_start_byte_);
        this->get_parameter("rpm_length_bytes", rpm_length_bytes_);
        this->get_parameter("rpm_scale", rpm_scale_);
        this->get_parameter("rpm_offset", rpm_offset_);

        auto sensor_qos = rclcpp::QoS(10).best_effort();

        // Core ROS 2 publishers
        frame_pub_ = this->create_publisher<lart_msgs::msg::CanFrame>("/can/frames", sensor_qos);
        rpm_pub_ = this->create_publisher<std_msgs::msg::Float32>("/vehicle/rpm", sensor_qos);

        // Initialize DBC parser dispatcher
        dbc_impl_ = std::make_unique<CanBridgeImpl>(this);

        // Start SocketCAN listener thread
        running_ = true;
        reader_thread_ = std::thread(&CanBridgeNode::socketcan_reader_loop, this);

        RCLCPP_INFO(this->get_logger(), "CAN bridge initialized on %s (RPM ID=0x%03X)", 
                    iface_name_.c_str(), rpm_can_id_);
    }

    /**
     * @brief Destructor ensures the reading loop thread exits and clean socket cleanup.
     */
    ~CanBridgeNode() {
        running_ = false;
        if (reader_thread_.joinable()) {
            reader_thread_.join();
        }
        close_socket();
    }

private:
    /**
     * @brief Closes the SocketCAN file descriptor if valid.
     */
    void close_socket() {
        if (sock_ >= 0) {
            ::close(sock_);
            sock_ = -1;
        }
    }

    /**
     * @brief Initializes raw SocketCAN interface connection.
     * @return true if socket successfully opened and bound, false otherwise.
     */
    bool init_socket() {
        close_socket();

        sock_ = ::socket(PF_CAN, SOCK_RAW, CAN_RAW);
        if (sock_ < 0) {
            RCLCPP_ERROR(this->get_logger(), "Failed to create SocketCAN socket: %s", strerror(errno));
            return false;
        }

        struct ifreq ifr;
        std::strncpy(ifr.ifr_name, iface_name_.c_str(), IFNAMSIZ - 1);
        ifr.ifr_name[IFNAMSIZ - 1] = '\0';

        if (::ioctl(sock_, SIOCGIFINDEX, &ifr) < 0) {
            RCLCPP_ERROR(this->get_logger(), "Failed to resolve CAN interface index for %s: %s", 
                         iface_name_.c_str(), strerror(errno));
            close_socket();
            return false;
        }

        struct sockaddr_can addr;
        addr.can_family = AF_CAN;
        addr.can_ifindex = ifr.ifr_ifindex;

        if (::bind(sock_, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
            RCLCPP_ERROR(this->get_logger(), "Failed to bind SocketCAN socket to %s: %s", 
                         iface_name_.c_str(), strerror(errno));
            close_socket();
            return false;
        }

        // Set a read timeout so the loop doesn't block indefinitely on node shutdown
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 100000; // 100ms receive timeout
        if (::setsockopt(sock_, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
            RCLCPP_WARN(this->get_logger(), "Failed to set socket read timeout: %s", strerror(errno));
        }

        return true;
    }

    /**
     * @brief Background thread worker to continuously poll SocketCAN and dispatch frames.
     */
    void socketcan_reader_loop() {
        while (running_ && rclcpp::ok()) {
            if (sock_ < 0) {
                if (!init_socket()) {
                    // Backoff before retrying socket setup
                    std::this_thread::sleep_for(std::chrono::seconds(2));
                    continue;
                }
            }

            struct can_frame frame;
            int nbytes = ::read(sock_, &frame, sizeof(struct can_frame));

            if (nbytes < 0) {
                if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) {
                    continue;
                }
                RCLCPP_ERROR(this->get_logger(), "SocketCAN read error: %s", strerror(errno));
                close_socket();
                continue;
            }

            if (nbytes < static_cast<int>(sizeof(struct can_frame))) {
                RCLCPP_WARN(this->get_logger(), "Incomplete CAN frame read (%d bytes)", nbytes);
                continue;
            }

            // Extract standard or extended ID without flag bits
            uint32_t clean_id = frame.can_id & CAN_EFF_MASK;

            // 1. Publish raw frame
            lart_msgs::msg::CanFrame raw_msg;
            raw_msg.stamp = this->get_clock()->now();
            raw_msg.can_id = clean_id;
            raw_msg.dlc = frame.can_dlc;
            std::copy(frame.data, frame.data + 8, raw_msg.data.begin());
            frame_pub_->publish(raw_msg);

            // 2. Decode legacy RPM
            if (clean_id == static_cast<uint32_t>(rpm_can_id_)) {
                size_t end = rpm_start_byte_ + rpm_length_bytes_;
                if (end <= frame.can_dlc) {
                    uint32_t raw_val = 0;
                    for (int i = 0; i < rpm_length_bytes_; ++i) {
                        raw_val = (raw_val << 8) | frame.data[rpm_start_byte_ + i];
                    }
                    std_msgs::msg::Float32 rpm_msg;
                    rpm_msg.data = static_cast<float>(raw_val) * rpm_scale_ + rpm_offset_;
                    rpm_pub_->publish(rpm_msg);
                }
            }

            // 3. Decode signals using compiled DBC definitions
            dbc_impl_->handle_frame(clean_id, frame.data, frame.can_dlc);
        }
    }

    // Parameters
    std::string iface_name_;
    int rpm_can_id_;
    int rpm_start_byte_;
    int rpm_length_bytes_;
    double rpm_scale_;
    double rpm_offset_;

    // Socket state
    int sock_;
    std::atomic<bool> running_;
    std::thread reader_thread_;

    // ROS 2 publishers
    rclcpp::Publisher<lart_msgs::msg::CanFrame>::SharedPtr frame_pub_;
    rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr rpm_pub_;

    // DBC parser implementation
    std::unique_ptr<CanBridgeImpl> dbc_impl_;
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<CanBridgeNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
