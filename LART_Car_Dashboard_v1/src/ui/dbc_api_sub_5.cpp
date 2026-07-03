// Auto-generated from DBC files by generate_dbc_api.py. Do not edit.
#include "dbc_api.h"
#if defined(LART_UI_HAVE_RCLCPP) && LART_UI_HAVE_RCLCPP
#if defined(LART_HAVE_LART_MSGS) && LART_HAVE_LART_MSGS
#include <rclcpp/rclcpp.hpp>
#include <mutex>
#include <vector>
extern std::mutex dbc_api_mutex;
#include <lart_msgs/msg/slave11_temperature_id1.hpp>
#include <lart_msgs/msg/slave11_temperature_id2.hpp>
#include <lart_msgs/msg/slave11_voltage_id1.hpp>
#include <lart_msgs/msg/slave11_voltage_id2.hpp>
#include <lart_msgs/msg/slave11_voltage_id3.hpp>
#include <lart_msgs/msg/slave12_msc_id1.hpp>
#include <lart_msgs/msg/slave12_msc_id2.hpp>
#include <lart_msgs/msg/slave12_temperature_id1.hpp>
#include <lart_msgs/msg/slave12_temperature_id2.hpp>
#include <lart_msgs/msg/slave12_voltage_id1.hpp>
#include <lart_msgs/msg/slave12_voltage_id2.hpp>
#include <lart_msgs/msg/slave12_voltage_id3.hpp>
#include <lart_msgs/msg/start_balancing.hpp>
#include <lart_msgs/msg/start_charging.hpp>
#include <lart_msgs/msg/start_precharge.hpp>
#include <lart_msgs/msg/start_programmer.hpp>
#include <lart_msgs/msg/vcu_hv.hpp>
#include <lart_msgs/msg/vcu_ign_r2d.hpp>
#include <lart_msgs/msg/vcu_rpm.hpp>
#include <lart_msgs/msg/vcu_rpm_target.hpp>
#include <lart_msgs/msg/vcu_states.hpp>
#include <lart_msgs/msg/vcu_torque_target.hpp>

void init_dbc_api_subscribers_chunk_5(std::shared_ptr<rclcpp::Node> node, std::vector<rclcpp::SubscriptionBase::SharedPtr>& subs) {
    auto sensor_qos = rclcpp::QoS(10).best_effort();

    subs.push_back(node->create_subscription<lart_msgs::msg::Slave11TemperatureId1>(
        "/can/dbc/slave_11_temperature_id_1", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave11TemperatureId1> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_11_temperature_id_1.temperature_value_1 = msg->temperature_value_1;
                dbc_api.slave_11_temperature_id_1.temperature_value_2 = msg->temperature_value_2;
                dbc_api.slave_11_temperature_id_1.temperature_value_3 = msg->temperature_value_3;
                dbc_api.slave_11_temperature_id_1.temperature_value_4 = msg->temperature_value_4;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave11TemperatureId2>(
        "/can/dbc/slave_11_temperature_id_2", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave11TemperatureId2> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_11_temperature_id_2.temperature_delta = msg->temperature_delta;
                dbc_api.slave_11_temperature_id_2.temperature_maximum = msg->temperature_maximum;
                dbc_api.slave_11_temperature_id_2.temperature_value_5 = msg->temperature_value_5;
                dbc_api.slave_11_temperature_id_2.temperature_value_6 = msg->temperature_value_6;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave11VoltageId1>(
        "/can/dbc/slave_11_voltage_id_1", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave11VoltageId1> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_11_voltage_id_1.cell_voltage_1 = msg->cell_voltage_1;
                dbc_api.slave_11_voltage_id_1.cell_voltage_2 = msg->cell_voltage_2;
                dbc_api.slave_11_voltage_id_1.cell_voltage_3 = msg->cell_voltage_3;
                dbc_api.slave_11_voltage_id_1.cell_voltage_4 = msg->cell_voltage_4;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave11VoltageId2>(
        "/can/dbc/slave_11_voltage_id_2", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave11VoltageId2> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_11_voltage_id_2.cell_voltage_5 = msg->cell_voltage_5;
                dbc_api.slave_11_voltage_id_2.cell_voltage_6 = msg->cell_voltage_6;
                dbc_api.slave_11_voltage_id_2.cell_voltage_7 = msg->cell_voltage_7;
                dbc_api.slave_11_voltage_id_2.cell_voltage_8 = msg->cell_voltage_8;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave11VoltageId3>(
        "/can/dbc/slave_11_voltage_id_3", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave11VoltageId3> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_11_voltage_id_3.cell_voltage_10 = msg->cell_voltage_10;
                dbc_api.slave_11_voltage_id_3.cell_voltage_11 = msg->cell_voltage_11;
                dbc_api.slave_11_voltage_id_3.cell_voltage_12 = msg->cell_voltage_12;
                dbc_api.slave_11_voltage_id_3.cell_voltage_9 = msg->cell_voltage_9;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave12MscId1>(
        "/can/dbc/slave_12_msc_id_1", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave12MscId1> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_12_msc_id_1.module_voltage_avg = msg->module_voltage_avg;
                dbc_api.slave_12_msc_id_1.module_voltage_max = msg->module_voltage_max;
                dbc_api.slave_12_msc_id_1.module_voltage_min = msg->module_voltage_min;
                dbc_api.slave_12_msc_id_1.module_voltage_sum = msg->module_voltage_sum;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave12MscId2>(
        "/can/dbc/slave_12_msc_id_2", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave12MscId2> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_12_msc_id_2.module_ic_temperature = msg->module_ic_temperature;
                dbc_api.slave_12_msc_id_2.module_ic_voltage = msg->module_ic_voltage;
                dbc_api.slave_12_msc_id_2.module_open_wire = msg->module_open_wire;
                dbc_api.slave_12_msc_id_2.module_voltage_delta = msg->module_voltage_delta;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave12TemperatureId1>(
        "/can/dbc/slave_12_temperature_id_1", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave12TemperatureId1> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_12_temperature_id_1.temperature_value_1 = msg->temperature_value_1;
                dbc_api.slave_12_temperature_id_1.temperature_value_2 = msg->temperature_value_2;
                dbc_api.slave_12_temperature_id_1.temperature_value_3 = msg->temperature_value_3;
                dbc_api.slave_12_temperature_id_1.temperature_value_4 = msg->temperature_value_4;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave12TemperatureId2>(
        "/can/dbc/slave_12_temperature_id_2", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave12TemperatureId2> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_12_temperature_id_2.temperature_delta = msg->temperature_delta;
                dbc_api.slave_12_temperature_id_2.temperature_maximum = msg->temperature_maximum;
                dbc_api.slave_12_temperature_id_2.temperature_value_5 = msg->temperature_value_5;
                dbc_api.slave_12_temperature_id_2.temperature_value_6 = msg->temperature_value_6;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave12VoltageId1>(
        "/can/dbc/slave_12_voltage_id_1", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave12VoltageId1> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_12_voltage_id_1.cell_voltage_1 = msg->cell_voltage_1;
                dbc_api.slave_12_voltage_id_1.cell_voltage_2 = msg->cell_voltage_2;
                dbc_api.slave_12_voltage_id_1.cell_voltage_3 = msg->cell_voltage_3;
                dbc_api.slave_12_voltage_id_1.cell_voltage_4 = msg->cell_voltage_4;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave12VoltageId2>(
        "/can/dbc/slave_12_voltage_id_2", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave12VoltageId2> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_12_voltage_id_2.cell_voltage_5 = msg->cell_voltage_5;
                dbc_api.slave_12_voltage_id_2.cell_voltage_6 = msg->cell_voltage_6;
                dbc_api.slave_12_voltage_id_2.cell_voltage_7 = msg->cell_voltage_7;
                dbc_api.slave_12_voltage_id_2.cell_voltage_8 = msg->cell_voltage_8;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave12VoltageId3>(
        "/can/dbc/slave_12_voltage_id_3", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave12VoltageId3> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_12_voltage_id_3.cell_voltage_10 = msg->cell_voltage_10;
                dbc_api.slave_12_voltage_id_3.cell_voltage_11 = msg->cell_voltage_11;
                dbc_api.slave_12_voltage_id_3.cell_voltage_12 = msg->cell_voltage_12;
                dbc_api.slave_12_voltage_id_3.cell_voltage_9 = msg->cell_voltage_9;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::StartBalancing>(
        "/can/dbc/start_balancing", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::StartBalancing> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.start_balancing.balancing_request = msg->balancing_request;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::StartCharging>(
        "/can/dbc/start_charging", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::StartCharging> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.start_charging.charging_request = msg->charging_request;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::StartPrecharge>(
        "/can/dbc/start_precharge", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::StartPrecharge> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.start_precharge.precharge_request = msg->precharge_request;
            }
        }));
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
