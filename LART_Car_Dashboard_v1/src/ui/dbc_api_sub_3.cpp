// Auto-generated from DBC files by generate_dbc_api.py. Do not edit.
#include "dbc_api.h"
#if defined(LART_UI_HAVE_RCLCPP) && LART_UI_HAVE_RCLCPP
#if defined(LART_HAVE_LART_MSGS) && LART_HAVE_LART_MSGS
#include <rclcpp/rclcpp.hpp>
#include <mutex>
#include <vector>
extern std::mutex dbc_api_mutex;
#include <lart_msgs/msg/slave02_msc_id1.hpp>
#include <lart_msgs/msg/slave02_msc_id2.hpp>
#include <lart_msgs/msg/slave02_temperature_id1.hpp>
#include <lart_msgs/msg/slave02_temperature_id2.hpp>
#include <lart_msgs/msg/slave02_voltage_id1.hpp>
#include <lart_msgs/msg/slave02_voltage_id2.hpp>
#include <lart_msgs/msg/slave02_voltage_id3.hpp>
#include <lart_msgs/msg/slave03_msc_id1.hpp>
#include <lart_msgs/msg/slave03_msc_id2.hpp>
#include <lart_msgs/msg/slave03_temperature_id1.hpp>
#include <lart_msgs/msg/slave03_temperature_id2.hpp>
#include <lart_msgs/msg/slave03_voltage_id1.hpp>
#include <lart_msgs/msg/slave03_voltage_id2.hpp>
#include <lart_msgs/msg/slave03_voltage_id3.hpp>
#include <lart_msgs/msg/slave04_msc_id1.hpp>
#include <lart_msgs/msg/slave04_msc_id2.hpp>
#include <lart_msgs/msg/slave04_temperature_id1.hpp>
#include <lart_msgs/msg/slave04_temperature_id2.hpp>
#include <lart_msgs/msg/slave04_voltage_id1.hpp>
#include <lart_msgs/msg/slave04_voltage_id2.hpp>
#include <lart_msgs/msg/slave04_voltage_id3.hpp>
#include <lart_msgs/msg/slave05_msc_id1.hpp>
#include <lart_msgs/msg/slave05_msc_id2.hpp>
#include <lart_msgs/msg/slave05_temperature_id1.hpp>
#include <lart_msgs/msg/slave05_temperature_id2.hpp>
#include <lart_msgs/msg/slave05_voltage_id1.hpp>
#include <lart_msgs/msg/slave05_voltage_id2.hpp>
#include <lart_msgs/msg/slave05_voltage_id3.hpp>
#include <lart_msgs/msg/slave06_msc_id1.hpp>
#include <lart_msgs/msg/slave06_msc_id2.hpp>

void init_dbc_api_subscribers_chunk_3(std::shared_ptr<rclcpp::Node> node, std::vector<rclcpp::SubscriptionBase::SharedPtr>& subs) {
    auto sensor_qos = rclcpp::QoS(10).best_effort();

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
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave02VoltageId2>(
        "/can/dbc/slave_02_voltage_id_2", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave02VoltageId2> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_02_voltage_id_2.cell_voltage_5 = msg->cell_voltage_5;
                dbc_api.slave_02_voltage_id_2.cell_voltage_6 = msg->cell_voltage_6;
                dbc_api.slave_02_voltage_id_2.cell_voltage_7 = msg->cell_voltage_7;
                dbc_api.slave_02_voltage_id_2.cell_voltage_8 = msg->cell_voltage_8;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave02VoltageId3>(
        "/can/dbc/slave_02_voltage_id_3", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave02VoltageId3> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_02_voltage_id_3.cell_voltage_10 = msg->cell_voltage_10;
                dbc_api.slave_02_voltage_id_3.cell_voltage_11 = msg->cell_voltage_11;
                dbc_api.slave_02_voltage_id_3.cell_voltage_12 = msg->cell_voltage_12;
                dbc_api.slave_02_voltage_id_3.cell_voltage_9 = msg->cell_voltage_9;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave03MscId1>(
        "/can/dbc/slave_03_msc_id_1", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave03MscId1> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_03_msc_id_1.module_voltage_avg = msg->module_voltage_avg;
                dbc_api.slave_03_msc_id_1.module_voltage_max = msg->module_voltage_max;
                dbc_api.slave_03_msc_id_1.module_voltage_min = msg->module_voltage_min;
                dbc_api.slave_03_msc_id_1.module_voltage_sum = msg->module_voltage_sum;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave03MscId2>(
        "/can/dbc/slave_03_msc_id_2", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave03MscId2> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_03_msc_id_2.module_ic_temperature = msg->module_ic_temperature;
                dbc_api.slave_03_msc_id_2.module_ic_voltage = msg->module_ic_voltage;
                dbc_api.slave_03_msc_id_2.module_open_wire = msg->module_open_wire;
                dbc_api.slave_03_msc_id_2.module_voltage_delta = msg->module_voltage_delta;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave03TemperatureId1>(
        "/can/dbc/slave_03_temperature_id_1", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave03TemperatureId1> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_03_temperature_id_1.temperature_value_1 = msg->temperature_value_1;
                dbc_api.slave_03_temperature_id_1.temperature_value_2 = msg->temperature_value_2;
                dbc_api.slave_03_temperature_id_1.temperature_value_3 = msg->temperature_value_3;
                dbc_api.slave_03_temperature_id_1.temperature_value_4 = msg->temperature_value_4;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave03TemperatureId2>(
        "/can/dbc/slave_03_temperature_id_2", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave03TemperatureId2> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_03_temperature_id_2.temperature_delta = msg->temperature_delta;
                dbc_api.slave_03_temperature_id_2.temperature_maximum = msg->temperature_maximum;
                dbc_api.slave_03_temperature_id_2.temperature_value_5 = msg->temperature_value_5;
                dbc_api.slave_03_temperature_id_2.temperature_value_6 = msg->temperature_value_6;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave03VoltageId1>(
        "/can/dbc/slave_03_voltage_id_1", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave03VoltageId1> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_03_voltage_id_1.cell_voltage_1 = msg->cell_voltage_1;
                dbc_api.slave_03_voltage_id_1.cell_voltage_2 = msg->cell_voltage_2;
                dbc_api.slave_03_voltage_id_1.cell_voltage_3 = msg->cell_voltage_3;
                dbc_api.slave_03_voltage_id_1.cell_voltage_4 = msg->cell_voltage_4;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave03VoltageId2>(
        "/can/dbc/slave_03_voltage_id_2", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave03VoltageId2> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_03_voltage_id_2.cell_voltage_5 = msg->cell_voltage_5;
                dbc_api.slave_03_voltage_id_2.cell_voltage_6 = msg->cell_voltage_6;
                dbc_api.slave_03_voltage_id_2.cell_voltage_7 = msg->cell_voltage_7;
                dbc_api.slave_03_voltage_id_2.cell_voltage_8 = msg->cell_voltage_8;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave03VoltageId3>(
        "/can/dbc/slave_03_voltage_id_3", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave03VoltageId3> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_03_voltage_id_3.cell_voltage_10 = msg->cell_voltage_10;
                dbc_api.slave_03_voltage_id_3.cell_voltage_11 = msg->cell_voltage_11;
                dbc_api.slave_03_voltage_id_3.cell_voltage_12 = msg->cell_voltage_12;
                dbc_api.slave_03_voltage_id_3.cell_voltage_9 = msg->cell_voltage_9;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave04MscId1>(
        "/can/dbc/slave_04_msc_id_1", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave04MscId1> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_04_msc_id_1.module_voltage_avg = msg->module_voltage_avg;
                dbc_api.slave_04_msc_id_1.module_voltage_max = msg->module_voltage_max;
                dbc_api.slave_04_msc_id_1.module_voltage_min = msg->module_voltage_min;
                dbc_api.slave_04_msc_id_1.module_voltage_sum = msg->module_voltage_sum;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave04MscId2>(
        "/can/dbc/slave_04_msc_id_2", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave04MscId2> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_04_msc_id_2.module_ic_temperature = msg->module_ic_temperature;
                dbc_api.slave_04_msc_id_2.module_ic_voltage = msg->module_ic_voltage;
                dbc_api.slave_04_msc_id_2.module_open_wire = msg->module_open_wire;
                dbc_api.slave_04_msc_id_2.module_voltage_delta = msg->module_voltage_delta;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave04TemperatureId1>(
        "/can/dbc/slave_04_temperature_id_1", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave04TemperatureId1> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_04_temperature_id_1.temperature_value_1 = msg->temperature_value_1;
                dbc_api.slave_04_temperature_id_1.temperature_value_2 = msg->temperature_value_2;
                dbc_api.slave_04_temperature_id_1.temperature_value_3 = msg->temperature_value_3;
                dbc_api.slave_04_temperature_id_1.temperature_value_4 = msg->temperature_value_4;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave04TemperatureId2>(
        "/can/dbc/slave_04_temperature_id_2", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave04TemperatureId2> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_04_temperature_id_2.temperature_delta = msg->temperature_delta;
                dbc_api.slave_04_temperature_id_2.temperature_maximum = msg->temperature_maximum;
                dbc_api.slave_04_temperature_id_2.temperature_value_5 = msg->temperature_value_5;
                dbc_api.slave_04_temperature_id_2.temperature_value_6 = msg->temperature_value_6;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave04VoltageId1>(
        "/can/dbc/slave_04_voltage_id_1", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave04VoltageId1> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_04_voltage_id_1.cell_voltage_1 = msg->cell_voltage_1;
                dbc_api.slave_04_voltage_id_1.cell_voltage_2 = msg->cell_voltage_2;
                dbc_api.slave_04_voltage_id_1.cell_voltage_3 = msg->cell_voltage_3;
                dbc_api.slave_04_voltage_id_1.cell_voltage_4 = msg->cell_voltage_4;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave04VoltageId2>(
        "/can/dbc/slave_04_voltage_id_2", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave04VoltageId2> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_04_voltage_id_2.cell_voltage_5 = msg->cell_voltage_5;
                dbc_api.slave_04_voltage_id_2.cell_voltage_6 = msg->cell_voltage_6;
                dbc_api.slave_04_voltage_id_2.cell_voltage_7 = msg->cell_voltage_7;
                dbc_api.slave_04_voltage_id_2.cell_voltage_8 = msg->cell_voltage_8;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave04VoltageId3>(
        "/can/dbc/slave_04_voltage_id_3", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave04VoltageId3> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_04_voltage_id_3.cell_voltage_10 = msg->cell_voltage_10;
                dbc_api.slave_04_voltage_id_3.cell_voltage_11 = msg->cell_voltage_11;
                dbc_api.slave_04_voltage_id_3.cell_voltage_12 = msg->cell_voltage_12;
                dbc_api.slave_04_voltage_id_3.cell_voltage_9 = msg->cell_voltage_9;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave05MscId1>(
        "/can/dbc/slave_05_msc_id_1", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave05MscId1> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_05_msc_id_1.module_voltage_avg = msg->module_voltage_avg;
                dbc_api.slave_05_msc_id_1.module_voltage_max = msg->module_voltage_max;
                dbc_api.slave_05_msc_id_1.module_voltage_min = msg->module_voltage_min;
                dbc_api.slave_05_msc_id_1.module_voltage_sum = msg->module_voltage_sum;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave05MscId2>(
        "/can/dbc/slave_05_msc_id_2", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave05MscId2> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_05_msc_id_2.module_ic_temperature = msg->module_ic_temperature;
                dbc_api.slave_05_msc_id_2.module_ic_voltage = msg->module_ic_voltage;
                dbc_api.slave_05_msc_id_2.module_open_wire = msg->module_open_wire;
                dbc_api.slave_05_msc_id_2.module_voltage_delta = msg->module_voltage_delta;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave05TemperatureId1>(
        "/can/dbc/slave_05_temperature_id_1", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave05TemperatureId1> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_05_temperature_id_1.temperature_value_1 = msg->temperature_value_1;
                dbc_api.slave_05_temperature_id_1.temperature_value_2 = msg->temperature_value_2;
                dbc_api.slave_05_temperature_id_1.temperature_value_3 = msg->temperature_value_3;
                dbc_api.slave_05_temperature_id_1.temperature_value_4 = msg->temperature_value_4;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave05TemperatureId2>(
        "/can/dbc/slave_05_temperature_id_2", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave05TemperatureId2> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_05_temperature_id_2.temperature_delta = msg->temperature_delta;
                dbc_api.slave_05_temperature_id_2.temperature_maximum = msg->temperature_maximum;
                dbc_api.slave_05_temperature_id_2.temperature_value_5 = msg->temperature_value_5;
                dbc_api.slave_05_temperature_id_2.temperature_value_6 = msg->temperature_value_6;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave05VoltageId1>(
        "/can/dbc/slave_05_voltage_id_1", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave05VoltageId1> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_05_voltage_id_1.cell_voltage_1 = msg->cell_voltage_1;
                dbc_api.slave_05_voltage_id_1.cell_voltage_2 = msg->cell_voltage_2;
                dbc_api.slave_05_voltage_id_1.cell_voltage_3 = msg->cell_voltage_3;
                dbc_api.slave_05_voltage_id_1.cell_voltage_4 = msg->cell_voltage_4;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave05VoltageId2>(
        "/can/dbc/slave_05_voltage_id_2", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave05VoltageId2> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_05_voltage_id_2.cell_voltage_5 = msg->cell_voltage_5;
                dbc_api.slave_05_voltage_id_2.cell_voltage_6 = msg->cell_voltage_6;
                dbc_api.slave_05_voltage_id_2.cell_voltage_7 = msg->cell_voltage_7;
                dbc_api.slave_05_voltage_id_2.cell_voltage_8 = msg->cell_voltage_8;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave05VoltageId3>(
        "/can/dbc/slave_05_voltage_id_3", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave05VoltageId3> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_05_voltage_id_3.cell_voltage_10 = msg->cell_voltage_10;
                dbc_api.slave_05_voltage_id_3.cell_voltage_11 = msg->cell_voltage_11;
                dbc_api.slave_05_voltage_id_3.cell_voltage_12 = msg->cell_voltage_12;
                dbc_api.slave_05_voltage_id_3.cell_voltage_9 = msg->cell_voltage_9;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave06MscId1>(
        "/can/dbc/slave_06_msc_id_1", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave06MscId1> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_06_msc_id_1.module_voltage_avg = msg->module_voltage_avg;
                dbc_api.slave_06_msc_id_1.module_voltage_max = msg->module_voltage_max;
                dbc_api.slave_06_msc_id_1.module_voltage_min = msg->module_voltage_min;
                dbc_api.slave_06_msc_id_1.module_voltage_sum = msg->module_voltage_sum;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Slave06MscId2>(
        "/can/dbc/slave_06_msc_id_2", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Slave06MscId2> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.slave_06_msc_id_2.module_ic_temperature = msg->module_ic_temperature;
                dbc_api.slave_06_msc_id_2.module_ic_voltage = msg->module_ic_voltage;
                dbc_api.slave_06_msc_id_2.module_open_wire = msg->module_open_wire;
                dbc_api.slave_06_msc_id_2.module_voltage_delta = msg->module_voltage_delta;
            }
        }));
}
#endif
#endif
