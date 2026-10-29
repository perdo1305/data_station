// Auto-generated from DBC files by generate_dbc_api.py. Do not edit.
#include "dbc_api.h"
#if defined(LART_UI_HAVE_RCLCPP) && LART_UI_HAVE_RCLCPP
#if defined(LART_HAVE_LART_MSGS) && LART_HAVE_LART_MSGS
#include <rclcpp/rclcpp.hpp>
#include <mutex>
#include <vector>
extern std::mutex dbc_api_mutex;
#include <lart_msgs/msg/vcu_states.hpp>
#include <lart_msgs/msg/vcu_torque_target.hpp>

void init_dbc_api_subscribers_chunk_7(std::shared_ptr<rclcpp::Node> node, std::vector<rclcpp::SubscriptionBase::SharedPtr>& subs) {
    auto sensor_qos = rclcpp::QoS(10).best_effort();

    subs.push_back(node->create_subscription<lart_msgs::msg::VcuStates>(
        "/pwt/vcu_states", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::VcuStates> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.vcu_states.front_left_speed = msg->front_left_speed;
                dbc_api.vcu_states.front_right_speed = msg->front_right_speed;
                dbc_api.vcu_states.front_wheel_speed_ok = msg->front_wheel_speed_ok;
                dbc_api.vcu_states.road_wheel_angle = msg->road_wheel_angle;
                dbc_api.vcu_states.steering_angle_ok = msg->steering_angle_ok;
                dbc_api.vcu_states.tc_factor_rear_left = msg->tc_factor_rear_left;
                dbc_api.vcu_states.tc_factor_rear_right = msg->tc_factor_rear_right;
                dbc_api.vcu_states.tc_state = msg->tc_state;
                dbc_api.vcu_states.throttle_status = msg->throttle_status;
                dbc_api.vcu_states.tv_shift = msg->tv_shift;
                dbc_api.vcu_states.tv_state = msg->tv_state;
                dbc_api.vcu_states.vcu_state = msg->vcu_state;
                dbc_api.vcu_states.vcu_states_counter = msg->vcu_states_counter;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::VcuTorqueTarget>(
        "/can/vcu_torque_target", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::VcuTorqueTarget> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.vcu_torque_target.torque_target = msg->torque_target;
            }
        }));
}
#endif
#endif
