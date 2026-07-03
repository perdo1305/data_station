// Auto-generated from DBC files by generate_dbc_api.py. Do not edit.
#include "dbc_api.h"
#if defined(LART_UI_HAVE_RCLCPP) && LART_UI_HAVE_RCLCPP
#if defined(LART_HAVE_LART_MSGS) && LART_HAVE_LART_MSGS
#include <rclcpp/rclcpp.hpp>
#include <mutex>
#include <vector>
extern std::mutex dbc_api_mutex;
#include <lart_msgs/msg/ivt_msg_result_i.hpp>
#include <lart_msgs/msg/ivt_msg_result_t.hpp>
#include <lart_msgs/msg/ivt_msg_result_u1.hpp>
#include <lart_msgs/msg/ivt_msg_result_u2.hpp>
#include <lart_msgs/msg/ivt_msg_result_u3.hpp>
#include <lart_msgs/msg/ivt_msg_result_w.hpp>
#include <lart_msgs/msg/ivt_msg_result_wh.hpp>
#include <lart_msgs/msg/jetson.hpp>
#include <lart_msgs/msg/master_msc_id1.hpp>
#include <lart_msgs/msg/master_msc_id2.hpp>
#include <lart_msgs/msg/master_msc_id3.hpp>
#include <lart_msgs/msg/master_msc_id4.hpp>
#include <lart_msgs/msg/master_precharge_id1.hpp>
#include <lart_msgs/msg/master_soc_accumulator.hpp>
#include <lart_msgs/msg/pdm_cooling.hpp>
#include <lart_msgs/msg/pdm_lv.hpp>
#include <lart_msgs/msg/res.hpp>
#include <lart_msgs/msg/slam_stats_can.hpp>
#include <lart_msgs/msg/slave01_msc_id1.hpp>
#include <lart_msgs/msg/slave01_msc_id2.hpp>
#include <lart_msgs/msg/slave01_temperature_id1.hpp>
#include <lart_msgs/msg/slave01_temperature_id2.hpp>
#include <lart_msgs/msg/slave01_voltage_id1.hpp>
#include <lart_msgs/msg/slave01_voltage_id2.hpp>
#include <lart_msgs/msg/slave01_voltage_id3.hpp>
#include <lart_msgs/msg/slave02_msc_id1.hpp>
#include <lart_msgs/msg/slave02_msc_id2.hpp>
#include <lart_msgs/msg/slave02_temperature_id1.hpp>
#include <lart_msgs/msg/slave02_temperature_id2.hpp>
#include <lart_msgs/msg/slave02_voltage_id1.hpp>

void init_dbc_api_subscribers_chunk_2(std::shared_ptr<rclcpp::Node> node, std::vector<rclcpp::SubscriptionBase::SharedPtr>& subs) {
    auto sensor_qos = rclcpp::QoS(10).best_effort();

    subs.push_back(node->create_subscription<lart_msgs::msg::IvtMsgResultI>(
        "/can/dbc/ivt_msg_result_i", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::IvtMsgResultI> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_i.ivt_id_result_i = msg->ivt_id_result_i;
                dbc_api.ivt_msg_result_i.ivt_msgcount_result_i = msg->ivt_msgcount_result_i;
                dbc_api.ivt_msg_result_i.ivt_result_i = msg->ivt_result_i;
                dbc_api.ivt_msg_result_i.ivt_result_i_channel_error = msg->ivt_result_i_channel_error;
                dbc_api.ivt_msg_result_i.ivt_result_i_measurement_error = msg->ivt_result_i_measurement_error;
                dbc_api.ivt_msg_result_i.ivt_result_i_ocs = msg->ivt_result_i_ocs;
                dbc_api.ivt_msg_result_i.ivt_result_i_system_error = msg->ivt_result_i_system_error;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::IvtMsgResultT>(
        "/can/dbc/ivt_msg_result_t", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::IvtMsgResultT> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_t.ivt_id_result_t = msg->ivt_id_result_t;
                dbc_api.ivt_msg_result_t.ivt_msgcount_result_t = msg->ivt_msgcount_result_t;
                dbc_api.ivt_msg_result_t.ivt_result_t = msg->ivt_result_t;
                dbc_api.ivt_msg_result_t.ivt_result_t_channel_error = msg->ivt_result_t_channel_error;
                dbc_api.ivt_msg_result_t.ivt_result_t_measurement_error = msg->ivt_result_t_measurement_error;
                dbc_api.ivt_msg_result_t.ivt_result_t_ocs = msg->ivt_result_t_ocs;
                dbc_api.ivt_msg_result_t.ivt_result_t_system_error = msg->ivt_result_t_system_error;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::IvtMsgResultU1>(
        "/can/dbc/ivt_msg_result_u1", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::IvtMsgResultU1> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_u1.ivt_id_result_u1 = msg->ivt_id_result_u1;
                dbc_api.ivt_msg_result_u1.ivt_msgcount_result_u1 = msg->ivt_msgcount_result_u1;
                dbc_api.ivt_msg_result_u1.ivt_result_u1 = msg->ivt_result_u1;
                dbc_api.ivt_msg_result_u1.ivt_result_u1_channel_error = msg->ivt_result_u1_channel_error;
                dbc_api.ivt_msg_result_u1.ivt_result_u1_measurement_error = msg->ivt_result_u1_measurement_error;
                dbc_api.ivt_msg_result_u1.ivt_result_u1_ocs = msg->ivt_result_u1_ocs;
                dbc_api.ivt_msg_result_u1.ivt_result_u1_system_error = msg->ivt_result_u1_system_error;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::IvtMsgResultU2>(
        "/can/dbc/ivt_msg_result_u2", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::IvtMsgResultU2> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_u2.ivt_id_result_u2 = msg->ivt_id_result_u2;
                dbc_api.ivt_msg_result_u2.ivt_msgcount_result_u2 = msg->ivt_msgcount_result_u2;
                dbc_api.ivt_msg_result_u2.ivt_result_u2 = msg->ivt_result_u2;
                dbc_api.ivt_msg_result_u2.ivt_result_u2_channel_error = msg->ivt_result_u2_channel_error;
                dbc_api.ivt_msg_result_u2.ivt_result_u2_measurement_error = msg->ivt_result_u2_measurement_error;
                dbc_api.ivt_msg_result_u2.ivt_result_u2_ocs = msg->ivt_result_u2_ocs;
                dbc_api.ivt_msg_result_u2.ivt_result_u2_system_error = msg->ivt_result_u2_system_error;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::IvtMsgResultU3>(
        "/can/dbc/ivt_msg_result_u3", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::IvtMsgResultU3> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_u3.ivt_id_result_u3 = msg->ivt_id_result_u3;
                dbc_api.ivt_msg_result_u3.ivt_msgcount_result_u3 = msg->ivt_msgcount_result_u3;
                dbc_api.ivt_msg_result_u3.ivt_result_u3 = msg->ivt_result_u3;
                dbc_api.ivt_msg_result_u3.ivt_result_u3_channel_error = msg->ivt_result_u3_channel_error;
                dbc_api.ivt_msg_result_u3.ivt_result_u3_measurement_error = msg->ivt_result_u3_measurement_error;
                dbc_api.ivt_msg_result_u3.ivt_result_u3_ocs = msg->ivt_result_u3_ocs;
                dbc_api.ivt_msg_result_u3.ivt_result_u3_system_error = msg->ivt_result_u3_system_error;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::IvtMsgResultW>(
        "/can/dbc/ivt_msg_result_w", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::IvtMsgResultW> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_w.ivt_id_result_w = msg->ivt_id_result_w;
                dbc_api.ivt_msg_result_w.ivt_msgcount_result_w = msg->ivt_msgcount_result_w;
                dbc_api.ivt_msg_result_w.ivt_result_w = msg->ivt_result_w;
                dbc_api.ivt_msg_result_w.ivt_result_w_channel_error = msg->ivt_result_w_channel_error;
                dbc_api.ivt_msg_result_w.ivt_result_w_measurement_error = msg->ivt_result_w_measurement_error;
                dbc_api.ivt_msg_result_w.ivt_result_w_ocs = msg->ivt_result_w_ocs;
                dbc_api.ivt_msg_result_w.ivt_result_w_system_error = msg->ivt_result_w_system_error;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::IvtMsgResultWh>(
        "/can/dbc/ivt_msg_result_wh", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::IvtMsgResultWh> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_wh.ivt_id_result_wh = msg->ivt_id_result_wh;
                dbc_api.ivt_msg_result_wh.ivt_msgcount_result_wh = msg->ivt_msgcount_result_wh;
                dbc_api.ivt_msg_result_wh.ivt_result_wh = msg->ivt_result_wh;
                dbc_api.ivt_msg_result_wh.ivt_result_wh_channel_error = msg->ivt_result_wh_channel_error;
                dbc_api.ivt_msg_result_wh.ivt_result_wh_measurement_error = msg->ivt_result_wh_measurement_error;
                dbc_api.ivt_msg_result_wh.ivt_result_wh_ocs = msg->ivt_result_wh_ocs;
                dbc_api.ivt_msg_result_wh.ivt_result_wh_system_error = msg->ivt_result_wh_system_error;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Jetson>(
        "/can/dbc/jetson", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Jetson> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.jetson.as_mission = msg->as_mission;
                dbc_api.jetson.as_state = msg->as_state;
                dbc_api.jetson.cpu = msg->cpu;
                dbc_api.jetson.emergency_cause = msg->emergency_cause;
                dbc_api.jetson.gpu = msg->gpu;
                dbc_api.jetson.temperature = msg->temperature;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::MasterMscId1>(
        "/can/dbc/master_msc_id_1", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::MasterMscId1> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.master_msc_id_1.adbms_pec_error = msg->adbms_pec_error;
                dbc_api.master_msc_id_1.ams_current_draw = msg->ams_current_draw;
                dbc_api.master_msc_id_1.fault_counter = msg->fault_counter;
                dbc_api.master_msc_id_1.master_fan_pwm = msg->master_fan_pwm;
                dbc_api.master_msc_id_1.master_firmware_version = msg->master_firmware_version;
                dbc_api.master_msc_id_1.master_state = msg->master_state;
                dbc_api.master_msc_id_1.mcu_temperature = msg->mcu_temperature;
                dbc_api.master_msc_id_1.mcu_vref = msg->mcu_vref;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::MasterMscId2>(
        "/can/dbc/master_msc_id_2", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::MasterMscId2> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.master_msc_id_2.fault1_code = msg->fault1_code;
                dbc_api.master_msc_id_2.fault1_index_type = msg->fault1_index_type;
                dbc_api.master_msc_id_2.fault1_index_value = msg->fault1_index_value;
                dbc_api.master_msc_id_2.fault2_code = msg->fault2_code;
                dbc_api.master_msc_id_2.fault2_index_type = msg->fault2_index_type;
                dbc_api.master_msc_id_2.fault2_index_value = msg->fault2_index_value;
                dbc_api.master_msc_id_2.master_runtime = msg->master_runtime;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::MasterMscId3>(
        "/can/dbc/master_msc_id_3", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::MasterMscId3> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.master_msc_id_3.overall_maximum_temperature = msg->overall_maximum_temperature;
                dbc_api.master_msc_id_3.overall_maximum_voltage = msg->overall_maximum_voltage;
                dbc_api.master_msc_id_3.overall_minimum_temperature = msg->overall_minimum_temperature;
                dbc_api.master_msc_id_3.overall_minimum_voltage = msg->overall_minimum_voltage;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::MasterMscId4>(
        "/can/dbc/master_msc_id_4", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::MasterMscId4> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.master_msc_id_4.slaves_detected = msg->slaves_detected;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::MasterPrechargeId1>(
        "/can/dbc/master_precharge_id_1", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::MasterPrechargeId1> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.master_precharge_id_1.precharge_ctc_air_min_state = msg->precharge_ctc_air_min_state;
                dbc_api.master_precharge_id_1.precharge_ctc_air_pos_state = msg->precharge_ctc_air_pos_state;
                dbc_api.master_precharge_id_1.precharge_ctc_charge_state = msg->precharge_ctc_charge_state;
                dbc_api.master_precharge_id_1.precharge_ctc_discharge_state = msg->precharge_ctc_discharge_state;
                dbc_api.master_precharge_id_1.precharge_state = msg->precharge_state;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::MasterSocAccumulator>(
        "/can/dbc/master_soc_accumulator", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::MasterSocAccumulator> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.master_soc_accumulator.soc_float = msg->soc_float;
                dbc_api.master_soc_accumulator.soc_integer = msg->soc_integer;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::PdmCooling>(
        "/can/dbc/pdm_cooling", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::PdmCooling> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.pdm_cooling.coolingfan_pwm = msg->coolingfan_pwm;
                dbc_api.pdm_cooling.waterpump_pwm = msg->waterpump_pwm;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::PdmLv>(
        "/can/dbc/pdm_lv", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::PdmLv> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.pdm_lv.lv_voltage_mv = msg->lv_voltage_mv;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Res>(
        "/can/dbc/res", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Res> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.res.signal = msg->signal;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::SlamStatsCan>(
        "/can/dbc/slam_stats_can", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::SlamStatsCan> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slam_stats_can.cones_count_actual = msg->cones_count_actual;
                dbc_api.slam_stats_can.cones_count_all = msg->cones_count_all;
                dbc_api.slam_stats_can.lap_counter = msg->lap_counter;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave01MscId1>(
        "/can/dbc/slave_01_msc_id_1", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave01MscId1> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_01_msc_id_1.module_voltage_avg = msg->module_voltage_avg;
                dbc_api.slave_01_msc_id_1.module_voltage_max = msg->module_voltage_max;
                dbc_api.slave_01_msc_id_1.module_voltage_min = msg->module_voltage_min;
                dbc_api.slave_01_msc_id_1.module_voltage_sum = msg->module_voltage_sum;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave01MscId2>(
        "/can/dbc/slave_01_msc_id_2", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave01MscId2> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_01_msc_id_2.module_ic_temperature = msg->module_ic_temperature;
                dbc_api.slave_01_msc_id_2.module_ic_voltage = msg->module_ic_voltage;
                dbc_api.slave_01_msc_id_2.module_open_wire = msg->module_open_wire;
                dbc_api.slave_01_msc_id_2.module_overvoltage = msg->module_overvoltage;
                dbc_api.slave_01_msc_id_2.module_under_over_identifier = msg->module_under_over_identifier;
                dbc_api.slave_01_msc_id_2.module_undervoltage = msg->module_undervoltage;
                dbc_api.slave_01_msc_id_2.module_voltage_delta = msg->module_voltage_delta;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave01TemperatureId1>(
        "/can/dbc/slave_01_temperature_id_1", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave01TemperatureId1> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_01_temperature_id_1.temperature_value_1 = msg->temperature_value_1;
                dbc_api.slave_01_temperature_id_1.temperature_value_2 = msg->temperature_value_2;
                dbc_api.slave_01_temperature_id_1.temperature_value_3 = msg->temperature_value_3;
                dbc_api.slave_01_temperature_id_1.temperature_value_4 = msg->temperature_value_4;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave01TemperatureId2>(
        "/can/dbc/slave_01_temperature_id_2", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave01TemperatureId2> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_01_temperature_id_2.temperature_delta = msg->temperature_delta;
                dbc_api.slave_01_temperature_id_2.temperature_maximum = msg->temperature_maximum;
                dbc_api.slave_01_temperature_id_2.temperature_value_5 = msg->temperature_value_5;
                dbc_api.slave_01_temperature_id_2.temperature_value_6 = msg->temperature_value_6;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave01VoltageId1>(
        "/can/dbc/slave_01_voltage_id_1", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave01VoltageId1> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_01_voltage_id_1.cell_voltage_1 = msg->cell_voltage_1;
                dbc_api.slave_01_voltage_id_1.cell_voltage_2 = msg->cell_voltage_2;
                dbc_api.slave_01_voltage_id_1.cell_voltage_3 = msg->cell_voltage_3;
                dbc_api.slave_01_voltage_id_1.cell_voltage_4 = msg->cell_voltage_4;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave01VoltageId2>(
        "/can/dbc/slave_01_voltage_id_2", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave01VoltageId2> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_01_voltage_id_2.cell_voltage_5 = msg->cell_voltage_5;
                dbc_api.slave_01_voltage_id_2.cell_voltage_6 = msg->cell_voltage_6;
                dbc_api.slave_01_voltage_id_2.cell_voltage_7 = msg->cell_voltage_7;
                dbc_api.slave_01_voltage_id_2.cell_voltage_8 = msg->cell_voltage_8;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave01VoltageId3>(
        "/can/dbc/slave_01_voltage_id_3", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave01VoltageId3> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_01_voltage_id_3.cell_voltage_10 = msg->cell_voltage_10;
                dbc_api.slave_01_voltage_id_3.cell_voltage_11 = msg->cell_voltage_11;
                dbc_api.slave_01_voltage_id_3.cell_voltage_12 = msg->cell_voltage_12;
                dbc_api.slave_01_voltage_id_3.cell_voltage_9 = msg->cell_voltage_9;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave02MscId1>(
        "/can/dbc/slave_02_msc_id_1", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave02MscId1> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_02_msc_id_1.module_voltage_avg = msg->module_voltage_avg;
                dbc_api.slave_02_msc_id_1.module_voltage_max = msg->module_voltage_max;
                dbc_api.slave_02_msc_id_1.module_voltage_min = msg->module_voltage_min;
                dbc_api.slave_02_msc_id_1.module_voltage_sum = msg->module_voltage_sum;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave02MscId2>(
        "/can/dbc/slave_02_msc_id_2", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave02MscId2> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_02_msc_id_2.module_ic_temperature = msg->module_ic_temperature;
                dbc_api.slave_02_msc_id_2.module_ic_voltage = msg->module_ic_voltage;
                dbc_api.slave_02_msc_id_2.module_open_wire = msg->module_open_wire;
                dbc_api.slave_02_msc_id_2.module_overvoltage = msg->module_overvoltage;
                dbc_api.slave_02_msc_id_2.module_under_over_identifier = msg->module_under_over_identifier;
                dbc_api.slave_02_msc_id_2.module_undervoltage = msg->module_undervoltage;
                dbc_api.slave_02_msc_id_2.module_voltage_delta = msg->module_voltage_delta;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave02TemperatureId1>(
        "/can/dbc/slave_02_temperature_id_1", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave02TemperatureId1> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_02_temperature_id_1.temperature_value_1 = msg->temperature_value_1;
                dbc_api.slave_02_temperature_id_1.temperature_value_2 = msg->temperature_value_2;
                dbc_api.slave_02_temperature_id_1.temperature_value_3 = msg->temperature_value_3;
                dbc_api.slave_02_temperature_id_1.temperature_value_4 = msg->temperature_value_4;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave02TemperatureId2>(
        "/can/dbc/slave_02_temperature_id_2", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave02TemperatureId2> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_02_temperature_id_2.temperature_delta = msg->temperature_delta;
                dbc_api.slave_02_temperature_id_2.temperature_maximum = msg->temperature_maximum;
                dbc_api.slave_02_temperature_id_2.temperature_value_5 = msg->temperature_value_5;
                dbc_api.slave_02_temperature_id_2.temperature_value_6 = msg->temperature_value_6;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave02VoltageId1>(
        "/can/dbc/slave_02_voltage_id_1", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave02VoltageId1> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_02_voltage_id_1.cell_voltage_1 = msg->cell_voltage_1;
                dbc_api.slave_02_voltage_id_1.cell_voltage_2 = msg->cell_voltage_2;
                dbc_api.slave_02_voltage_id_1.cell_voltage_3 = msg->cell_voltage_3;
                dbc_api.slave_02_voltage_id_1.cell_voltage_4 = msg->cell_voltage_4;
            }
        }));
}
#endif
#endif
