// Auto-generated from DBC files by generate_dbc_api.py. Do not edit.
#include "dbc_api.h"
#include <string>
#include <cctype>

DbcApi dbc_api = {};

#if defined(LART_UI_HAVE_RCLCPP) && LART_UI_HAVE_RCLCPP
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float32.hpp>
#include <mutex>
#include <vector>

static std::mutex dbc_api_mutex;

void init_dbc_api_subscribers(std::shared_ptr<rclcpp::Node> node, std::vector<rclcpp::SubscriptionBase::SharedPtr>& subs) {
    auto sensor_qos = rclcpp::QoS(10).best_effort();

    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/acu/acu_cpu_temp", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.acu.acu_cpu_temp = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/acu/acu_state", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.acu.acu_state = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/acu/as_state", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.acu.as_state = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/acu/asms", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.acu.asms = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/acu/assi_state", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.acu.assi_state = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/acu/emergency", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.acu.emergency = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/acu/emergency_cause", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.acu.emergency_cause = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/acu/ign", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.acu.ign = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/acu/mission_select", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.acu.mission_select = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/apps_adc_raw/apps1_raw", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.apps_adc_raw.apps1_raw = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/apps_adc_raw/apps2_raw", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.apps_adc_raw.apps2_raw = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/aqt1/bots", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt1.bots = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/aqt1/frt_brk_press", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt1.frt_brk_press = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/aqt1/res", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt1.res = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/aqt2/brake_temp", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt2.brake_temp = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/aqt2/tire_temp", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt2.tire_temp = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/aqt2/wheel_spd", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt2.wheel_spd = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/aqt3/brake_temp", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt3.brake_temp = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/aqt3/tire_temp", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt3.tire_temp = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/aqt3/wheel_spd", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt3.wheel_spd = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/aqt4/emergency", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt4.emergency = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/aqt4/inertia", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt4.inertia = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/aqt4/st_angle", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt4.st_angle = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/aqt4/susp_l", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt4.susp_l = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/aqt4/susp_r", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt4.susp_r = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/aqt5/brake_temp", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt5.brake_temp = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/aqt5/tire_temp", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt5.tire_temp = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/aqt6/brake_temp", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt6.brake_temp = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/aqt6/tire_temp", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt6.tire_temp = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/aqt7/rear_brk_press", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt7.rear_brk_press = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/aqt7/susp_l", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt7.susp_l = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/aqt7/susp_r", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt7.susp_r = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/aqt8/ntc1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt8.ntc1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/aqt8/ntc2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt8.ntc2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/aqt8/ntc3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.aqt8.ntc3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/asf_signals/brake_pressure_front", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.asf_signals.brake_pressure_front = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/asf_signals/brake_pressure_rear", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.asf_signals.brake_pressure_rear = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/asf_signals/ebs_pressure_tank_front", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.asf_signals.ebs_pressure_tank_front = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/asf_signals/ebs_pressure_tank_rear", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.asf_signals.ebs_pressure_tank_rear = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/central_rear/brake_disk_temp", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.central_rear.brake_disk_temp = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/central_rear/tire_temp", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.central_rear.tire_temp = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/central_rear/wheel_speed", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.central_rear.wheel_speed = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/cubemars_feedback/current", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.cubemars_feedback.current = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/cubemars_feedback/driver_temp", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.cubemars_feedback.driver_temp = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/cubemars_feedback/error_code", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.cubemars_feedback.error_code = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/cubemars_feedback/position", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.cubemars_feedback.position = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/cubemars_feedback/speed_rpm", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.cubemars_feedback.speed_rpm = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/cubemars_position_loop/position", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.cubemars_position_loop.position = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/dashboard/ignition_switch_raw", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.dashboard.ignition_switch_raw = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/dashboard/r2d_button_raw", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.dashboard.r2d_button_raw = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/data_box/suspension_level_left", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.data_box.suspension_level_left = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/data_box/suspension_level_right", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.data_box.suspension_level_right = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/dv_dynamics_1/brake_hydr_actual", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.dv_dynamics_1.brake_hydr_actual = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/dv_dynamics_1/brake_hydr_target", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.dv_dynamics_1.brake_hydr_target = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/dv_dynamics_1/motor_moment_actual", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.dv_dynamics_1.motor_moment_actual = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/dv_dynamics_1/motor_moment_target", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.dv_dynamics_1.motor_moment_target = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/dv_dynamics_1/speed_actual", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.dv_dynamics_1.speed_actual = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/dv_dynamics_1/speed_target", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.dv_dynamics_1.speed_target = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/dv_dynamics_1/steering_angle_actual", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.dv_dynamics_1.steering_angle_actual = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/dv_dynamics_1/steering_angle_target", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.dv_dynamics_1.steering_angle_target = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/dv_dynamics_2/acceleration_lateral", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.dv_dynamics_2.acceleration_lateral = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/dv_dynamics_2/acceleration_longitudinal", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.dv_dynamics_2.acceleration_longitudinal = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/dv_dynamics_2/yaw_rate", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.dv_dynamics_2.yaw_rate = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/dv_status/ami_state", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.dv_status.ami_state = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/dv_status/as_status", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.dv_status.as_status = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/dv_status/asb_ebs_state", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.dv_status.asb_ebs_state = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/dv_status/asb_redundancy_state", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.dv_status.asb_redundancy_state = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/dv_status/cones_count_actual", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.dv_status.cones_count_actual = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/dv_status/cones_count_all", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.dv_status.cones_count_all = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/dv_status/lap_counter", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.dv_status.lap_counter = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/dv_status/steering_state", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.dv_status.steering_state = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/front_wheel_l/brake_disc_temp", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.front_wheel_l.brake_disc_temp = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/front_wheel_l/tire_temp", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.front_wheel_l.tire_temp = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/front_wheel_l/wheel_angle", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.front_wheel_l.wheel_angle = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/front_wheel_l/wheel_speed", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.front_wheel_l.wheel_speed = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/front_wheel_r/brake_disc_temp", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.front_wheel_r.brake_disc_temp = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/front_wheel_r/tire_temp", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.front_wheel_r.tire_temp = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/front_wheel_r/wheel_angle", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.front_wheel_r.wheel_angle = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/front_wheel_r/wheel_speed", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.front_wheel_r.wheel_speed = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_ac_dc_current/actual_accurrent", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_ac_dc_current.actual_accurrent = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_ac_dc_current/actual_dccurrent", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_ac_dc_current.actual_dccurrent = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_erpm_duty_voltage/actual_duty", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_erpm_duty_voltage.actual_duty = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_erpm_duty_voltage/actual_erpm", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_erpm_duty_voltage.actual_erpm = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_erpm_duty_voltage/actual_inputvoltage", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_erpm_duty_voltage.actual_inputvoltage = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_foc/actual_foc_id", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_foc.actual_foc_id = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_foc/actual_foc_iq", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_foc.actual_foc_iq = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_minmaxaccurrent/availablemaxaccurrent", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_minmaxaccurrent.availablemaxaccurrent = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_minmaxaccurrent/availableminaccurrent", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_minmaxaccurrent.availableminaccurrent = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_minmaxaccurrent/maxaccurrent", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_minmaxaccurrent.maxaccurrent = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_minmaxaccurrent/minaccurrent", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_minmaxaccurrent.minaccurrent = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_minmaxdccurrent/availablemaxdccurrent", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_minmaxdccurrent.availablemaxdccurrent = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_minmaxdccurrent/availablemindccurrent", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_minmaxdccurrent.availablemindccurrent = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_minmaxdccurrent/maxdccurrent", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_minmaxdccurrent.maxdccurrent = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_minmaxdccurrent/mindccurrent", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_minmaxdccurrent.mindccurrent = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_misc/actual_brake", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_misc.actual_brake = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_misc/actual_throttle", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_misc.actual_throttle = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_misc/can_map_version", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_misc.can_map_version = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_misc/capacitor_temp_limit", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_misc.capacitor_temp_limit = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_misc/dc_current_limit", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_misc.dc_current_limit = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_misc/digital_input_1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_misc.digital_input_1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_misc/digital_input_2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_misc.digital_input_2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_misc/digital_input_3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_misc.digital_input_3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_misc/digital_input_4", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_misc.digital_input_4 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_misc/digital_output_1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_misc.digital_output_1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_misc/digital_output_2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_misc.digital_output_2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_misc/digital_output_3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_misc.digital_output_3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_misc/digital_output_4", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_misc.digital_output_4 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_misc/drive_enable", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_misc.drive_enable = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_misc/drive_enable_limit", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_misc.drive_enable_limit = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_misc/igbt_accel_limit", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_misc.igbt_accel_limit = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_misc/igbt_temp_limit", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_misc.igbt_temp_limit = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_misc/input_voltage_limit", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_misc.input_voltage_limit = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_misc/motor_accel_limit", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_misc.motor_accel_limit = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_misc/motor_temp_limit", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_misc.motor_temp_limit = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_misc/power_limit", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_misc.power_limit = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_misc/rpm_max_limit", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_misc.rpm_max_limit = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_misc/rpm_min_limit", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_misc.rpm_min_limit = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_setaccurrent/cmd_targetaccurrent", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_setaccurrent.cmd_targetaccurrent = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_setbrakecurrent/cmd_targetbrakecurrent", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_setbrakecurrent.cmd_targetbrakecurrent = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_setdigoutput/cmd_setdigoutput1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_setdigoutput.cmd_setdigoutput1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_setdigoutput/cmd_setdigoutput2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_setdigoutput.cmd_setdigoutput2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_setdigoutput/cmd_setdigoutput3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_setdigoutput.cmd_setdigoutput3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_setdigoutput/cmd_setdigoutput4", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_setdigoutput.cmd_setdigoutput4 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_setdriveenable/cmd_driveenable", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_setdriveenable.cmd_driveenable = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_seterpm/cmd_targetspeed", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_seterpm.cmd_targetspeed = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_setmaxacbrakecurrent/cmd_maxacbrakecurrent", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_setmaxacbrakecurrent.cmd_maxacbrakecurrent = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_setmaxaccurrent/cmd_maxaccurrent", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_setmaxaccurrent.cmd_maxaccurrent = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_setmaxdcbrakecurrent/cmd_maxdcbrakecurrent", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_setmaxdcbrakecurrent.cmd_maxdcbrakecurrent = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_setmaxdccurrent/cmd_maxdccurrent", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_setmaxdccurrent.cmd_maxdccurrent = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_setposition/cmd_targetposition", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_setposition.cmd_targetposition = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_setrelbrakecurrent/cmd_targerelativebrakecurrent", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_setrelbrakecurrent.cmd_targerelativebrakecurrent = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_setrelcurrent/cmd_targetrelativecurrent", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_setrelcurrent.cmd_targetrelativecurrent = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_targetiq/controlmode", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_targetiq.controlmode = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_targetiq/ismotorstill", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_targetiq.ismotorstill = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_targetiq/motorposition", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_targetiq.motorposition = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_targetiq/targetiq", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_targetiq.targetiq = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_temperatures/actual_faultcode", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_temperatures.actual_faultcode = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_temperatures/actual_tempcontroller", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_temperatures.actual_tempcontroller = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/hv500_temperatures/actual_tempmotor", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_temperatures.actual_tempmotor = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/ivt_id_cmd", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.ivt_id_cmd = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_10_set_can_id_sn_i", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_10_set_can_id_sn_i = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_10_set_can_id_val_i", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_10_set_can_id_val_i = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_11_set_can_id_sn_u1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_11_set_can_id_sn_u1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_11_set_can_id_val_u1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_11_set_can_id_val_u1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_12_set_can_id_sn_u2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_12_set_can_id_sn_u2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_12_set_can_id_val_u2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_12_set_can_id_val_u2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_13_set_can_id_sn_u3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_13_set_can_id_sn_u3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_13_set_can_id_val_u3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_13_set_can_id_val_u3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_14_set_can_id_sn_t", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_14_set_can_id_sn_t = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_14_set_can_id_val_t", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_14_set_can_id_val_t = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_15_set_can_id_sn_w", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_15_set_can_id_sn_w = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_15_set_can_id_val_w", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_15_set_can_id_val_w = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_16_set_can_id_sn_as", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_16_set_can_id_sn_as = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_16_set_can_id_val_as", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_16_set_can_id_val_as = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_17_set_can_id_sn_wh", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_17_set_can_id_sn_wh = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_17_set_can_id_val_wh", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_17_set_can_id_val_wh = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_1d_set_can_id_sn_command", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_1d_set_can_id_sn_command = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_1d_set_can_id_val_command", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_1d_set_can_id_val_command = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_1f_set_can_id_sn_response", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_1f_set_can_id_sn_response = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_1f_set_can_id_val_response", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_1f_set_can_id_val_response = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_20_conf_cycletime_i", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_20_conf_cycletime_i = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_20_conf_endianess_i", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_20_conf_endianess_i = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_20_conf_sign_i", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_20_conf_sign_i = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_20_conf_triggermode_i", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_20_conf_triggermode_i = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_21_conf_cycletime_u1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_21_conf_cycletime_u1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_21_conf_endianess_u1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_21_conf_endianess_u1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_21_conf_sign_u1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_21_conf_sign_u1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_21_conf_triggermode_u1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_21_conf_triggermode_u1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_22_conf_cycletime_u2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_22_conf_cycletime_u2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_22_conf_endianess_u2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_22_conf_endianess_u2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_22_conf_sign_u2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_22_conf_sign_u2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_22_conf_triggermode_u2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_22_conf_triggermode_u2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_23_conf_cycletime_u3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_23_conf_cycletime_u3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_23_conf_endianess_u3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_23_conf_endianess_u3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_23_conf_sign_u3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_23_conf_sign_u3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_23_conf_triggermode_u3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_23_conf_triggermode_u3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_24_conf_cycletime_t", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_24_conf_cycletime_t = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_24_conf_endianess_t", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_24_conf_endianess_t = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_24_conf_sign_t", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_24_conf_sign_t = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_24_conf_triggermode_t", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_24_conf_triggermode_t = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_25_conf_cycletime_w", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_25_conf_cycletime_w = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_25_conf_endianess_w", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_25_conf_endianess_w = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_25_conf_sign_w", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_25_conf_sign_w = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_25_conf_triggermode_w", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_25_conf_triggermode_w = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_26_conf_cycletime_as", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_26_conf_cycletime_as = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_26_conf_endianess_as", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_26_conf_endianess_as = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_26_conf_sign_as", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_26_conf_sign_as = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_26_conf_triggermode_as", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_26_conf_triggermode_as = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_27_conf_cycletime_wh", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_27_conf_cycletime_wh = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_27_conf_endianess_wh", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_27_conf_endianess_wh = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_27_conf_sign_wh", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_27_conf_sign_wh = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_27_conf_triggermode_wh", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_27_conf_triggermode_wh = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_30_reset_device_sn", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_30_reset_device_sn = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_30_reset_item", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_30_reset_item = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_31_00_trigger_i", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_31_00_trigger_i = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_31_01_trigger_u1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_31_01_trigger_u1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_31_02_trigger_u2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_31_02_trigger_u2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_31_03_trigger_u3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_31_03_trigger_u3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_31_04_trigger_t", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_31_04_trigger_t = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_31_05_trigger_w", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_31_05_trigger_w = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_31_06_trigger_as", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_31_06_trigger_as = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_31_07_trigger_wh", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_31_07_trigger_wh = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_31_trigger_spare", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_31_trigger_spare = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_32_store_dummy", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_32_store_dummy = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_33_oc_test_time", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_33_oc_test_time = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_34_actual_mode", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_34_actual_mode = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_34_code_level", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_34_code_level = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_34_startup_mode", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_34_startup_mode = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_35_oc_pos_reset_threshold", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_35_oc_pos_reset_threshold = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_35_oc_pos_set_threshold", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_35_oc_pos_set_threshold = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_36_oc_neg_reset_threshold", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_36_oc_neg_reset_threshold = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_36_oc_neg_set_threshold", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_36_oc_neg_set_threshold = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_3a_restart_to_bitrate", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_3a_restart_to_bitrate = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_3d_restart_default_dummy", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_3d_restart_default_dummy = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_3f_restart_dummy", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_3f_restart_dummy = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_40_get_measerror_item", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_40_get_measerror_item = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_41_get_systemerror_item", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_41_get_systemerror_item = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_42_get_alllogdata_item", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_42_get_alllogdata_item = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_43_get_logdata_item", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_43_get_logdata_item = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_50_get_can_id_sn_i", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_50_get_can_id_sn_i = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_51_get_can_id_sn_u1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_51_get_can_id_sn_u1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_52_get_can_id_sn_u2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_52_get_can_id_sn_u2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_53_get_can_id_sn_u3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_53_get_can_id_sn_u3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_54_get_can_id_sn_t", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_54_get_can_id_sn_t = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_55_get_can_id_sn_w", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_55_get_can_id_sn_w = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_56_get_can_id_sn_as", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_56_get_can_id_sn_as = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_57_get_can_id_sn_wh", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_57_get_can_id_sn_wh = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_5d_get_can_id_sn_command", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_5d_get_can_id_sn_command = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_5f_get_can_id_sn_response", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_5f_get_can_id_sn_response = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_60_get_result_dummy_i", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_60_get_result_dummy_i = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_61_get_result_dummy_u1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_61_get_result_dummy_u1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_62_get_result_dummy_u2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_62_get_result_dummy_u2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_63_get_result_dummy_u3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_63_get_result_dummy_u3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_64_get_result_dummy_t", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_64_get_result_dummy_t = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_65_get_result_dummy_w", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_65_get_result_dummy_w = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_66_get_result_dummy_as", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_66_get_result_dummy_as = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_67_get_result_dummy_wh", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_67_get_result_dummy_wh = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_73_get_oc_test_dummy", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_73_get_oc_test_dummy = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_74_get_mode_dummy", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_74_get_mode_dummy = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_75_get_oc_pos_dummy", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_75_get_oc_pos_dummy = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_76_get_oc_neg_dummy", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_76_get_oc_neg_dummy = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_79_get_device_id_dummy", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_79_get_device_id_dummy = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_7a_get_sw_version_dummy", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_7a_get_sw_version_dummy = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_7b_get_serien_nr_dummy", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_7b_get_serien_nr_dummy = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_cmd/val_7c_get_art_nr_dummy", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.val_7c_get_art_nr_dummy = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/a0_resp_cycletime_i", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.a0_resp_cycletime_i = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/a0_resp_endianess_i", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.a0_resp_endianess_i = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/a0_resp_sign_i", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.a0_resp_sign_i = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/a0_resp_triggermode_i", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.a0_resp_triggermode_i = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/a1_resp_cycletime_u1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.a1_resp_cycletime_u1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/a1_resp_endianess_u1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.a1_resp_endianess_u1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/a1_resp_sign_u1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.a1_resp_sign_u1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/a1_resp_triggermode_u1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.a1_resp_triggermode_u1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/a2_resp_cycletime_u2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.a2_resp_cycletime_u2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/a2_resp_endianess_u2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.a2_resp_endianess_u2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/a2_resp_sign_u2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.a2_resp_sign_u2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/a2_resp_triggermode_u2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.a2_resp_triggermode_u2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/a3_resp_cycletime_u3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.a3_resp_cycletime_u3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/a3_resp_endianess_u3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.a3_resp_endianess_u3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/a3_resp_sign_u3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.a3_resp_sign_u3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/a3_resp_triggermode_u3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.a3_resp_triggermode_u3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/a4_resp_cycletime_t", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.a4_resp_cycletime_t = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/a4_resp_endianess_t", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.a4_resp_endianess_t = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/a4_resp_sign_t", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.a4_resp_sign_t = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/a4_resp_triggermode_t", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.a4_resp_triggermode_t = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/a5_resp_cycletime_w", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.a5_resp_cycletime_w = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/a5_resp_endianess_w", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.a5_resp_endianess_w = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/a5_resp_sign_w", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.a5_resp_sign_w = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/a5_resp_triggermode_w", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.a5_resp_triggermode_w = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/a6_resp_cycletime_as", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.a6_resp_cycletime_as = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/a6_resp_endianess_as", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.a6_resp_endianess_as = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/a6_resp_sign_as", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.a6_resp_sign_as = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/a6_resp_triggermode_as", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.a6_resp_triggermode_as = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/a7_resp_cycletime_wh", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.a7_resp_cycletime_wh = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/a7_resp_endianess_wh", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.a7_resp_endianess_wh = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/a7_resp_sign_wh", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.a7_resp_sign_wh = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/a7_resp_triggermode_wh", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.a7_resp_triggermode_wh = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/b0_resp_reset_device_sn", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.b0_resp_reset_device_sn = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/b0_resp_reset_item", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.b0_resp_reset_item = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/b1_00_trigger_i", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.b1_00_trigger_i = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/b1_01_trigger_u1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.b1_01_trigger_u1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/b1_02_trigger_u2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.b1_02_trigger_u2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/b1_03_trigger_u3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.b1_03_trigger_u3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/b1_04_trigger_t", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.b1_04_trigger_t = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/b1_05_trigger_w", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.b1_05_trigger_w = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/b1_06_trigger_as", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.b1_06_trigger_as = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/b1_07_trigger_wh", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.b1_07_trigger_wh = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/b1_trigger_spare", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.b1_trigger_spare = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/b2_resp_store_device_sn", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.b2_resp_store_device_sn = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/b2_resp_store_dummy", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.b2_resp_store_dummy = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/b3_resp_oc_test_time", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.b3_resp_oc_test_time = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/b4_resp_actual_mode", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.b4_resp_actual_mode = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/b4_resp_code_level", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.b4_resp_code_level = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/b4_resp_startup_mode", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.b4_resp_startup_mode = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/b5_resp_oc_pos_reset_threshold", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.b5_resp_oc_pos_reset_threshold = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/b5_resp_oc_pos_set_threshold", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.b5_resp_oc_pos_set_threshold = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/b6_resp_oc_neg_reset_threshold", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.b6_resp_oc_neg_reset_threshold = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/b6_resp_oc_neg_set_threshold", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.b6_resp_oc_neg_set_threshold = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/b9_resp_device_com", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.b9_resp_device_com = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/b9_resp_device_current", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.b9_resp_device_current = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/b9_resp_device_toi", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.b9_resp_device_toi = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/b9_resp_device_type", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.b9_resp_device_type = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/b9_resp_device_v_supply", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.b9_resp_device_v_supply = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/b9_resp_device_voltage_chan", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.b9_resp_device_voltage_chan = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/ba_resp_sw_ver_day", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.ba_resp_sw_ver_day = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/ba_resp_sw_ver_internal", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.ba_resp_sw_ver_internal = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/ba_resp_sw_ver_major_hw", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.ba_resp_sw_ver_major_hw = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/ba_resp_sw_ver_minor", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.ba_resp_sw_ver_minor = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/ba_resp_sw_ver_month", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.ba_resp_sw_ver_month = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/ba_resp_sw_ver_revision", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.ba_resp_sw_ver_revision = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/ba_resp_sw_ver_vear", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.ba_resp_sw_ver_vear = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/bb_resp_serien_nr", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.bb_resp_serien_nr = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/bc_resp_art_n", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.bc_resp_art_n = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/bf_restart_alive_cmd_id", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.bf_restart_alive_cmd_id = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/bf_restart_alive_sn", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.bf_restart_alive_sn = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/ff_wrong_command", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.ff_wrong_command = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/ivt_id_response", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.ivt_id_response = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/val_80_resp_measerror_count_mask", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.val_80_resp_measerror_count_mask = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/val_80_resp_measerror_item", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.val_80_resp_measerror_item = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/val_81_resp_systemerror_count_mask", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.val_81_resp_systemerror_count_mask = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/val_81_resp_systemerror_item", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.val_81_resp_systemerror_item = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/val_82_resp_alllogdata_counter", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.val_82_resp_alllogdata_counter = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/val_82_resp_alllogdata_item", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.val_82_resp_alllogdata_item = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/val_83_resp_logdata_counter", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.val_83_resp_logdata_counter = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/val_83_resp_logdata_item", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.val_83_resp_logdata_item = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/val_90_resp_can_id_sn_i", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.val_90_resp_can_id_sn_i = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/val_90_resp_can_id_val_i", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.val_90_resp_can_id_val_i = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/val_91_resp_can_id_sn_u1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.val_91_resp_can_id_sn_u1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/val_91_resp_can_id_val_u1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.val_91_resp_can_id_val_u1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/val_92_resp_can_id_sn_u2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.val_92_resp_can_id_sn_u2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/val_92_resp_can_id_val_u2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.val_92_resp_can_id_val_u2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/val_93_resp_can_id_sn_u3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.val_93_resp_can_id_sn_u3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/val_93_resp_can_id_val_u3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.val_93_resp_can_id_val_u3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/val_94_resp_can_id_sn_t", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.val_94_resp_can_id_sn_t = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/val_94_resp_can_id_val_t", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.val_94_resp_can_id_val_t = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/val_95_resp_can_id_sn_w", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.val_95_resp_can_id_sn_w = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/val_95_resp_can_id_val_w", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.val_95_resp_can_id_val_w = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/val_96_resp_can_id_sn_as", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.val_96_resp_can_id_sn_as = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/val_96_resp_can_id_val_as", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.val_96_resp_can_id_val_as = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/val_97_resp_can_id_sn_wh", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.val_97_resp_can_id_sn_wh = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/val_97_resp_can_id_val_wh", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.val_97_resp_can_id_val_wh = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/val_9d_resp_can_id_sn_command", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.val_9d_resp_can_id_sn_command = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/val_9d_resp_can_id_val_command", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.val_9d_resp_can_id_val_command = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/val_9f_resp_can_id_sn_response", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.val_9f_resp_can_id_sn_response = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_response/val_9f_resp_can_id_val_response", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.val_9f_resp_can_id_val_response = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_as/ivt_id_result_as", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_as.ivt_id_result_as = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_as/ivt_msgcount_result_as", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_as.ivt_msgcount_result_as = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_as/ivt_result_as", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_as.ivt_result_as = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_as/ivt_result_as_channel_error", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_as.ivt_result_as_channel_error = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_as/ivt_result_as_measurement_error", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_as.ivt_result_as_measurement_error = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_as/ivt_result_as_ocs", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_as.ivt_result_as_ocs = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_as/ivt_result_as_system_error", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_as.ivt_result_as_system_error = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_i/ivt_id_result_i", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_i.ivt_id_result_i = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_i/ivt_msgcount_result_i", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_i.ivt_msgcount_result_i = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_i/ivt_result_i", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_i.ivt_result_i = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_i/ivt_result_i_channel_error", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_i.ivt_result_i_channel_error = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_i/ivt_result_i_measurement_error", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_i.ivt_result_i_measurement_error = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_i/ivt_result_i_ocs", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_i.ivt_result_i_ocs = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_i/ivt_result_i_system_error", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_i.ivt_result_i_system_error = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_t/ivt_id_result_t", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_t.ivt_id_result_t = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_t/ivt_msgcount_result_t", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_t.ivt_msgcount_result_t = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_t/ivt_result_t", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_t.ivt_result_t = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_t/ivt_result_t_channel_error", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_t.ivt_result_t_channel_error = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_t/ivt_result_t_measurement_error", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_t.ivt_result_t_measurement_error = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_t/ivt_result_t_ocs", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_t.ivt_result_t_ocs = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_t/ivt_result_t_system_error", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_t.ivt_result_t_system_error = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_u1/ivt_id_result_u1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_u1.ivt_id_result_u1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_u1/ivt_msgcount_result_u1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_u1.ivt_msgcount_result_u1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_u1/ivt_result_u1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_u1.ivt_result_u1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_u1/ivt_result_u1_channel_error", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_u1.ivt_result_u1_channel_error = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_u1/ivt_result_u1_measurement_error", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_u1.ivt_result_u1_measurement_error = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_u1/ivt_result_u1_ocs", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_u1.ivt_result_u1_ocs = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_u1/ivt_result_u1_system_error", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_u1.ivt_result_u1_system_error = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_u2/ivt_id_result_u2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_u2.ivt_id_result_u2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_u2/ivt_msgcount_result_u2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_u2.ivt_msgcount_result_u2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_u2/ivt_result_u2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_u2.ivt_result_u2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_u2/ivt_result_u2_channel_error", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_u2.ivt_result_u2_channel_error = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_u2/ivt_result_u2_measurement_error", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_u2.ivt_result_u2_measurement_error = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_u2/ivt_result_u2_ocs", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_u2.ivt_result_u2_ocs = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_u2/ivt_result_u2_system_error", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_u2.ivt_result_u2_system_error = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_u3/ivt_id_result_u3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_u3.ivt_id_result_u3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_u3/ivt_msgcount_result_u3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_u3.ivt_msgcount_result_u3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_u3/ivt_result_u3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_u3.ivt_result_u3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_u3/ivt_result_u3_channel_error", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_u3.ivt_result_u3_channel_error = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_u3/ivt_result_u3_measurement_error", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_u3.ivt_result_u3_measurement_error = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_u3/ivt_result_u3_ocs", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_u3.ivt_result_u3_ocs = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_u3/ivt_result_u3_system_error", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_u3.ivt_result_u3_system_error = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_w/ivt_id_result_w", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_w.ivt_id_result_w = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_w/ivt_msgcount_result_w", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_w.ivt_msgcount_result_w = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_w/ivt_result_w", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_w.ivt_result_w = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_w/ivt_result_w_channel_error", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_w.ivt_result_w_channel_error = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_w/ivt_result_w_measurement_error", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_w.ivt_result_w_measurement_error = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_w/ivt_result_w_ocs", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_w.ivt_result_w_ocs = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_w/ivt_result_w_system_error", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_w.ivt_result_w_system_error = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_wh/ivt_id_result_wh", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_wh.ivt_id_result_wh = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_wh/ivt_msgcount_result_wh", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_wh.ivt_msgcount_result_wh = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_wh/ivt_result_wh", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_wh.ivt_result_wh = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_wh/ivt_result_wh_channel_error", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_wh.ivt_result_wh_channel_error = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_wh/ivt_result_wh_measurement_error", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_wh.ivt_result_wh_measurement_error = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_wh/ivt_result_wh_ocs", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_wh.ivt_result_wh_ocs = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/ivt_msg_result_wh/ivt_result_wh_system_error", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_wh.ivt_result_wh_system_error = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/jetson/as_mission", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.jetson.as_mission = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/jetson/as_state", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.jetson.as_state = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/jetson/cpu", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.jetson.cpu = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/jetson/emergency_cause", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.jetson.emergency_cause = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/jetson/gpu", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.jetson.gpu = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/jetson/temperature", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.jetson.temperature = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/master_msc_id_1/adbms_pec_error", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.master_msc_id_1.adbms_pec_error = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/master_msc_id_1/ams_current_draw", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.master_msc_id_1.ams_current_draw = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/master_msc_id_1/fault_counter", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.master_msc_id_1.fault_counter = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/master_msc_id_1/master_fan_pwm", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.master_msc_id_1.master_fan_pwm = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/master_msc_id_1/master_firmware_version", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.master_msc_id_1.master_firmware_version = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/master_msc_id_1/master_state", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.master_msc_id_1.master_state = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/master_msc_id_1/mcu_temperature", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.master_msc_id_1.mcu_temperature = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/master_msc_id_1/mcu_vref", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.master_msc_id_1.mcu_vref = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/master_msc_id_2/fault1_code", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.master_msc_id_2.fault1_code = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/master_msc_id_2/fault1_index_type", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.master_msc_id_2.fault1_index_type = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/master_msc_id_2/fault1_index_value", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.master_msc_id_2.fault1_index_value = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/master_msc_id_2/fault2_code", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.master_msc_id_2.fault2_code = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/master_msc_id_2/fault2_index_type", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.master_msc_id_2.fault2_index_type = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/master_msc_id_2/fault2_index_value", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.master_msc_id_2.fault2_index_value = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/master_msc_id_2/master_runtime", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.master_msc_id_2.master_runtime = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/master_msc_id_3/overall_maximum_temperature", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.master_msc_id_3.overall_maximum_temperature = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/master_msc_id_3/overall_maximum_voltage", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.master_msc_id_3.overall_maximum_voltage = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/master_msc_id_3/overall_minimum_temperature", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.master_msc_id_3.overall_minimum_temperature = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/master_msc_id_3/overall_minimum_voltage", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.master_msc_id_3.overall_minimum_voltage = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/master_msc_id_4/slaves_detected", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.master_msc_id_4.slaves_detected = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/master_precharge_id_1/precharge_ctc_air_min_state", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.master_precharge_id_1.precharge_ctc_air_min_state = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/master_precharge_id_1/precharge_ctc_air_pos_state", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.master_precharge_id_1.precharge_ctc_air_pos_state = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/master_precharge_id_1/precharge_ctc_charge_state", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.master_precharge_id_1.precharge_ctc_charge_state = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/master_precharge_id_1/precharge_ctc_discharge_state", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.master_precharge_id_1.precharge_ctc_discharge_state = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/master_precharge_id_1/precharge_state", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.master_precharge_id_1.precharge_state = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/master_soc_accumulator/soc_float", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.master_soc_accumulator.soc_float = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/master_soc_accumulator/soc_integer", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.master_soc_accumulator.soc_integer = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/pedal_box/apps1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.pedal_box.apps1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/pedal_box/apps2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.pedal_box.apps2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/rear_wheel_l/ignition", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.rear_wheel_l.ignition = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/rear_wheel_l/mission_select", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.rear_wheel_l.mission_select = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/rear_wheel_l/shutdown_circuit", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.rear_wheel_l.shutdown_circuit = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/rear_wheel_r/hydrauli_brake_pressure", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.rear_wheel_r.hydrauli_brake_pressure = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/rear_wheel_r/ntc1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.rear_wheel_r.ntc1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/rear_wheel_r/ntc2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.rear_wheel_r.ntc2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/rear_wheel_r/ntc3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.rear_wheel_r.ntc3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/rear_wheel_r/ntc4", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.rear_wheel_r.ntc4 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/rear_wheel_r/ntc5", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.rear_wheel_r.ntc5 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/rear_wheel_r/ntc6", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.rear_wheel_r.ntc6 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/res/signal", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.res.signal = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slam_stats_can/cones_count_actual", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slam_stats_can.cones_count_actual = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slam_stats_can/cones_count_all", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slam_stats_can.cones_count_all = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slam_stats_can/lap_counter", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slam_stats_can.lap_counter = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_01_msc_id_1/module_voltage_avg", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_01_msc_id_1.module_voltage_avg = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_01_msc_id_1/module_voltage_max", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_01_msc_id_1.module_voltage_max = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_01_msc_id_1/module_voltage_min", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_01_msc_id_1.module_voltage_min = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_01_msc_id_1/module_voltage_sum", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_01_msc_id_1.module_voltage_sum = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_01_msc_id_2/module_ic_temperature", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_01_msc_id_2.module_ic_temperature = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_01_msc_id_2/module_ic_voltage", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_01_msc_id_2.module_ic_voltage = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_01_msc_id_2/module_open_wire", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_01_msc_id_2.module_open_wire = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_01_msc_id_2/module_overvoltage", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_01_msc_id_2.module_overvoltage = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_01_msc_id_2/module_under_over_identifier", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_01_msc_id_2.module_under_over_identifier = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_01_msc_id_2/module_undervoltage", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_01_msc_id_2.module_undervoltage = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_01_msc_id_2/module_voltage_delta", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_01_msc_id_2.module_voltage_delta = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_01_temperature_id_1/temperature_value_1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_01_temperature_id_1.temperature_value_1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_01_temperature_id_1/temperature_value_2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_01_temperature_id_1.temperature_value_2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_01_temperature_id_1/temperature_value_3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_01_temperature_id_1.temperature_value_3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_01_temperature_id_1/temperature_value_4", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_01_temperature_id_1.temperature_value_4 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_01_temperature_id_2/temperature_delta", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_01_temperature_id_2.temperature_delta = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_01_temperature_id_2/temperature_maximum", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_01_temperature_id_2.temperature_maximum = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_01_temperature_id_2/temperature_value_5", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_01_temperature_id_2.temperature_value_5 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_01_temperature_id_2/temperature_value_6", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_01_temperature_id_2.temperature_value_6 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_01_voltage_id_1/cell_voltage_1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_01_voltage_id_1.cell_voltage_1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_01_voltage_id_1/cell_voltage_2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_01_voltage_id_1.cell_voltage_2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_01_voltage_id_1/cell_voltage_3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_01_voltage_id_1.cell_voltage_3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_01_voltage_id_1/cell_voltage_4", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_01_voltage_id_1.cell_voltage_4 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_01_voltage_id_2/cell_voltage_5", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_01_voltage_id_2.cell_voltage_5 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_01_voltage_id_2/cell_voltage_6", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_01_voltage_id_2.cell_voltage_6 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_01_voltage_id_2/cell_voltage_7", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_01_voltage_id_2.cell_voltage_7 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_01_voltage_id_2/cell_voltage_8", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_01_voltage_id_2.cell_voltage_8 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_01_voltage_id_3/cell_voltage_10", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_01_voltage_id_3.cell_voltage_10 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_01_voltage_id_3/cell_voltage_11", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_01_voltage_id_3.cell_voltage_11 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_01_voltage_id_3/cell_voltage_12", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_01_voltage_id_3.cell_voltage_12 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_01_voltage_id_3/cell_voltage_9", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_01_voltage_id_3.cell_voltage_9 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_02_msc_id_1/module_voltage_avg", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_02_msc_id_1.module_voltage_avg = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_02_msc_id_1/module_voltage_max", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_02_msc_id_1.module_voltage_max = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_02_msc_id_1/module_voltage_min", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_02_msc_id_1.module_voltage_min = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_02_msc_id_1/module_voltage_sum", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_02_msc_id_1.module_voltage_sum = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_02_msc_id_2/module_ic_temperature", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_02_msc_id_2.module_ic_temperature = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_02_msc_id_2/module_ic_voltage", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_02_msc_id_2.module_ic_voltage = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_02_msc_id_2/module_open_wire", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_02_msc_id_2.module_open_wire = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_02_msc_id_2/module_overvoltage", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_02_msc_id_2.module_overvoltage = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_02_msc_id_2/module_under_over_identifier", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_02_msc_id_2.module_under_over_identifier = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_02_msc_id_2/module_undervoltage", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_02_msc_id_2.module_undervoltage = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_02_msc_id_2/module_voltage_delta", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_02_msc_id_2.module_voltage_delta = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_02_temperature_id_1/temperature_value_1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_02_temperature_id_1.temperature_value_1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_02_temperature_id_1/temperature_value_2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_02_temperature_id_1.temperature_value_2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_02_temperature_id_1/temperature_value_3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_02_temperature_id_1.temperature_value_3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_02_temperature_id_1/temperature_value_4", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_02_temperature_id_1.temperature_value_4 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_02_temperature_id_2/temperature_delta", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_02_temperature_id_2.temperature_delta = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_02_temperature_id_2/temperature_maximum", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_02_temperature_id_2.temperature_maximum = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_02_temperature_id_2/temperature_value_5", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_02_temperature_id_2.temperature_value_5 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_02_temperature_id_2/temperature_value_6", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_02_temperature_id_2.temperature_value_6 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_02_voltage_id_1/cell_voltage_1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_02_voltage_id_1.cell_voltage_1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_02_voltage_id_1/cell_voltage_2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_02_voltage_id_1.cell_voltage_2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_02_voltage_id_1/cell_voltage_3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_02_voltage_id_1.cell_voltage_3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_02_voltage_id_1/cell_voltage_4", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_02_voltage_id_1.cell_voltage_4 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_02_voltage_id_2/cell_voltage_5", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_02_voltage_id_2.cell_voltage_5 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_02_voltage_id_2/cell_voltage_6", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_02_voltage_id_2.cell_voltage_6 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_02_voltage_id_2/cell_voltage_7", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_02_voltage_id_2.cell_voltage_7 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_02_voltage_id_2/cell_voltage_8", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_02_voltage_id_2.cell_voltage_8 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_02_voltage_id_3/cell_voltage_10", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_02_voltage_id_3.cell_voltage_10 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_02_voltage_id_3/cell_voltage_11", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_02_voltage_id_3.cell_voltage_11 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_02_voltage_id_3/cell_voltage_12", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_02_voltage_id_3.cell_voltage_12 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_02_voltage_id_3/cell_voltage_9", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_02_voltage_id_3.cell_voltage_9 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_03_msc_id_1/module_voltage_avg", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_03_msc_id_1.module_voltage_avg = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_03_msc_id_1/module_voltage_max", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_03_msc_id_1.module_voltage_max = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_03_msc_id_1/module_voltage_min", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_03_msc_id_1.module_voltage_min = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_03_msc_id_1/module_voltage_sum", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_03_msc_id_1.module_voltage_sum = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_03_msc_id_2/module_ic_temperature", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_03_msc_id_2.module_ic_temperature = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_03_msc_id_2/module_ic_voltage", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_03_msc_id_2.module_ic_voltage = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_03_msc_id_2/module_open_wire", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_03_msc_id_2.module_open_wire = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_03_msc_id_2/module_voltage_delta", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_03_msc_id_2.module_voltage_delta = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_03_temperature_id_1/temperature_value_1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_03_temperature_id_1.temperature_value_1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_03_temperature_id_1/temperature_value_2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_03_temperature_id_1.temperature_value_2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_03_temperature_id_1/temperature_value_3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_03_temperature_id_1.temperature_value_3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_03_temperature_id_1/temperature_value_4", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_03_temperature_id_1.temperature_value_4 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_03_temperature_id_2/temperature_delta", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_03_temperature_id_2.temperature_delta = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_03_temperature_id_2/temperature_maximum", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_03_temperature_id_2.temperature_maximum = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_03_temperature_id_2/temperature_value_5", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_03_temperature_id_2.temperature_value_5 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_03_temperature_id_2/temperature_value_6", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_03_temperature_id_2.temperature_value_6 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_03_voltage_id_1/cell_voltage_1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_03_voltage_id_1.cell_voltage_1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_03_voltage_id_1/cell_voltage_2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_03_voltage_id_1.cell_voltage_2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_03_voltage_id_1/cell_voltage_3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_03_voltage_id_1.cell_voltage_3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_03_voltage_id_1/cell_voltage_4", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_03_voltage_id_1.cell_voltage_4 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_03_voltage_id_2/cell_voltage_5", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_03_voltage_id_2.cell_voltage_5 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_03_voltage_id_2/cell_voltage_6", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_03_voltage_id_2.cell_voltage_6 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_03_voltage_id_2/cell_voltage_7", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_03_voltage_id_2.cell_voltage_7 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_03_voltage_id_2/cell_voltage_8", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_03_voltage_id_2.cell_voltage_8 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_03_voltage_id_3/cell_voltage_10", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_03_voltage_id_3.cell_voltage_10 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_03_voltage_id_3/cell_voltage_11", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_03_voltage_id_3.cell_voltage_11 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_03_voltage_id_3/cell_voltage_12", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_03_voltage_id_3.cell_voltage_12 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_03_voltage_id_3/cell_voltage_9", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_03_voltage_id_3.cell_voltage_9 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_04_msc_id_1/module_voltage_avg", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_04_msc_id_1.module_voltage_avg = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_04_msc_id_1/module_voltage_max", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_04_msc_id_1.module_voltage_max = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_04_msc_id_1/module_voltage_min", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_04_msc_id_1.module_voltage_min = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_04_msc_id_1/module_voltage_sum", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_04_msc_id_1.module_voltage_sum = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_04_msc_id_2/module_ic_temperature", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_04_msc_id_2.module_ic_temperature = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_04_msc_id_2/module_ic_voltage", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_04_msc_id_2.module_ic_voltage = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_04_msc_id_2/module_open_wire", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_04_msc_id_2.module_open_wire = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_04_msc_id_2/module_voltage_delta", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_04_msc_id_2.module_voltage_delta = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_04_temperature_id_1/temperature_value_1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_04_temperature_id_1.temperature_value_1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_04_temperature_id_1/temperature_value_2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_04_temperature_id_1.temperature_value_2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_04_temperature_id_1/temperature_value_3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_04_temperature_id_1.temperature_value_3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_04_temperature_id_1/temperature_value_4", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_04_temperature_id_1.temperature_value_4 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_04_temperature_id_2/temperature_delta", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_04_temperature_id_2.temperature_delta = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_04_temperature_id_2/temperature_maximum", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_04_temperature_id_2.temperature_maximum = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_04_temperature_id_2/temperature_value_5", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_04_temperature_id_2.temperature_value_5 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_04_temperature_id_2/temperature_value_6", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_04_temperature_id_2.temperature_value_6 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_04_voltage_id_1/cell_voltage_1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_04_voltage_id_1.cell_voltage_1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_04_voltage_id_1/cell_voltage_2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_04_voltage_id_1.cell_voltage_2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_04_voltage_id_1/cell_voltage_3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_04_voltage_id_1.cell_voltage_3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_04_voltage_id_1/cell_voltage_4", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_04_voltage_id_1.cell_voltage_4 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_04_voltage_id_2/cell_voltage_5", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_04_voltage_id_2.cell_voltage_5 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_04_voltage_id_2/cell_voltage_6", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_04_voltage_id_2.cell_voltage_6 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_04_voltage_id_2/cell_voltage_7", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_04_voltage_id_2.cell_voltage_7 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_04_voltage_id_2/cell_voltage_8", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_04_voltage_id_2.cell_voltage_8 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_04_voltage_id_3/cell_voltage_10", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_04_voltage_id_3.cell_voltage_10 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_04_voltage_id_3/cell_voltage_11", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_04_voltage_id_3.cell_voltage_11 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_04_voltage_id_3/cell_voltage_12", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_04_voltage_id_3.cell_voltage_12 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_04_voltage_id_3/cell_voltage_9", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_04_voltage_id_3.cell_voltage_9 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_05_msc_id_1/module_voltage_avg", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_05_msc_id_1.module_voltage_avg = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_05_msc_id_1/module_voltage_max", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_05_msc_id_1.module_voltage_max = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_05_msc_id_1/module_voltage_min", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_05_msc_id_1.module_voltage_min = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_05_msc_id_1/module_voltage_sum", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_05_msc_id_1.module_voltage_sum = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_05_msc_id_2/module_ic_temperature", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_05_msc_id_2.module_ic_temperature = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_05_msc_id_2/module_ic_voltage", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_05_msc_id_2.module_ic_voltage = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_05_msc_id_2/module_open_wire", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_05_msc_id_2.module_open_wire = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_05_msc_id_2/module_voltage_delta", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_05_msc_id_2.module_voltage_delta = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_05_temperature_id_1/temperature_value_1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_05_temperature_id_1.temperature_value_1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_05_temperature_id_1/temperature_value_2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_05_temperature_id_1.temperature_value_2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_05_temperature_id_1/temperature_value_3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_05_temperature_id_1.temperature_value_3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_05_temperature_id_1/temperature_value_4", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_05_temperature_id_1.temperature_value_4 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_05_temperature_id_2/temperature_delta", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_05_temperature_id_2.temperature_delta = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_05_temperature_id_2/temperature_maximum", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_05_temperature_id_2.temperature_maximum = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_05_temperature_id_2/temperature_value_5", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_05_temperature_id_2.temperature_value_5 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_05_temperature_id_2/temperature_value_6", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_05_temperature_id_2.temperature_value_6 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_05_voltage_id_1/cell_voltage_1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_05_voltage_id_1.cell_voltage_1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_05_voltage_id_1/cell_voltage_2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_05_voltage_id_1.cell_voltage_2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_05_voltage_id_1/cell_voltage_3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_05_voltage_id_1.cell_voltage_3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_05_voltage_id_1/cell_voltage_4", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_05_voltage_id_1.cell_voltage_4 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_05_voltage_id_2/cell_voltage_5", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_05_voltage_id_2.cell_voltage_5 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_05_voltage_id_2/cell_voltage_6", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_05_voltage_id_2.cell_voltage_6 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_05_voltage_id_2/cell_voltage_7", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_05_voltage_id_2.cell_voltage_7 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_05_voltage_id_2/cell_voltage_8", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_05_voltage_id_2.cell_voltage_8 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_05_voltage_id_3/cell_voltage_10", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_05_voltage_id_3.cell_voltage_10 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_05_voltage_id_3/cell_voltage_11", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_05_voltage_id_3.cell_voltage_11 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_05_voltage_id_3/cell_voltage_12", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_05_voltage_id_3.cell_voltage_12 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_05_voltage_id_3/cell_voltage_9", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_05_voltage_id_3.cell_voltage_9 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_06_msc_id_1/module_voltage_avg", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_06_msc_id_1.module_voltage_avg = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_06_msc_id_1/module_voltage_max", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_06_msc_id_1.module_voltage_max = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_06_msc_id_1/module_voltage_min", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_06_msc_id_1.module_voltage_min = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_06_msc_id_1/module_voltage_sum", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_06_msc_id_1.module_voltage_sum = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_06_msc_id_2/module_ic_temperature", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_06_msc_id_2.module_ic_temperature = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_06_msc_id_2/module_ic_voltage", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_06_msc_id_2.module_ic_voltage = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_06_msc_id_2/module_open_wire", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_06_msc_id_2.module_open_wire = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_06_msc_id_2/module_voltage_delta", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_06_msc_id_2.module_voltage_delta = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_06_temperature_id_1/temperature_value_1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_06_temperature_id_1.temperature_value_1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_06_temperature_id_1/temperature_value_2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_06_temperature_id_1.temperature_value_2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_06_temperature_id_1/temperature_value_3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_06_temperature_id_1.temperature_value_3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_06_temperature_id_1/temperature_value_4", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_06_temperature_id_1.temperature_value_4 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_06_temperature_id_2/temperature_delta", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_06_temperature_id_2.temperature_delta = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_06_temperature_id_2/temperature_maximum", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_06_temperature_id_2.temperature_maximum = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_06_temperature_id_2/temperature_value_5", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_06_temperature_id_2.temperature_value_5 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_06_temperature_id_2/temperature_value_6", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_06_temperature_id_2.temperature_value_6 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_06_voltage_id_1/cell_voltage_1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_06_voltage_id_1.cell_voltage_1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_06_voltage_id_1/cell_voltage_2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_06_voltage_id_1.cell_voltage_2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_06_voltage_id_1/cell_voltage_3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_06_voltage_id_1.cell_voltage_3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_06_voltage_id_1/cell_voltage_4", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_06_voltage_id_1.cell_voltage_4 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_06_voltage_id_2/cell_voltage_5", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_06_voltage_id_2.cell_voltage_5 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_06_voltage_id_2/cell_voltage_6", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_06_voltage_id_2.cell_voltage_6 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_06_voltage_id_2/cell_voltage_7", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_06_voltage_id_2.cell_voltage_7 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_06_voltage_id_2/cell_voltage_8", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_06_voltage_id_2.cell_voltage_8 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_06_voltage_id_3/cell_voltage_10", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_06_voltage_id_3.cell_voltage_10 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_06_voltage_id_3/cell_voltage_11", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_06_voltage_id_3.cell_voltage_11 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_06_voltage_id_3/cell_voltage_12", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_06_voltage_id_3.cell_voltage_12 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_06_voltage_id_3/cell_voltage_9", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_06_voltage_id_3.cell_voltage_9 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_07_msc_id_1/module_voltage_avg", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_07_msc_id_1.module_voltage_avg = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_07_msc_id_1/module_voltage_max", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_07_msc_id_1.module_voltage_max = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_07_msc_id_1/module_voltage_min", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_07_msc_id_1.module_voltage_min = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_07_msc_id_1/module_voltage_sum", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_07_msc_id_1.module_voltage_sum = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_07_msc_id_2/module_ic_temperature", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_07_msc_id_2.module_ic_temperature = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_07_msc_id_2/module_ic_voltage", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_07_msc_id_2.module_ic_voltage = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_07_msc_id_2/module_open_wire", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_07_msc_id_2.module_open_wire = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_07_msc_id_2/module_voltage_delta", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_07_msc_id_2.module_voltage_delta = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_07_temperature_id_1/temperature_value_1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_07_temperature_id_1.temperature_value_1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_07_temperature_id_1/temperature_value_2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_07_temperature_id_1.temperature_value_2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_07_temperature_id_1/temperature_value_3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_07_temperature_id_1.temperature_value_3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_07_temperature_id_1/temperature_value_4", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_07_temperature_id_1.temperature_value_4 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_07_temperature_id_2/temperature_delta", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_07_temperature_id_2.temperature_delta = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_07_temperature_id_2/temperature_maximum", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_07_temperature_id_2.temperature_maximum = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_07_temperature_id_2/temperature_value_5", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_07_temperature_id_2.temperature_value_5 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_07_temperature_id_2/temperature_value_6", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_07_temperature_id_2.temperature_value_6 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_07_voltage_id_1/cell_voltage_1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_07_voltage_id_1.cell_voltage_1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_07_voltage_id_1/cell_voltage_2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_07_voltage_id_1.cell_voltage_2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_07_voltage_id_1/cell_voltage_3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_07_voltage_id_1.cell_voltage_3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_07_voltage_id_1/cell_voltage_4", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_07_voltage_id_1.cell_voltage_4 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_07_voltage_id_2/cell_voltage_5", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_07_voltage_id_2.cell_voltage_5 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_07_voltage_id_2/cell_voltage_6", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_07_voltage_id_2.cell_voltage_6 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_07_voltage_id_2/cell_voltage_7", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_07_voltage_id_2.cell_voltage_7 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_07_voltage_id_2/cell_voltage_8", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_07_voltage_id_2.cell_voltage_8 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_07_voltage_id_3/cell_voltage_10", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_07_voltage_id_3.cell_voltage_10 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_07_voltage_id_3/cell_voltage_11", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_07_voltage_id_3.cell_voltage_11 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_07_voltage_id_3/cell_voltage_12", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_07_voltage_id_3.cell_voltage_12 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_07_voltage_id_3/cell_voltage_9", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_07_voltage_id_3.cell_voltage_9 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_08_msc_id_1/module_voltage_avg", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_08_msc_id_1.module_voltage_avg = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_08_msc_id_1/module_voltage_max", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_08_msc_id_1.module_voltage_max = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_08_msc_id_1/module_voltage_min", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_08_msc_id_1.module_voltage_min = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_08_msc_id_1/module_voltage_sum", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_08_msc_id_1.module_voltage_sum = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_08_msc_id_2/module_ic_temperature", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_08_msc_id_2.module_ic_temperature = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_08_msc_id_2/module_ic_voltage", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_08_msc_id_2.module_ic_voltage = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_08_msc_id_2/module_open_wire", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_08_msc_id_2.module_open_wire = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_08_msc_id_2/module_voltage_delta", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_08_msc_id_2.module_voltage_delta = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_08_temperature_id_1/temperature_value_1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_08_temperature_id_1.temperature_value_1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_08_temperature_id_1/temperature_value_2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_08_temperature_id_1.temperature_value_2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_08_temperature_id_1/temperature_value_3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_08_temperature_id_1.temperature_value_3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_08_temperature_id_1/temperature_value_4", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_08_temperature_id_1.temperature_value_4 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_08_temperature_id_2/temperature_delta", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_08_temperature_id_2.temperature_delta = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_08_temperature_id_2/temperature_maximum", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_08_temperature_id_2.temperature_maximum = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_08_temperature_id_2/temperature_value_5", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_08_temperature_id_2.temperature_value_5 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_08_temperature_id_2/temperature_value_6", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_08_temperature_id_2.temperature_value_6 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_08_voltage_id_1/cell_voltage_1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_08_voltage_id_1.cell_voltage_1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_08_voltage_id_1/cell_voltage_2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_08_voltage_id_1.cell_voltage_2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_08_voltage_id_1/cell_voltage_3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_08_voltage_id_1.cell_voltage_3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_08_voltage_id_1/cell_voltage_4", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_08_voltage_id_1.cell_voltage_4 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_08_voltage_id_2/cell_voltage_5", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_08_voltage_id_2.cell_voltage_5 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_08_voltage_id_2/cell_voltage_6", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_08_voltage_id_2.cell_voltage_6 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_08_voltage_id_2/cell_voltage_7", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_08_voltage_id_2.cell_voltage_7 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_08_voltage_id_2/cell_voltage_8", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_08_voltage_id_2.cell_voltage_8 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_08_voltage_id_3/cell_voltage_10", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_08_voltage_id_3.cell_voltage_10 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_08_voltage_id_3/cell_voltage_11", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_08_voltage_id_3.cell_voltage_11 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_08_voltage_id_3/cell_voltage_12", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_08_voltage_id_3.cell_voltage_12 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_08_voltage_id_3/cell_voltage_9", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_08_voltage_id_3.cell_voltage_9 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_09_msc_id_1/module_voltage_avg", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_09_msc_id_1.module_voltage_avg = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_09_msc_id_1/module_voltage_max", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_09_msc_id_1.module_voltage_max = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_09_msc_id_1/module_voltage_min", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_09_msc_id_1.module_voltage_min = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_09_msc_id_1/module_voltage_sum", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_09_msc_id_1.module_voltage_sum = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_09_msc_id_2/module_ic_temperature", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_09_msc_id_2.module_ic_temperature = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_09_msc_id_2/module_ic_voltage", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_09_msc_id_2.module_ic_voltage = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_09_msc_id_2/module_open_wire", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_09_msc_id_2.module_open_wire = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_09_msc_id_2/module_voltage_delta", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_09_msc_id_2.module_voltage_delta = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_09_temperature_id_1/temperature_value_1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_09_temperature_id_1.temperature_value_1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_09_temperature_id_1/temperature_value_2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_09_temperature_id_1.temperature_value_2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_09_temperature_id_1/temperature_value_3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_09_temperature_id_1.temperature_value_3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_09_temperature_id_1/temperature_value_4", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_09_temperature_id_1.temperature_value_4 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_09_temperature_id_2/temperature_delta", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_09_temperature_id_2.temperature_delta = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_09_temperature_id_2/temperature_maximum", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_09_temperature_id_2.temperature_maximum = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_09_temperature_id_2/temperature_value_5", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_09_temperature_id_2.temperature_value_5 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_09_temperature_id_2/temperature_value_6", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_09_temperature_id_2.temperature_value_6 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_09_voltage_id_1/cell_voltage_1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_09_voltage_id_1.cell_voltage_1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_09_voltage_id_1/cell_voltage_2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_09_voltage_id_1.cell_voltage_2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_09_voltage_id_1/cell_voltage_3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_09_voltage_id_1.cell_voltage_3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_09_voltage_id_1/cell_voltage_4", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_09_voltage_id_1.cell_voltage_4 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_09_voltage_id_2/cell_voltage_5", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_09_voltage_id_2.cell_voltage_5 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_09_voltage_id_2/cell_voltage_6", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_09_voltage_id_2.cell_voltage_6 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_09_voltage_id_2/cell_voltage_7", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_09_voltage_id_2.cell_voltage_7 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_09_voltage_id_2/cell_voltage_8", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_09_voltage_id_2.cell_voltage_8 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_09_voltage_id_3/cell_voltage_10", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_09_voltage_id_3.cell_voltage_10 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_09_voltage_id_3/cell_voltage_11", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_09_voltage_id_3.cell_voltage_11 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_09_voltage_id_3/cell_voltage_12", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_09_voltage_id_3.cell_voltage_12 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_09_voltage_id_3/cell_voltage_9", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_09_voltage_id_3.cell_voltage_9 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_10_msc_id_1/module_voltage_avg", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_10_msc_id_1.module_voltage_avg = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_10_msc_id_1/module_voltage_max", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_10_msc_id_1.module_voltage_max = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_10_msc_id_1/module_voltage_min", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_10_msc_id_1.module_voltage_min = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_10_msc_id_1/module_voltage_sum", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_10_msc_id_1.module_voltage_sum = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_10_msc_id_2/module_ic_temperature", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_10_msc_id_2.module_ic_temperature = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_10_msc_id_2/module_ic_voltage", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_10_msc_id_2.module_ic_voltage = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_10_msc_id_2/module_open_wire", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_10_msc_id_2.module_open_wire = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_10_msc_id_2/module_voltage_delta", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_10_msc_id_2.module_voltage_delta = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_10_temperature_id_1/temperature_value_1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_10_temperature_id_1.temperature_value_1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_10_temperature_id_1/temperature_value_2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_10_temperature_id_1.temperature_value_2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_10_temperature_id_1/temperature_value_3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_10_temperature_id_1.temperature_value_3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_10_temperature_id_1/temperature_value_4", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_10_temperature_id_1.temperature_value_4 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_10_temperature_id_2/temperature_delta", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_10_temperature_id_2.temperature_delta = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_10_temperature_id_2/temperature_maximum", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_10_temperature_id_2.temperature_maximum = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_10_temperature_id_2/temperature_value_5", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_10_temperature_id_2.temperature_value_5 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_10_temperature_id_2/temperature_value_6", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_10_temperature_id_2.temperature_value_6 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_10_voltage_id_1/cell_voltage_1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_10_voltage_id_1.cell_voltage_1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_10_voltage_id_1/cell_voltage_2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_10_voltage_id_1.cell_voltage_2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_10_voltage_id_1/cell_voltage_3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_10_voltage_id_1.cell_voltage_3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_10_voltage_id_1/cell_voltage_4", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_10_voltage_id_1.cell_voltage_4 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_10_voltage_id_2/cell_voltage_5", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_10_voltage_id_2.cell_voltage_5 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_10_voltage_id_2/cell_voltage_6", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_10_voltage_id_2.cell_voltage_6 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_10_voltage_id_2/cell_voltage_7", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_10_voltage_id_2.cell_voltage_7 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_10_voltage_id_2/cell_voltage_8", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_10_voltage_id_2.cell_voltage_8 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_10_voltage_id_3/cell_voltage_10", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_10_voltage_id_3.cell_voltage_10 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_10_voltage_id_3/cell_voltage_11", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_10_voltage_id_3.cell_voltage_11 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_10_voltage_id_3/cell_voltage_12", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_10_voltage_id_3.cell_voltage_12 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_10_voltage_id_3/cell_voltage_9", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_10_voltage_id_3.cell_voltage_9 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_11_msc_id_1/module_voltage_avg", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_11_msc_id_1.module_voltage_avg = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_11_msc_id_1/module_voltage_max", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_11_msc_id_1.module_voltage_max = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_11_msc_id_1/module_voltage_min", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_11_msc_id_1.module_voltage_min = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_11_msc_id_1/module_voltage_sum", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_11_msc_id_1.module_voltage_sum = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_11_msc_id_2/module_ic_temperature", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_11_msc_id_2.module_ic_temperature = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_11_msc_id_2/module_ic_voltage", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_11_msc_id_2.module_ic_voltage = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_11_msc_id_2/module_open_wire", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_11_msc_id_2.module_open_wire = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_11_msc_id_2/module_voltage_delta", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_11_msc_id_2.module_voltage_delta = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_11_temperature_id_1/temperature_value_1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_11_temperature_id_1.temperature_value_1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_11_temperature_id_1/temperature_value_2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_11_temperature_id_1.temperature_value_2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_11_temperature_id_1/temperature_value_3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_11_temperature_id_1.temperature_value_3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_11_temperature_id_1/temperature_value_4", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_11_temperature_id_1.temperature_value_4 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_11_temperature_id_2/temperature_delta", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_11_temperature_id_2.temperature_delta = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_11_temperature_id_2/temperature_maximum", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_11_temperature_id_2.temperature_maximum = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_11_temperature_id_2/temperature_value_5", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_11_temperature_id_2.temperature_value_5 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_11_temperature_id_2/temperature_value_6", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_11_temperature_id_2.temperature_value_6 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_11_voltage_id_1/cell_voltage_1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_11_voltage_id_1.cell_voltage_1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_11_voltage_id_1/cell_voltage_2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_11_voltage_id_1.cell_voltage_2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_11_voltage_id_1/cell_voltage_3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_11_voltage_id_1.cell_voltage_3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_11_voltage_id_1/cell_voltage_4", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_11_voltage_id_1.cell_voltage_4 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_11_voltage_id_2/cell_voltage_5", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_11_voltage_id_2.cell_voltage_5 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_11_voltage_id_2/cell_voltage_6", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_11_voltage_id_2.cell_voltage_6 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_11_voltage_id_2/cell_voltage_7", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_11_voltage_id_2.cell_voltage_7 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_11_voltage_id_2/cell_voltage_8", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_11_voltage_id_2.cell_voltage_8 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_11_voltage_id_3/cell_voltage_10", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_11_voltage_id_3.cell_voltage_10 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_11_voltage_id_3/cell_voltage_11", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_11_voltage_id_3.cell_voltage_11 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_11_voltage_id_3/cell_voltage_12", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_11_voltage_id_3.cell_voltage_12 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_11_voltage_id_3/cell_voltage_9", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_11_voltage_id_3.cell_voltage_9 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_12_msc_id_1/module_voltage_avg", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_12_msc_id_1.module_voltage_avg = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_12_msc_id_1/module_voltage_max", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_12_msc_id_1.module_voltage_max = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_12_msc_id_1/module_voltage_min", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_12_msc_id_1.module_voltage_min = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_12_msc_id_1/module_voltage_sum", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_12_msc_id_1.module_voltage_sum = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_12_msc_id_2/module_ic_temperature", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_12_msc_id_2.module_ic_temperature = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_12_msc_id_2/module_ic_voltage", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_12_msc_id_2.module_ic_voltage = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_12_msc_id_2/module_open_wire", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_12_msc_id_2.module_open_wire = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_12_msc_id_2/module_voltage_delta", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_12_msc_id_2.module_voltage_delta = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_12_temperature_id_1/temperature_value_1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_12_temperature_id_1.temperature_value_1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_12_temperature_id_1/temperature_value_2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_12_temperature_id_1.temperature_value_2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_12_temperature_id_1/temperature_value_3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_12_temperature_id_1.temperature_value_3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_12_temperature_id_1/temperature_value_4", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_12_temperature_id_1.temperature_value_4 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_12_temperature_id_2/temperature_delta", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_12_temperature_id_2.temperature_delta = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_12_temperature_id_2/temperature_maximum", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_12_temperature_id_2.temperature_maximum = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_12_temperature_id_2/temperature_value_5", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_12_temperature_id_2.temperature_value_5 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_12_temperature_id_2/temperature_value_6", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_12_temperature_id_2.temperature_value_6 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_12_voltage_id_1/cell_voltage_1", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_12_voltage_id_1.cell_voltage_1 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_12_voltage_id_1/cell_voltage_2", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_12_voltage_id_1.cell_voltage_2 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_12_voltage_id_1/cell_voltage_3", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_12_voltage_id_1.cell_voltage_3 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_12_voltage_id_1/cell_voltage_4", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_12_voltage_id_1.cell_voltage_4 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_12_voltage_id_2/cell_voltage_5", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_12_voltage_id_2.cell_voltage_5 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_12_voltage_id_2/cell_voltage_6", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_12_voltage_id_2.cell_voltage_6 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_12_voltage_id_2/cell_voltage_7", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_12_voltage_id_2.cell_voltage_7 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_12_voltage_id_2/cell_voltage_8", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_12_voltage_id_2.cell_voltage_8 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_12_voltage_id_3/cell_voltage_10", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_12_voltage_id_3.cell_voltage_10 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_12_voltage_id_3/cell_voltage_11", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_12_voltage_id_3.cell_voltage_11 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_12_voltage_id_3/cell_voltage_12", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_12_voltage_id_3.cell_voltage_12 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/slave_12_voltage_id_3/cell_voltage_9", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_12_voltage_id_3.cell_voltage_9 = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/start_balancing/balancing_request", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.start_balancing.balancing_request = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/start_charging/charging_request", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.start_charging.charging_request = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/start_precharge/precharge_request", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.start_precharge.precharge_request = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/start_programmer/programming_request", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.start_programmer.programming_request = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/vcu_hv/brake_pressure_front", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.vcu_hv.brake_pressure_front = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/vcu_hv/brake_pressure_rear", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.vcu_hv.brake_pressure_rear = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/vcu_hv/hv", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.vcu_hv.hv = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/vcu_ign_r2d/ignition_auto", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.vcu_ign_r2d.ignition_auto = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/vcu_ign_r2d/ignition_manual", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.vcu_ign_r2d.ignition_manual = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/vcu_ign_r2d/ignition_switch_raw", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.vcu_ign_r2d.ignition_switch_raw = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/vcu_ign_r2d/r2d_auto", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.vcu_ign_r2d.r2d_auto = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/vcu_ign_r2d/r2d_button_raw", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.vcu_ign_r2d.r2d_button_raw = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/vcu_ign_r2d/r2d_manual", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.vcu_ign_r2d.r2d_manual = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/vcu_ign_r2d/shutdown_signal", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.vcu_ign_r2d.shutdown_signal = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/vcu_ign_r2d/vcu_state", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.vcu_ign_r2d.vcu_state = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/vcu_rpm/motor_current_left", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.vcu_rpm.motor_current_left = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/vcu_rpm/motor_current_right", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.vcu_rpm.motor_current_right = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/vcu_rpm/motor_rpm_left", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.vcu_rpm.motor_rpm_left = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/vcu_rpm/motor_rpm_right", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.vcu_rpm.motor_rpm_right = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/vcu_rpm_target/rpm_target", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.vcu_rpm_target.rpm_target = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/vcu_states/vcu_state", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.vcu_states.vcu_state = msg->data;
            }
        }));
    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(
        "/can/dbc/vcu_torque_target/torque_target", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.vcu_torque_target.torque_target = msg->data;
            }
        }));
}
#endif

// Combined from power_api.cpp and speed_api.cpp
#include "ui.h"
#include "vars.h"
#include "eez-flow.h"
#include "ros2subscriber.h"
#include <cstdint>

extern "C" void ui_set_hv(float hv_value) {
    (void)hv_value;
}

extern "C" void ui_set_lv(float lv_value) {
    eez::flow::setGlobalVariable(
        FLOW_GLOBAL_VARIABLE_LV,
        eez::FloatValue(lv_value)
    );
}

extern "C" void ui_set_speed(float speed_kph) {
    (void)speed_kph;
    eez::flow::setGlobalVariable(
        FLOW_GLOBAL_VARIABLE_SPEED,
        eez::FloatValue(speed_kph)
    );
}

extern "C" void ui_update_telemetry_vars(const void *t_ptr) {
    if (t_ptr) {
        const TelemetryData *t = static_cast<const TelemetryData *>(t_ptr);
        dbc_api.asf_signals.brake_pressure_front = t->brk_press_f;
        dbc_api.asf_signals.brake_pressure_rear = t->brk_press_r;
        dbc_api.vcu_hv.brake_pressure_front = t->vcu_brkf;
        dbc_api.vcu_hv.brake_pressure_rear = t->vcu_brkr;
        dbc_api.hv500_misc.actual_brake = t->inv_brake;
        dbc_api.pedal_box.apps1 = t->apps1;
        dbc_api.pedal_box.apps2 = t->apps2;
        dbc_api.hv500_misc.actual_throttle = t->inv_throttle;
        dbc_api.master_soc_accumulator.soc_float = t->ams_soc;
        dbc_api.ivt_msg_result_u3.ivt_result_u3 = t->ivt_u3;
        dbc_api.master_msc_id_1.mcu_vref = t->ams_mcu_vref;
        dbc_api.vcu_ign_r2d.r2d_manual = t->vcu_r2d_man;
        dbc_api.vcu_ign_r2d.r2d_auto = t->vcu_r2d_auto;
        dbc_api.rear_wheel_l.shutdown_circuit = t->rear_r2d;
        dbc_api.acu.acu_state = t->acu_state;
        dbc_api.dv_dynamics_1.speed_actual = t->dv_spd_act;
        dbc_api.hv500_temperatures.actual_tempcontroller = t->inv_temp_ctrl;
        dbc_api.hv500_temperatures.actual_tempmotor = t->inv_temp_mot;
        dbc_api.slam_stats_can.lap_counter = t->slam_laps;
        dbc_api.acu.mission_select = t->acu_mission;
    }

    // 1. BRAKE PEDAL PRESSURE (0 to 100)
    float max_brk = dbc_api.asf_signals.brake_pressure_front;
    if (dbc_api.asf_signals.brake_pressure_rear > max_brk) max_brk = dbc_api.asf_signals.brake_pressure_rear;
    if (dbc_api.vcu_hv.brake_pressure_front > max_brk) max_brk = dbc_api.vcu_hv.brake_pressure_front;
    if (dbc_api.vcu_hv.brake_pressure_rear > max_brk) max_brk = dbc_api.vcu_hv.brake_pressure_rear;
    if (dbc_api.hv500_misc.actual_brake > max_brk) max_brk = dbc_api.hv500_misc.actual_brake;
    int brake_val = static_cast<int>(max_brk);
    if (brake_val < 0) brake_val = 0;
    if (brake_val > 100) brake_val = 100;
    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_BRAKE_PEDAL_PRESSURE, eez::IntegerValue(brake_val));

    // 2. ACCELL PEDAL PRESSURE (0 to 100)
    float apps1_val = (dbc_api.apps_adc_raw.apps1_raw / 4095.0f) * 100.0f;
    float apps2_val = (dbc_api.apps_adc_raw.apps2_raw / 4095.0f) * 100.0f;
    float max_acc = apps1_val;
    if (apps2_val > max_acc) max_acc = apps2_val;
    if (dbc_api.pedal_box.apps1 > max_acc) max_acc = dbc_api.pedal_box.apps1;
    if (dbc_api.pedal_box.apps2 > max_acc) max_acc = dbc_api.pedal_box.apps2;
    if (dbc_api.hv500_misc.actual_throttle > max_acc) max_acc = dbc_api.hv500_misc.actual_throttle;
    int acc_val = static_cast<int>(max_acc);
    if (acc_val < 0) acc_val = 0;
    if (acc_val > 100) acc_val = 100;
    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_ACCELL_PEDAL_PRESSURE, eez::IntegerValue(acc_val));

    // 3. SOC (State of Charge, 0 to 100)
    int soc_val = static_cast<int>(dbc_api.master_soc_accumulator.soc_float);
    if (soc_val < 0) soc_val = 0;
    if (soc_val > 100) soc_val = 100;
    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_SOC, eez::IntegerValue(soc_val));

    // 4. LV (Low Voltage, Volts)
    float lv_val = dbc_api.ivt_msg_result_u3.ivt_result_u3 / 1000.0f;
    if (lv_val < 5.0f || lv_val > 30.0f) {
        if (dbc_api.master_msc_id_1.mcu_vref >= 5.0f && dbc_api.master_msc_id_1.mcu_vref <= 30.0f) {
            lv_val = dbc_api.master_msc_id_1.mcu_vref;
        } else {
            // No valid LV data received — set to bar minimum (20.0 V) so the
            // indicator appears empty rather than partially filled.
            lv_val = 20.0f;
        }
    }
    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_LV, eez::FloatValue(lv_val));

    // 5. READY (String "READY" / "NOT READY")
    bool is_ready = (dbc_api.vcu_ign_r2d.r2d_manual == 1.0f || 
                    dbc_api.vcu_ign_r2d.r2d_auto == 1.0f || 
                    dbc_api.rear_wheel_l.shutdown_circuit == 1.0f || 
                    dbc_api.acu.acu_state == 4.0f || 
                    dbc_api.acu.acu_state == 5.0f);
    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_READY, eez::StringValue(is_ready ? "READY" : "NOT READY"));

    // 6. SPEED (km/h)
    float speed_val = dbc_api.dv_dynamics_1.speed_actual >= 0.0f ? dbc_api.dv_dynamics_1.speed_actual : 0.0f;
    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_SPEED, eez::FloatValue(speed_val));

    // 7. TEMP_INV (Inverter Temp, Celsius)
    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_TEMP_INV, eez::FloatValue(dbc_api.hv500_temperatures.actual_tempcontroller));

    // 8. TEMP_MOTOR (Motor Temp, Celsius)
    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_TEMP_MOTOR, eez::FloatValue(dbc_api.hv500_temperatures.actual_tempmotor));

    // 9. LAP_COUNT (Integer)
    int laps_val = static_cast<int>(dbc_api.slam_stats_can.lap_counter);
    if (laps_val < 0) laps_val = 0;
    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_LAP_COUNT, eez::IntegerValue(laps_val));

    // 10. MISSION (String "ACCEL", "SKIDPAD", etc.)
    int mission_id = 0;
    if (dbc_api.acu.mission_select > 0.0f) {
        mission_id = static_cast<int>(dbc_api.acu.mission_select);
    } else if (dbc_api.jetson.as_mission > 0.0f) {
        mission_id = static_cast<int>(dbc_api.jetson.as_mission);
    } else if (dbc_api.dv_status.ami_state > 0.0f) {
        mission_id = static_cast<int>(dbc_api.dv_status.ami_state);
    }
    const char *mission_str = ui_get_mission_name(mission_id);
    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_MISSION, eez::StringValue(mission_str));
}

extern "C" const char *ui_get_mission_str() {
    const char *str = eez::flow::getGlobalVariable(FLOW_GLOBAL_VARIABLE_MISSION).getString();
    return str ? str : "";
}

extern "C" const char *ui_get_mission_name(int mission_id) {
    switch (mission_id) {
        case 1: return "ACCEL";
        case 2: return "SKIDPAD";
        case 3: return "TRACKDRIVE";
        case 4: return "BRAKE TEST";
        case 5: return "INSPECTION";
        case 6: return "AUTOCROSS";
        default: return "MANUAL";
    }
}

extern "C" void check_dbc_errors(void (*on_error)(const char *id, const char *msg_name, const char *sig_name, float value, const char *choice_label)) {
    if (!on_error) return;

    {
        float val = dbc_api.acu.emergency;
        int val_int = static_cast<int>(val);
        const char *label = nullptr;
        if (val_int == 0) label = "OFF";
        if (val_int == 1) label = "ON";
        if (label) {
            bool is_safe = false;
            std::string l_lower = label;
            for (auto &c : l_lower) c = std::tolower(c);
            if (l_lower == "none" || l_lower == "empty" || l_lower == "no fault" ||
                l_lower == "ok" || l_lower == "normal" || l_lower == "off" ||
                l_lower == "deactivated" || l_lower == "no open wire" || l_lower == "false") {
                is_safe = true;
            }
            if (!is_safe) {
                on_error("acu.emergency", "ACU", "EMERGENCY", val, label);
            }
        }
    }
    {
        float val = dbc_api.acu.emergency_cause;
        int val_int = static_cast<int>(val);
        const char *label = nullptr;
        if (val_int == 0) label = "None";
        if (val_int == 1) label = "SDC_OPEN";
        if (val_int == 2) label = "RES";
        if (val_int == 3) label = "Pressure_checks";
        if (val_int == 4) label = "VCU_timeout";
        if (val_int == 5) label = "Jetson_timeout";
        if (val_int == 6) label = "ACU_WDT_TRIGGERED";
        if (val_int == 7) label = "dir_actuator_timeout";
        if (val_int == 8) label = "CAN_Dynamics_Pressure_timeout";
        if (label) {
            bool is_safe = false;
            std::string l_lower = label;
            for (auto &c : l_lower) c = std::tolower(c);
            if (l_lower == "none" || l_lower == "empty" || l_lower == "no fault" ||
                l_lower == "ok" || l_lower == "normal" || l_lower == "off" ||
                l_lower == "deactivated" || l_lower == "no open wire" || l_lower == "false") {
                is_safe = true;
            }
            if (!is_safe) {
                on_error("acu.emergency_cause", "ACU", "EMERGENCY_cause", val, label);
            }
        }
    }
    {
        float val = dbc_api.cubemars_feedback.error_code;
        int val_int = static_cast<int>(val);
        const char *label = nullptr;
        if (val_int == 0) label = "None";
        if (val_int == 1) label = "OVER-VOLTAGE";
        if (val_int == 2) label = "UNDER-VOLTAGE";
        if (val_int == 3) label = "DRIVER-FAULT";
        if (val_int == 4) label = "OVER-CURRENT";
        if (val_int == 5) label = "MOSFET-OVER-TEMP";
        if (val_int == 6) label = "MOTOR-OVER-TEMP";
        if (val_int == 7) label = "GATE-DRIVE-OVER-VOLTAGE";
        if (val_int == 8) label = "GATE-DRIVE-UNDER-VOLTAGE";
        if (val_int == 9) label = "MCU-UNDER-VOLTAGE";
        if (val_int == 10) label = "REBOOTING-FROM-WATCHDOG";
        if (val_int == 11) label = "ENCODER-FAULT";
        if (val_int == 12) label = "MIN-ENCODER-LIMIT-EXCEEDED";
        if (val_int == 13) label = "MAX-ENCODER-LIMIT-EXCEEDED";
        if (val_int == 14) label = "FLASH-FAULT";
        if (val_int == 15) label = "HIGH-OFFSET-CURRENT-SENSOR1";
        if (val_int == 16) label = "HIGH-OFFSET-CURRENT-SENSOR2";
        if (label) {
            bool is_safe = false;
            std::string l_lower = label;
            for (auto &c : l_lower) c = std::tolower(c);
            if (l_lower == "none" || l_lower == "empty" || l_lower == "no fault" ||
                l_lower == "ok" || l_lower == "normal" || l_lower == "off" ||
                l_lower == "deactivated" || l_lower == "no open wire" || l_lower == "false") {
                is_safe = true;
            }
            if (!is_safe) {
                on_error("cubemars_feedback.error_code", "CubeMars_Feedback", "Error_Code", val, label);
            }
        }
    }
    {
        float val = dbc_api.aqt4.emergency;
        if (val > 0.5f) {
            on_error("aqt4.emergency", "AQT4", "EMERGENCY", val, "ERROR");
        }
    }
    {
        float val = dbc_api.jetson.emergency_cause;
        int val_int = static_cast<int>(val);
        const char *label = nullptr;
        if (val_int == 0) label = "None";
        if (val_int == 1) label = "RES";
        if (val_int == 2) label = "ACU";
        if (val_int == 3) label = "ZED";
        if (val_int == 4) label = "Steering_error";
        if (val_int == 5) label = "Steering_timeout";
        if (val_int == 6) label = "Planner";
        if (val_int == 7) label = "SLAM";
        if (val_int == 8) label = "P-Puma";
        if (val_int == 9) label = "IMU";
        if (label) {
            bool is_safe = false;
            std::string l_lower = label;
            for (auto &c : l_lower) c = std::tolower(c);
            if (l_lower == "none" || l_lower == "empty" || l_lower == "no fault" ||
                l_lower == "ok" || l_lower == "normal" || l_lower == "off" ||
                l_lower == "deactivated" || l_lower == "no open wire" || l_lower == "false") {
                is_safe = true;
            }
            if (!is_safe) {
                on_error("jetson.emergency_cause", "JETSON", "EMERGENCY_CAUSE", val, label);
            }
        }
    }
    {
        float val = dbc_api.master_msc_id_1.adbms_pec_error;
        if (val > 0.5f) {
            on_error("master_msc_id_1.adbms_pec_error", "Master_MSC_ID_1", "adbms_pec_error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.master_msc_id_1.fault_counter;
        if (val > 0.5f) {
            on_error("master_msc_id_1.fault_counter", "Master_MSC_ID_1", "fault_counter", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_wh.ivt_result_wh_system_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_wh.ivt_result_wh_system_error", "IVT_Msg_Result_Wh", "IVT_Result_Wh_System_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_wh.ivt_result_wh_measurement_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_wh.ivt_result_wh_measurement_error", "IVT_Msg_Result_Wh", "IVT_Result_Wh_Measurement_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_wh.ivt_result_wh_channel_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_wh.ivt_result_wh_channel_error", "IVT_Msg_Result_Wh", "IVT_Result_Wh_Channel_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_as.ivt_result_as_system_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_as.ivt_result_as_system_error", "IVT_Msg_Result_As", "IVT_Result_As_System_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_as.ivt_result_as_measurement_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_as.ivt_result_as_measurement_error", "IVT_Msg_Result_As", "IVT_Result_As_Measurement_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_as.ivt_result_as_channel_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_as.ivt_result_as_channel_error", "IVT_Msg_Result_As", "IVT_Result_As_Channel_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_w.ivt_result_w_system_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_w.ivt_result_w_system_error", "IVT_Msg_Result_W", "IVT_Result_W_System_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_w.ivt_result_w_measurement_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_w.ivt_result_w_measurement_error", "IVT_Msg_Result_W", "IVT_Result_W_Measurement_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_w.ivt_result_w_channel_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_w.ivt_result_w_channel_error", "IVT_Msg_Result_W", "IVT_Result_W_Channel_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_response.val_80_resp_measerror_item;
        if (val > 0.5f) {
            on_error("ivt_msg_response.val_80_resp_measerror_item", "IVT_Msg_Response", "_80_Resp_MeasError_Item", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_response.val_81_resp_systemerror_item;
        if (val > 0.5f) {
            on_error("ivt_msg_response.val_81_resp_systemerror_item", "IVT_Msg_Response", "_81_Resp_SystemError_Item", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_response.val_80_resp_measerror_count_mask;
        if (val > 0.5f) {
            on_error("ivt_msg_response.val_80_resp_measerror_count_mask", "IVT_Msg_Response", "_80_Resp_MeasError_Count_Mask", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_response.val_81_resp_systemerror_count_mask;
        if (val > 0.5f) {
            on_error("ivt_msg_response.val_81_resp_systemerror_count_mask", "IVT_Msg_Response", "_81_Resp_SystemError_Count_Mask", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_t.ivt_result_t_system_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_t.ivt_result_t_system_error", "IVT_Msg_Result_T", "IVT_Result_T_System_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_t.ivt_result_t_measurement_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_t.ivt_result_t_measurement_error", "IVT_Msg_Result_T", "IVT_Result_T_Measurement_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_t.ivt_result_t_channel_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_t.ivt_result_t_channel_error", "IVT_Msg_Result_T", "IVT_Result_T_Channel_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_u3.ivt_result_u3_system_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_u3.ivt_result_u3_system_error", "IVT_Msg_Result_U3", "IVT_Result_U3_System_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_u3.ivt_result_u3_measurement_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_u3.ivt_result_u3_measurement_error", "IVT_Msg_Result_U3", "IVT_Result_U3_Measurement_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_u3.ivt_result_u3_channel_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_u3.ivt_result_u3_channel_error", "IVT_Msg_Result_U3", "IVT_Result_U3_Channel_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_u2.ivt_result_u2_system_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_u2.ivt_result_u2_system_error", "IVT_Msg_Result_U2", "IVT_Result_U2_System_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_u2.ivt_result_u2_measurement_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_u2.ivt_result_u2_measurement_error", "IVT_Msg_Result_U2", "IVT_Result_U2_Measurement_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_u2.ivt_result_u2_channel_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_u2.ivt_result_u2_channel_error", "IVT_Msg_Result_U2", "IVT_Result_U2_Channel_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_u1.ivt_result_u1_system_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_u1.ivt_result_u1_system_error", "IVT_Msg_Result_U1", "IVT_Result_U1_System_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_u1.ivt_result_u1_measurement_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_u1.ivt_result_u1_measurement_error", "IVT_Msg_Result_U1", "IVT_Result_U1_Measurement_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_u1.ivt_result_u1_channel_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_u1.ivt_result_u1_channel_error", "IVT_Msg_Result_U1", "IVT_Result_U1_Channel_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_i.ivt_result_i_system_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_i.ivt_result_i_system_error", "IVT_Msg_Result_I", "IVT_Result_I_System_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_i.ivt_result_i_measurement_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_i.ivt_result_i_measurement_error", "IVT_Msg_Result_I", "IVT_Result_I_Measurement_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_i.ivt_result_i_channel_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_i.ivt_result_i_channel_error", "IVT_Msg_Result_I", "IVT_Result_I_Channel_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_cmd.val_3d_restart_default_dummy;
        if (val > 0.5f) {
            on_error("ivt_msg_cmd.val_3d_restart_default_dummy", "IVT_Msg_Cmd", "_3D_Restart_Default_Dummy", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_cmd.val_40_get_measerror_item;
        if (val > 0.5f) {
            on_error("ivt_msg_cmd.val_40_get_measerror_item", "IVT_Msg_Cmd", "_40_Get_MeasError_Item", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_cmd.val_41_get_systemerror_item;
        if (val > 0.5f) {
            on_error("ivt_msg_cmd.val_41_get_systemerror_item", "IVT_Msg_Cmd", "_41_Get_SystemError_Item", val, "ERROR");
        }
    }
    {
        float val = dbc_api.hv500_temperatures.actual_faultcode;
        if (val > 0.5f) {
            on_error("hv500_temperatures.actual_faultcode", "HV500_Temperatures", "Actual_FaultCode", val, "ERROR");
        }
    }
    {
        float val = dbc_api.master_msc_id_2.fault1_code;
        int val_int = static_cast<int>(val);
        const char *label = nullptr;
        if (val_int == 0) label = "FAULT_OVERVOLTAGE";
        if (val_int == 1) label = "FAULT_UNDERVOLTAGE";
        if (val_int == 2) label = "FAULT_OVERTEMPERATURE";
        if (val_int == 3) label = "FAULT_UNDERTEMPERATURE";
        if (val_int == 4) label = "FAULT_OVERTEMPERATURE_DISCHARGE";
        if (val_int == 5) label = "FAULT_UNDERTEMPERATURE_CHARGE";
        if (val_int == 6) label = "FAULT_TEMP_SENSOR_OPEN";
        if (val_int == 7) label = "FAULT_SLAVE_NOT_DETECTED";
        if (val_int == 8) label = "FAULT_PEC_ERROR";
        if (val_int == 9) label = "FAULT_OPEN_WIRE";
        if (val_int == 10) label = "FAULT_ACQUISITION_TIMEOUT";
        if (val_int == 11) label = "FAULT_CAN_SEND_ERROR";
        if (val_int == 12) label = "FAULT_CAN_INIT_ERROR";
        if (val_int == 13) label = "FAULT_CAN_MAILBOX_FULL";
        if (val_int == 14) label = "FAULT_CAN_BUS_OFF";
        if (val_int == 15) label = "FAULT_CAN_RECEIVE_ERROR";
        if (val_int == 16) label = "FAULT_INVERTER_TIMEOUT";
        if (val_int == 17) label = "FAULT_VCU_TIMEOUT";
        if (val_int == 18) label = "FAULT_PDM_TIMEOUT";
        if (val_int == 19) label = "FAULT_CHARGER_TIMEOUT";
        if (val_int == 20) label = "FAULT_ACQUISITION_NODE_TIMEOUT";
        if (val_int == 21) label = "FAULT_CONTACTOR_MISMATCH";
        if (val_int == 22) label = "FAULT_PRECHARGE_TIMEOUT";
        if (val_int == 23) label = "FAULT_PRECHARGE_FAILURE";
        if (val_int == 24) label = "FAULT_SDC_TRIGGERED";
        if (val_int == 25) label = "FAULT_BALANCING_ERROR";
        if (val_int == 26) label = "FAULT_BALANCING_OVERTEMP";
        if (val_int == 27) label = "FAULT_ADC_ERROR";
        if (val_int == 28) label = "FAULT_CURRENT_SENSOR_ERROR";
        if (val_int == 29) label = "FAULT_SOC_CRITICAL_LOW";
        if (val_int == 30) label = "FAULT_PACK_VOLTAGE_MISMATCH";
        if (val_int == 31) label = "FAULT_EEPROM_READ_ERROR";
        if (val_int == 32) label = "FAULT_EEPROM_WRITE_ERROR";
        if (val_int == 33) label = "FAULT_EEPROM_VALIDATION_ERROR";
        if (val_int == 34) label = "FAULT_STARTUP_FAILURE";
        if (val_int == 35) label = "FAULT_WATCHDOG_RESET";
        if (val_int == 36) label = "FAULT_STACK_OVERFLOW";
        if (val_int == 37) label = "FAULT_UART1_TX";
        if (val_int == 38) label = "FAULT_UART1_RX";
        if (val_int == 39) label = "FAULT_UART2_TX";
        if (val_int == 40) label = "FAULT_UART2_RX";
        if (val_int == 41) label = "FAULT_OW_DETECTED_CELL";
        if (val_int == 42) label = "FAULT_OW_DETECTED_RTH";
        if (val_int == 255) label = "EMPTY";
        if (label) {
            bool is_safe = false;
            std::string l_lower = label;
            for (auto &c : l_lower) c = std::tolower(c);
            if (l_lower == "none" || l_lower == "empty" || l_lower == "no fault" ||
                l_lower == "ok" || l_lower == "normal" || l_lower == "off" ||
                l_lower == "deactivated" || l_lower == "no open wire" || l_lower == "false") {
                is_safe = true;
            }
            if (!is_safe) {
                on_error("master_msc_id_2.fault1_code", "Master_MSC_ID_2", "fault1_code", val, label);
            }
        }
    }
    {
        float val = dbc_api.master_msc_id_2.fault1_index_type;
        int val_int = static_cast<int>(val);
        const char *label = nullptr;
        if (val_int == 0) label = "SLAVE";
        if (val_int == 1) label = "CELL";
        if (val_int == 2) label = "CHANNEL";
        if (val_int == 3) label = "CONTACTORS";
        if (val_int == 4) label = "SLAVE_AND_CELL";
        if (val_int == 5) label = "THERMISTOR";
        if (val_int == 6) label = "SLAVE_AND_THERMISTOR";
        if (val_int == 7) label = "NONE";
        if (label) {
            bool is_safe = false;
            std::string l_lower = label;
            for (auto &c : l_lower) c = std::tolower(c);
            if (l_lower == "none" || l_lower == "empty" || l_lower == "no fault" ||
                l_lower == "ok" || l_lower == "normal" || l_lower == "off" ||
                l_lower == "deactivated" || l_lower == "no open wire" || l_lower == "false") {
                is_safe = true;
            }
            if (!is_safe) {
                on_error("master_msc_id_2.fault1_index_type", "Master_MSC_ID_2", "fault1_index_type", val, label);
            }
        }
    }
    {
        float val = dbc_api.master_msc_id_2.fault1_index_value;
        if (val > 0.5f) {
            on_error("master_msc_id_2.fault1_index_value", "Master_MSC_ID_2", "fault1_index_value", val, "ERROR");
        }
    }
    {
        float val = dbc_api.master_msc_id_2.fault2_code;
        int val_int = static_cast<int>(val);
        const char *label = nullptr;
        if (val_int == 0) label = "FAULT_OVERVOLTAGE";
        if (val_int == 1) label = "FAULT_UNDERVOLTAGE";
        if (val_int == 2) label = "FAULT_OVERTEMPERATURE";
        if (val_int == 3) label = "FAULT_UNDERTEMPERATURE";
        if (val_int == 4) label = "FAULT_OVERTEMPERATURE_DISCHARGE";
        if (val_int == 5) label = "FAULT_UNDERTEMPERATURE_CHARGE";
        if (val_int == 6) label = "FAULT_TEMP_SENSOR_OPEN";
        if (val_int == 7) label = "FAULT_SLAVE_NOT_DETECTED";
        if (val_int == 8) label = "FAULT_PEC_ERROR";
        if (val_int == 9) label = "FAULT_OPEN_WIRE";
        if (val_int == 10) label = "FAULT_ACQUISITION_TIMEOUT";
        if (val_int == 11) label = "FAULT_CAN_SEND_ERROR";
        if (val_int == 12) label = "FAULT_CAN_INIT_ERROR";
        if (val_int == 13) label = "FAULT_CAN_MAILBOX_FULL";
        if (val_int == 14) label = "FAULT_CAN_BUS_OFF";
        if (val_int == 15) label = "FAULT_CAN_RECEIVE_ERROR";
        if (val_int == 16) label = "FAULT_INVERTER_TIMEOUT";
        if (val_int == 17) label = "FAULT_VCU_TIMEOUT";
        if (val_int == 18) label = "FAULT_PDM_TIMEOUT";
        if (val_int == 19) label = "FAULT_CHARGER_TIMEOUT";
        if (val_int == 20) label = "FAULT_ACQUISITION_NODE_TIMEOUT";
        if (val_int == 21) label = "FAULT_CONTACTOR_MISMATCH";
        if (val_int == 22) label = "FAULT_PRECHARGE_TIMEOUT";
        if (val_int == 23) label = "FAULT_PRECHARGE_FAILURE";
        if (val_int == 24) label = "FAULT_SDC_TRIGGERED";
        if (val_int == 25) label = "FAULT_BALANCING_ERROR";
        if (val_int == 26) label = "FAULT_BALANCING_OVERTEMP";
        if (val_int == 27) label = "FAULT_ADC_ERROR";
        if (val_int == 28) label = "FAULT_CURRENT_SENSOR_ERROR";
        if (val_int == 29) label = "FAULT_SOC_CRITICAL_LOW";
        if (val_int == 30) label = "FAULT_PACK_VOLTAGE_MISMATCH";
        if (val_int == 31) label = "FAULT_EEPROM_READ_ERROR";
        if (val_int == 32) label = "FAULT_EEPROM_WRITE_ERROR";
        if (val_int == 33) label = "FAULT_EEPROM_VALIDATION_ERROR";
        if (val_int == 34) label = "FAULT_STARTUP_FAILURE";
        if (val_int == 35) label = "FAULT_WATCHDOG_RESET";
        if (val_int == 36) label = "FAULT_STACK_OVERFLOW";
        if (val_int == 37) label = "FAULT_UART1_TX";
        if (val_int == 38) label = "FAULT_UART1_RX";
        if (val_int == 39) label = "FAULT_UART2_TX";
        if (val_int == 40) label = "FAULT_UART2_RX";
        if (val_int == 41) label = "FAULT_OW_DETECTED_CELL";
        if (val_int == 42) label = "FAULT_OW_DETECTED_RTH";
        if (val_int == 255) label = "EMPTY";
        if (label) {
            bool is_safe = false;
            std::string l_lower = label;
            for (auto &c : l_lower) c = std::tolower(c);
            if (l_lower == "none" || l_lower == "empty" || l_lower == "no fault" ||
                l_lower == "ok" || l_lower == "normal" || l_lower == "off" ||
                l_lower == "deactivated" || l_lower == "no open wire" || l_lower == "false") {
                is_safe = true;
            }
            if (!is_safe) {
                on_error("master_msc_id_2.fault2_code", "Master_MSC_ID_2", "fault2_code", val, label);
            }
        }
    }
    {
        float val = dbc_api.master_msc_id_2.fault2_index_type;
        int val_int = static_cast<int>(val);
        const char *label = nullptr;
        if (val_int == 0) label = "SLAVE";
        if (val_int == 1) label = "CELL";
        if (val_int == 2) label = "CHANNEL";
        if (val_int == 3) label = "CONTACTORS";
        if (val_int == 4) label = "SLAVE_AND_CELL";
        if (val_int == 5) label = "THERMISTOR";
        if (val_int == 6) label = "SLAVE_AND_THERMISTOR";
        if (val_int == 7) label = "NONE";
        if (label) {
            bool is_safe = false;
            std::string l_lower = label;
            for (auto &c : l_lower) c = std::tolower(c);
            if (l_lower == "none" || l_lower == "empty" || l_lower == "no fault" ||
                l_lower == "ok" || l_lower == "normal" || l_lower == "off" ||
                l_lower == "deactivated" || l_lower == "no open wire" || l_lower == "false") {
                is_safe = true;
            }
            if (!is_safe) {
                on_error("master_msc_id_2.fault2_index_type", "Master_MSC_ID_2", "fault2_index_type", val, label);
            }
        }
    }
    {
        float val = dbc_api.master_msc_id_2.fault2_index_value;
        if (val > 0.5f) {
            on_error("master_msc_id_2.fault2_index_value", "Master_MSC_ID_2", "fault2_index_value", val, "ERROR");
        }
    }
}
