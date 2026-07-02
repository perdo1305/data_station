// Auto-generated from DBC files by generate_dbc_api.py. Do not edit.
#include "dbc_api.h"
#if defined(LART_UI_HAVE_RCLCPP) && LART_UI_HAVE_RCLCPP
#if defined(LART_HAVE_LART_MSGS) && LART_HAVE_LART_MSGS
#include <rclcpp/rclcpp.hpp>
#include <mutex>
#include <vector>
extern std::mutex dbc_api_mutex;
#include <lart_msgs/msg/inv1_setdriveenable.hpp>
#include <lart_msgs/msg/inv1_seterpm.hpp>
#include <lart_msgs/msg/inv1_setmaxacbrakecurrent.hpp>
#include <lart_msgs/msg/inv1_setmaxaccurrent.hpp>
#include <lart_msgs/msg/inv1_setmaxdcbrakecurrent.hpp>
#include <lart_msgs/msg/inv1_setmaxdccurrent.hpp>
#include <lart_msgs/msg/inv1_setposition.hpp>
#include <lart_msgs/msg/inv1_setrelbrakecurrent.hpp>
#include <lart_msgs/msg/inv1_setrelcurrent.hpp>
#include <lart_msgs/msg/inv1_targetiq.hpp>
#include <lart_msgs/msg/inv1_temperatures.hpp>
#include <lart_msgs/msg/inv2_ac_dc_current.hpp>
#include <lart_msgs/msg/inv2_erpm_duty_voltage.hpp>
#include <lart_msgs/msg/inv2_foc.hpp>
#include <lart_msgs/msg/inv2_minmaxaccurrent.hpp>
#include <lart_msgs/msg/inv2_minmaxdccurrent.hpp>
#include <lart_msgs/msg/inv2_misc.hpp>
#include <lart_msgs/msg/inv2_setaccurrent.hpp>
#include <lart_msgs/msg/inv2_setbrakecurrent.hpp>
#include <lart_msgs/msg/inv2_setdigoutput.hpp>
#include <lart_msgs/msg/inv2_setdriveenable.hpp>
#include <lart_msgs/msg/inv2_seterpm.hpp>
#include <lart_msgs/msg/inv2_setmaxacbrakecurrent.hpp>
#include <lart_msgs/msg/inv2_setmaxaccurrent.hpp>
#include <lart_msgs/msg/inv2_setmaxdcbrakecurrent.hpp>
#include <lart_msgs/msg/inv2_setmaxdccurrent.hpp>
#include <lart_msgs/msg/inv2_setposition.hpp>
#include <lart_msgs/msg/inv2_setrelbrakecurrent.hpp>
#include <lart_msgs/msg/inv2_setrelcurrent.hpp>
#include <lart_msgs/msg/inv2_targetiq.hpp>

void init_dbc_api_subscribers_chunk_1(std::shared_ptr<rclcpp::Node> node, std::vector<rclcpp::SubscriptionBase::SharedPtr>& subs) {
    auto sensor_qos = rclcpp::QoS(10).best_effort();

    subs.push_back(node->create_subscription<lart_msgs::msg::Inv1Setdriveenable>(
        "/can/dbc/inv1_setdriveenable", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv1Setdriveenable> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv1_setdriveenable.inv1_cmd_driveenable = msg->inv1_cmd_driveenable;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv1Seterpm>(
        "/can/dbc/inv1_seterpm", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv1Seterpm> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv1_seterpm.inv1_cmd_targetspeed = msg->inv1_cmd_targetspeed;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv1Setmaxacbrakecurrent>(
        "/can/dbc/inv1_setmaxacbrakecurrent", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv1Setmaxacbrakecurrent> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv1_setmaxacbrakecurrent.inv1_cmd_maxacbrakecurrent = msg->inv1_cmd_maxacbrakecurrent;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv1Setmaxaccurrent>(
        "/can/dbc/inv1_setmaxaccurrent", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv1Setmaxaccurrent> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv1_setmaxaccurrent.inv1_cmd_maxaccurrent = msg->inv1_cmd_maxaccurrent;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv1Setmaxdcbrakecurrent>(
        "/can/dbc/inv1_setmaxdcbrakecurrent", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv1Setmaxdcbrakecurrent> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv1_setmaxdcbrakecurrent.inv1_cmd_maxdcbrakecurrent = msg->inv1_cmd_maxdcbrakecurrent;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv1Setmaxdccurrent>(
        "/can/dbc/inv1_setmaxdccurrent", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv1Setmaxdccurrent> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv1_setmaxdccurrent.inv1_cmd_maxdccurrent = msg->inv1_cmd_maxdccurrent;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv1Setposition>(
        "/can/dbc/inv1_setposition", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv1Setposition> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv1_setposition.inv1_cmd_targetposition = msg->inv1_cmd_targetposition;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv1Setrelbrakecurrent>(
        "/can/dbc/inv1_setrelbrakecurrent", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv1Setrelbrakecurrent> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv1_setrelbrakecurrent.inv1_cmd_tgtrelbrakecurrent = msg->inv1_cmd_tgtrelbrakecurrent;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv1Setrelcurrent>(
        "/can/dbc/inv1_setrelcurrent", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv1Setrelcurrent> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv1_setrelcurrent.inv1_cmd_targetrelativecurrent = msg->inv1_cmd_targetrelativecurrent;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv1Targetiq>(
        "/can/dbc/inv1_targetiq", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv1Targetiq> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv1_targetiq.inv1_controlmode = msg->inv1_controlmode;
                dbc_api.inv1_targetiq.inv1_ismotorstill = msg->inv1_ismotorstill;
                dbc_api.inv1_targetiq.inv1_motorposition = msg->inv1_motorposition;
                dbc_api.inv1_targetiq.inv1_targetiq = msg->inv1_targetiq;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv1Temperatures>(
        "/can/dbc/inv1_temperatures", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv1Temperatures> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv1_temperatures.inv1_actual_faultcode = msg->inv1_actual_faultcode;
                dbc_api.inv1_temperatures.inv1_actual_tempcontroller = msg->inv1_actual_tempcontroller;
                dbc_api.inv1_temperatures.inv1_actual_tempmotor = msg->inv1_actual_tempmotor;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv2AcDcCurrent>(
        "/can/dbc/inv2_ac_dc_current", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv2AcDcCurrent> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv2_ac_dc_current.inv2_actual_accurrent = msg->inv2_actual_accurrent;
                dbc_api.inv2_ac_dc_current.inv2_actual_dccurrent = msg->inv2_actual_dccurrent;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv2ErpmDutyVoltage>(
        "/can/dbc/inv2_erpm_duty_voltage", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv2ErpmDutyVoltage> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv2_erpm_duty_voltage.inv2_actual_duty = msg->inv2_actual_duty;
                dbc_api.inv2_erpm_duty_voltage.inv2_actual_erpm = msg->inv2_actual_erpm;
                dbc_api.inv2_erpm_duty_voltage.inv2_actual_inputvoltage = msg->inv2_actual_inputvoltage;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv2Foc>(
        "/can/dbc/inv2_foc", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv2Foc> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv2_foc.inv2_actual_foc_id = msg->inv2_actual_foc_id;
                dbc_api.inv2_foc.inv2_actual_foc_iq = msg->inv2_actual_foc_iq;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv2Minmaxaccurrent>(
        "/can/dbc/inv2_minmaxaccurrent", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv2Minmaxaccurrent> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv2_minmaxaccurrent.inv2_availablemaxaccurrent = msg->inv2_availablemaxaccurrent;
                dbc_api.inv2_minmaxaccurrent.inv2_availableminaccurrent = msg->inv2_availableminaccurrent;
                dbc_api.inv2_minmaxaccurrent.inv2_maxaccurrent = msg->inv2_maxaccurrent;
                dbc_api.inv2_minmaxaccurrent.inv2_minaccurrent = msg->inv2_minaccurrent;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv2Minmaxdccurrent>(
        "/can/dbc/inv2_minmaxdccurrent", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv2Minmaxdccurrent> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv2_minmaxdccurrent.inv2_availablemaxdccurrent = msg->inv2_availablemaxdccurrent;
                dbc_api.inv2_minmaxdccurrent.inv2_availablemindccurrent = msg->inv2_availablemindccurrent;
                dbc_api.inv2_minmaxdccurrent.inv2_maxdccurrent = msg->inv2_maxdccurrent;
                dbc_api.inv2_minmaxdccurrent.inv2_mindccurrent = msg->inv2_mindccurrent;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv2Misc>(
        "/can/dbc/inv2_misc", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv2Misc> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv2_misc.inv2_actual_brake = msg->inv2_actual_brake;
                dbc_api.inv2_misc.inv2_actual_throttle = msg->inv2_actual_throttle;
                dbc_api.inv2_misc.inv2_can_map_version = msg->inv2_can_map_version;
                dbc_api.inv2_misc.inv2_capacitor_temp_limit = msg->inv2_capacitor_temp_limit;
                dbc_api.inv2_misc.inv2_dc_current_limit = msg->inv2_dc_current_limit;
                dbc_api.inv2_misc.inv2_digital_input_1 = msg->inv2_digital_input_1;
                dbc_api.inv2_misc.inv2_digital_input_2 = msg->inv2_digital_input_2;
                dbc_api.inv2_misc.inv2_digital_input_3 = msg->inv2_digital_input_3;
                dbc_api.inv2_misc.inv2_digital_input_4 = msg->inv2_digital_input_4;
                dbc_api.inv2_misc.inv2_digital_output_1 = msg->inv2_digital_output_1;
                dbc_api.inv2_misc.inv2_digital_output_2 = msg->inv2_digital_output_2;
                dbc_api.inv2_misc.inv2_digital_output_3 = msg->inv2_digital_output_3;
                dbc_api.inv2_misc.inv2_digital_output_4 = msg->inv2_digital_output_4;
                dbc_api.inv2_misc.inv2_drive_enable = msg->inv2_drive_enable;
                dbc_api.inv2_misc.inv2_drive_enable_limit = msg->inv2_drive_enable_limit;
                dbc_api.inv2_misc.inv2_igbt_accel_limit = msg->inv2_igbt_accel_limit;
                dbc_api.inv2_misc.inv2_igbt_temp_limit = msg->inv2_igbt_temp_limit;
                dbc_api.inv2_misc.inv2_input_voltage_limit = msg->inv2_input_voltage_limit;
                dbc_api.inv2_misc.inv2_motor_accel_limit = msg->inv2_motor_accel_limit;
                dbc_api.inv2_misc.inv2_motor_temp_limit = msg->inv2_motor_temp_limit;
                dbc_api.inv2_misc.inv2_power_limit = msg->inv2_power_limit;
                dbc_api.inv2_misc.inv2_rpm_max_limit = msg->inv2_rpm_max_limit;
                dbc_api.inv2_misc.inv2_rpm_min_limit = msg->inv2_rpm_min_limit;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv2Setaccurrent>(
        "/can/dbc/inv2_setaccurrent", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv2Setaccurrent> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv2_setaccurrent.inv2_cmd_targetaccurrent = msg->inv2_cmd_targetaccurrent;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv2Setbrakecurrent>(
        "/can/dbc/inv2_setbrakecurrent", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv2Setbrakecurrent> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv2_setbrakecurrent.inv2_cmd_targetbrakecurrent = msg->inv2_cmd_targetbrakecurrent;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv2Setdigoutput>(
        "/can/dbc/inv2_setdigoutput", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv2Setdigoutput> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv2_setdigoutput.inv2_cmd_setdigoutput1 = msg->inv2_cmd_setdigoutput1;
                dbc_api.inv2_setdigoutput.inv2_cmd_setdigoutput2 = msg->inv2_cmd_setdigoutput2;
                dbc_api.inv2_setdigoutput.inv2_cmd_setdigoutput3 = msg->inv2_cmd_setdigoutput3;
                dbc_api.inv2_setdigoutput.inv2_cmd_setdigoutput4 = msg->inv2_cmd_setdigoutput4;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv2Setdriveenable>(
        "/can/dbc/inv2_setdriveenable", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv2Setdriveenable> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv2_setdriveenable.inv2_cmd_driveenable = msg->inv2_cmd_driveenable;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv2Seterpm>(
        "/can/dbc/inv2_seterpm", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv2Seterpm> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv2_seterpm.inv2_cmd_targetspeed = msg->inv2_cmd_targetspeed;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv2Setmaxacbrakecurrent>(
        "/can/dbc/inv2_setmaxacbrakecurrent", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv2Setmaxacbrakecurrent> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv2_setmaxacbrakecurrent.inv2_cmd_maxacbrakecurrent = msg->inv2_cmd_maxacbrakecurrent;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv2Setmaxaccurrent>(
        "/can/dbc/inv2_setmaxaccurrent", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv2Setmaxaccurrent> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv2_setmaxaccurrent.inv2_cmd_maxaccurrent = msg->inv2_cmd_maxaccurrent;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv2Setmaxdcbrakecurrent>(
        "/can/dbc/inv2_setmaxdcbrakecurrent", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv2Setmaxdcbrakecurrent> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv2_setmaxdcbrakecurrent.inv2_cmd_maxdcbrakecurrent = msg->inv2_cmd_maxdcbrakecurrent;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv2Setmaxdccurrent>(
        "/can/dbc/inv2_setmaxdccurrent", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv2Setmaxdccurrent> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv2_setmaxdccurrent.inv2_cmd_maxdccurrent = msg->inv2_cmd_maxdccurrent;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv2Setposition>(
        "/can/dbc/inv2_setposition", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv2Setposition> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv2_setposition.inv2_cmd_targetposition = msg->inv2_cmd_targetposition;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv2Setrelbrakecurrent>(
        "/can/dbc/inv2_setrelbrakecurrent", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv2Setrelbrakecurrent> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv2_setrelbrakecurrent.inv2_cmd_tgtrelbrakecurrent = msg->inv2_cmd_tgtrelbrakecurrent;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv2Setrelcurrent>(
        "/can/dbc/inv2_setrelcurrent", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv2Setrelcurrent> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv2_setrelcurrent.inv2_cmd_targetrelativecurrent = msg->inv2_cmd_targetrelativecurrent;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv2Targetiq>(
        "/can/dbc/inv2_targetiq", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv2Targetiq> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv2_targetiq.inv2_controlmode = msg->inv2_controlmode;
                dbc_api.inv2_targetiq.inv2_ismotorstill = msg->inv2_ismotorstill;
                dbc_api.inv2_targetiq.inv2_motorposition = msg->inv2_motorposition;
                dbc_api.inv2_targetiq.inv2_targetiq = msg->inv2_targetiq;
            }
        }));
}
#endif
#endif
