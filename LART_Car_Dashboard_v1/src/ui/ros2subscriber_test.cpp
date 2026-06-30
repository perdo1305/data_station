#include "ros2subscriber.h"
#include <rclcpp/rclcpp.hpp>
#include <lart_msgs/msg/can_frame.hpp>
#include <std_msgs/msg/float32.hpp>
#include <iostream>
#include <cassert>
#include <thread>
#include <chrono>
#include <cstdlib>
#include "eez-flow.h"
#include "dbc_api.h"

// Stub implementations of eez functions to satisfy the linker for the test executable
namespace eez {
    void free(void *ptr) {
        ::free(ptr);
    }

    Value Value::makeStringRef(const char *str, int len, uint32_t id) {
        (void)len;
        (void)id;
        Value val;
        val.type = VALUE_TYPE_STRING;
        val.options = 0;
        val.strValue = str;
        return val;
    }

    const char *Value::getString() const {
        return type == VALUE_TYPE_STRING ? strValue : nullptr;
    }

    namespace flow {
        void setGlobalVariable(uint32_t globalVariableIndex, const Value &value) {
            (void)globalVariableIndex;
            (void)value;
        }

        Value getGlobalVariable(uint32_t globalVariableIndex) {
            (void)globalVariableIndex;
            return Value();
        }
    }
}

// Stub UI notification callbacks (normally provided by ui_runner, not linked in this test)
extern "C" {
    void ui_add_notification(const char *id, const char *title, const char *message) {
        (void)id; (void)title; (void)message;
    }
    void ui_clear_notification(const char *id) {
        (void)id;
    }
    void ui_clear_all_notifications(void) {}
}

int main(int argc, char **argv) {
    // Sourcing rclcpp dependencies and initializing execution context
    rclcpp::init(argc, argv);
    
    // ros2subscriber_init sets up subscriptions, including the CAN log buffer
    int init_res = ros2subscriber_init();
    assert(init_res == 0);
    
    // We create a separate node to publish CanFrame messages on the "/can/frames" topic.
    // This allows us to simulate the incoming CAN stream locally.
    auto pub_node = std::make_shared<rclcpp::Node>("test_publisher");
    auto pub = pub_node->create_publisher<lart_msgs::msg::CanFrame>("/can/frames", rclcpp::QoS(10).best_effort());
    auto asms_pub = pub_node->create_publisher<std_msgs::msg::Float32>("/can/dbc/acu/asms", rclcpp::QoS(10).best_effort());
    
    auto msg = std::make_shared<lart_msgs::msg::CanFrame>();
    msg->can_id = 0x123;
    msg->dlc = 4;
    msg->data[0] = 0xDE;
    msg->data[1] = 0xAD;
    msg->data[2] = 0xBE;
    msg->data[3] = 0xEF;

    auto asms_msg = std::make_shared<std_msgs::msg::Float32>();
    asms_msg->data = 1.0f;
    
    // We spin and publish multiple times. This is needed because ROS2 discovery
    // of subscription matches can take some time.
    for (int i = 0; i < 50; ++i) {
        pub->publish(*msg);
        asms_pub->publish(*asms_msg);
        ros2subscriber_spin_some();
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
    
    char log_buf[1024];
    int get_res = ros2subscriber_get_can_log(log_buf, sizeof(log_buf));
    assert(get_res == 1);
    
    std::cout << "CAN LOG VALUE:\n" << log_buf << std::endl;
    
    // Verification: ensure the logged string matches the expected ID and byte output
    std::string log_str(log_buf);
    if (log_str.find("0x123") == std::string::npos || log_str.find("DE AD BE EF") == std::string::npos) {
        std::cerr << "Test failed! Log did not contain expected values." << std::endl;
        ros2subscriber_fini();
        rclcpp::shutdown();
        return 1;
    }

    // Verify ASMS telemetry propagation
    std::cout << "Verifying ASMS telemetry: dbc_api.acu.asms = " << dbc_api.acu.asms << std::endl;
    if (dbc_api.acu.asms != 1.0f) {
        std::cerr << "Test failed! ASMS value did not propagate." << std::endl;
        ros2subscriber_fini();
        rclcpp::shutdown();
        return 1;
    }
    
    std::cout << "Test passed!" << std::endl;
    ros2subscriber_fini();
    rclcpp::shutdown();
    return 0;
}
