/**
 * @file test_can_bridge.cpp
 * @brief Unit test verifying the C++ DBC decoding & ROS 2 dispatch pipeline.
 */

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float32.hpp>
#include <iostream>
#include <cassert>
#include <thread>
#include <chrono>
#include <cmath>

#include "generated/can_bridge_impl.hpp"

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    
    auto node = std::make_shared<rclcpp::Node>("test_can_bridge_node");
    
    // Instantiate target DBC dispatcher class
    CanBridgeImpl bridge(node.get());
    
    float received_speed = -1.0f;
    bool received = false;
    
    // Subscribe to test target topic
    auto sub = node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/aqt2/spd_wheel",
        rclcpp::QoS(10).best_effort(),
        [&](const std_msgs::msg::Float32::SharedPtr msg) {
            received_speed = msg->data;
            received = true;
        }
    );
    
    // Pack sample DBC data using the cantools unpack/pack C bindings
    struct data_t26_aqt2_t sample = {};
    sample.spd_wheel = data_t26_aqt2_spd_wheel_encode(42.5f);
    sample.tire_temp = data_t26_aqt2_tire_temp_encode(60.0f);
    sample.brake_temp = data_t26_aqt2_brake_temp_encode(120.0f);
    
    uint8_t buffer[8] = {0};
    int pack_res = data_t26_aqt2_pack(buffer, &sample, sizeof(buffer));
    assert(pack_res == DATA_T26_AQT2_LENGTH);
    
    // Execute frame decoding pipeline
    bool handled = bridge.handle_frame(DATA_T26_AQT2_FRAME_ID, buffer, sizeof(buffer));
    assert(handled == true);
    
    // Process message callbacks
    for (int i = 0; i < 15; ++i) {
        rclcpp::spin_some(node);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    
    std::cout << "Received speed: " << received_speed << " (Expected: ~42.5)" << std::endl;
    assert(received);
    assert(std::abs(received_speed - 42.5f) < 0.1f);
    
    std::cout << "✓ All CAN bridge DBC/ROS 2 decoding tests passed successfully!" << std::endl;
    rclcpp::shutdown();
    return 0;
}
