// Auto-generated from DBC files by generate_dbc_api.py. Do not edit.
#include "dbc_api.h"
#if defined(LART_UI_HAVE_RCLCPP) && LART_UI_HAVE_RCLCPP
#if defined(LART_HAVE_LART_MSGS) && LART_HAVE_LART_MSGS
#include <rclcpp/rclcpp.hpp>
#include <mutex>
#include <vector>
extern std::mutex dbc_api_mutex;
#include <lart_msgs/msg/start_programmer.hpp>
#include <lart_msgs/msg/vcu_hv.hpp>
#include <lart_msgs/msg/vcu_ign_r2d.hpp>
#include <lart_msgs/msg/vcu_rpm.hpp>
#include <lart_msgs/msg/vcu_rpm_target.hpp>
#include <lart_msgs/msg/vcu_states.hpp>
#include <lart_msgs/msg/vcu_torque_target.hpp>

void init_dbc_api_subscribers_chunk_5(std::shared_ptr<rclcpp::Node> node, std::vector<rclcpp::SubscriptionBase::SharedPtr>& subs) {
    auto sensor_qos = rclcpp::QoS(10).best_effort();

    subs.push_back(node->create_subscription<lart_msgs::msg::StartProgrammer>(
        "/can/dbc/start_programmer", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::StartProgrammer> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.start_programmer.programming_request = msg->programming_request;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::VcuHv>(
        "/can/dbc/vcu_hv", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::VcuHv> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.vcu_hv.brake_pressure_front = msg->brake_pressure_front;
                dbc_api.vcu_hv.brake_pressure_rear = msg->brake_pressure_rear;
                dbc_api.vcu_hv.hv = msg->hv;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::VcuIgnR2d>(
        "/can/dbc/vcu_ign_r2d", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::VcuIgnR2d> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.vcu_ign_r2d.ignition_auto = msg->ignition_auto;
                dbc_api.vcu_ign_r2d.ignition_manual = msg->ignition_manual;
                dbc_api.vcu_ign_r2d.ignition_switch_raw = msg->ignition_switch_raw;
                dbc_api.vcu_ign_r2d.r2d_auto = msg->r2d_auto;
                dbc_api.vcu_ign_r2d.r2d_button_raw = msg->r2d_button_raw;
                dbc_api.vcu_ign_r2d.r2d_manual = msg->r2d_manual;
                dbc_api.vcu_ign_r2d.shutdown_signal = msg->shutdown_signal;
                dbc_api.vcu_ign_r2d.vcu_state = msg->vcu_state;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::VcuRpm>(
        "/can/dbc/vcu_rpm", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::VcuRpm> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.vcu_rpm.motor_current_left = msg->motor_current_left;
                dbc_api.vcu_rpm.motor_current_right = msg->motor_current_right;
                dbc_api.vcu_rpm.motor_rpm_left = msg->motor_rpm_left;
                dbc_api.vcu_rpm.motor_rpm_right = msg->motor_rpm_right;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::VcuRpmTarget>(
        "/can/dbc/vcu_rpm_target", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::VcuRpmTarget> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.vcu_rpm_target.rpm_target = msg->rpm_target;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::VcuStates>(
        "/can/dbc/vcu_states", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::VcuStates> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.vcu_states.vcu_state = msg->vcu_state;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::VcuTorqueTarget>(
        "/can/dbc/vcu_torque_target", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::VcuTorqueTarget> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.vcu_torque_target.torque_target = msg->torque_target;
            }
        }));
}
#endif
#endif
