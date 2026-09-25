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
#include <lart_msgs/msg/aqt2.hpp>

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    
    auto node = std::make_shared<rclcpp::Node>("test_can_bridge_node");
    
    // Instantiate target DBC dispatcher class
    CanBridgeImpl bridge(node.get(), "data_t26");
    
    float received_temperature = -1.0f;
    bool received = false;
    
    // Subscribe to test target topic
    auto sub = node->create_subscription<lart_msgs::msg::Aqt2>(
        "/data/dbc/aqt2",
        rclcpp::QoS(10).best_effort(),
        [&](const lart_msgs::msg::Aqt2::SharedPtr msg) {
            received_temperature = msg->tire_temp;
            received = true;
        }
    );
    
    // Pack sample DBC data using the cantools unpack/pack C bindings
    struct data_t26_aqt2_t sample = {};
    sample.tire_temp = data_t26_aqt2_tire_temp_encode(42.5f);
    
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
    
    std::cout << "Received temperature: " << received_temperature << " (Expected: ~42.5)" << std::endl;
    assert(received);
    assert(std::abs(received_temperature - 42.5f) < 0.1f);
    
    // ICD shares 0x500-0x502 with autonomous frames on a different bus.
    CanBridgeImpl data_bridge(node.get(), "data_t26");
    bool got_result = false, got_response = false, got_wrong_bus = false;
    auto result_sub = node->create_subscription<lart_msgs::msg::IcdResult>(
        "/data/dbc/icd_result", rclcpp::QoS(10).best_effort(),
        [&](lart_msgs::msg::IcdResult::SharedPtr msg) {
            if (std::abs(msg->icd_current + 123.456f) > 0.001f ||
                std::abs(msg->icd_ubat - 12.345f) > 0.001f || msg->icd_msgcounter != 10)
                throw std::runtime_error("ICD result scaling mismatch");
            got_result = true;
        });
    constexpr uint64_t article = UINT64_C(72057594037927935);
    auto response_sub = node->create_subscription<lart_msgs::msg::IcdResponse>(
        "/data/dbc/icd_response", rclcpp::QoS(10).best_effort(),
        [&](lart_msgs::msg::IcdResponse::SharedPtr msg) {
            if (static_cast<uint8_t>(msg->resp_muxid) != 228 || msg->resp_articlenumber != article || msg->resp_fwmajor != 0)
                throw std::runtime_error("ICD precision/multiplexer mismatch");
            got_response = true;
        });
    auto wrong_sub = node->create_subscription<lart_msgs::msg::DvStatus>(
        "/data/dbc/dv_status", rclcpp::QoS(10).best_effort(),
        [&](lart_msgs::msg::DvStatus::SharedPtr) { got_wrong_bus = true; });
    // Fixed wire payloads independently exercise big-endian signed decoding.
    const uint8_t result_payload[8] = {0xa0, 0xff, 0xfe, 0x1d, 0xc0, 0x30, 0x39, 0};
    const uint8_t response_payload[8] = {228, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    for (int i = 0; i < 50; ++i) {
        data_bridge.handle_frame(0x502, result_payload, 8);
        data_bridge.handle_frame(0x501, response_payload, 8);
        rclcpp::spin_some(node);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    if (!got_result || !got_response || got_wrong_bus)
        throw std::runtime_error("ICD delivery or bus isolation failed");

    std::cout << "✓ All CAN bridge DBC/ROS 2 decoding tests passed successfully!" << std::endl;
    rclcpp::shutdown();
    return 0;
}
