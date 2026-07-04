// Auto-generated from DBC files by generate_dbc_api.py. Do not edit.
#include "dbc_api.h"
#if defined(LART_UI_HAVE_RCLCPP) && LART_UI_HAVE_RCLCPP
#if defined(LART_HAVE_LART_MSGS) && LART_HAVE_LART_MSGS
#include <rclcpp/rclcpp.hpp>
#include <mutex>
#include <vector>
extern std::mutex dbc_api_mutex;
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
#include <lart_msgs/msg/inv2_temperatures.hpp>
#include <lart_msgs/msg/ivt_msg_cmd.hpp>
#include <lart_msgs/msg/ivt_msg_response.hpp>
#include <lart_msgs/msg/ivt_msg_result_as.hpp>

void init_dbc_api_subscribers_chunk_1(std::shared_ptr<rclcpp::Node> node, std::vector<rclcpp::SubscriptionBase::SharedPtr>& subs) {
    auto sensor_qos = rclcpp::QoS(10).best_effort();

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
                dbc_api.inv1_temperatures.inv1_tempinverter = msg->inv1_tempinverter;
                dbc_api.inv1_temperatures.inv1_tempmotor = msg->inv1_tempmotor;
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
    subs.push_back(node->create_subscription<lart_msgs::msg::Inv2Temperatures>(
        "/can/dbc/inv2_temperatures", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Inv2Temperatures> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.inv2_temperatures.inv2_actual_faultcode = msg->inv2_actual_faultcode;
                dbc_api.inv2_temperatures.inv2_actual_tempcontroller = msg->inv2_actual_tempcontroller;
                dbc_api.inv2_temperatures.inv2_actual_tempmotor = msg->inv2_actual_tempmotor;
                dbc_api.inv2_temperatures.inv2_tempinverter = msg->inv2_tempinverter;
                dbc_api.inv2_temperatures.inv2_tempmotor = msg->inv2_tempmotor;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::IvtMsgCmd>(
        "/can/dbc/ivt_msg_cmd", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::IvtMsgCmd> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_cmd.ivt_id_cmd = msg->ivt_id_cmd;
                dbc_api.ivt_msg_cmd.val_10_set_can_id_sn_i = msg->val_10_set_can_id_sn_i;
                dbc_api.ivt_msg_cmd.val_10_set_can_id_val_i = msg->val_10_set_can_id_val_i;
                dbc_api.ivt_msg_cmd.val_11_set_can_id_sn_u1 = msg->val_11_set_can_id_sn_u1;
                dbc_api.ivt_msg_cmd.val_11_set_can_id_val_u1 = msg->val_11_set_can_id_val_u1;
                dbc_api.ivt_msg_cmd.val_12_set_can_id_sn_u2 = msg->val_12_set_can_id_sn_u2;
                dbc_api.ivt_msg_cmd.val_12_set_can_id_val_u2 = msg->val_12_set_can_id_val_u2;
                dbc_api.ivt_msg_cmd.val_13_set_can_id_sn_u3 = msg->val_13_set_can_id_sn_u3;
                dbc_api.ivt_msg_cmd.val_13_set_can_id_val_u3 = msg->val_13_set_can_id_val_u3;
                dbc_api.ivt_msg_cmd.val_14_set_can_id_sn_t = msg->val_14_set_can_id_sn_t;
                dbc_api.ivt_msg_cmd.val_14_set_can_id_val_t = msg->val_14_set_can_id_val_t;
                dbc_api.ivt_msg_cmd.val_15_set_can_id_sn_w = msg->val_15_set_can_id_sn_w;
                dbc_api.ivt_msg_cmd.val_15_set_can_id_val_w = msg->val_15_set_can_id_val_w;
                dbc_api.ivt_msg_cmd.val_16_set_can_id_sn_as = msg->val_16_set_can_id_sn_as;
                dbc_api.ivt_msg_cmd.val_16_set_can_id_val_as = msg->val_16_set_can_id_val_as;
                dbc_api.ivt_msg_cmd.val_17_set_can_id_sn_wh = msg->val_17_set_can_id_sn_wh;
                dbc_api.ivt_msg_cmd.val_17_set_can_id_val_wh = msg->val_17_set_can_id_val_wh;
                dbc_api.ivt_msg_cmd.val_1d_set_can_id_sn_command = msg->val_1d_set_can_id_sn_command;
                dbc_api.ivt_msg_cmd.val_1d_set_can_id_val_command = msg->val_1d_set_can_id_val_command;
                dbc_api.ivt_msg_cmd.val_1f_set_can_id_sn_response = msg->val_1f_set_can_id_sn_response;
                dbc_api.ivt_msg_cmd.val_1f_set_can_id_val_response = msg->val_1f_set_can_id_val_response;
                dbc_api.ivt_msg_cmd.val_20_conf_cycletime_i = msg->val_20_conf_cycletime_i;
                dbc_api.ivt_msg_cmd.val_20_conf_endianess_i = msg->val_20_conf_endianess_i;
                dbc_api.ivt_msg_cmd.val_20_conf_sign_i = msg->val_20_conf_sign_i;
                dbc_api.ivt_msg_cmd.val_20_conf_triggermode_i = msg->val_20_conf_triggermode_i;
                dbc_api.ivt_msg_cmd.val_21_conf_cycletime_u1 = msg->val_21_conf_cycletime_u1;
                dbc_api.ivt_msg_cmd.val_21_conf_endianess_u1 = msg->val_21_conf_endianess_u1;
                dbc_api.ivt_msg_cmd.val_21_conf_sign_u1 = msg->val_21_conf_sign_u1;
                dbc_api.ivt_msg_cmd.val_21_conf_triggermode_u1 = msg->val_21_conf_triggermode_u1;
                dbc_api.ivt_msg_cmd.val_22_conf_cycletime_u2 = msg->val_22_conf_cycletime_u2;
                dbc_api.ivt_msg_cmd.val_22_conf_endianess_u2 = msg->val_22_conf_endianess_u2;
                dbc_api.ivt_msg_cmd.val_22_conf_sign_u2 = msg->val_22_conf_sign_u2;
                dbc_api.ivt_msg_cmd.val_22_conf_triggermode_u2 = msg->val_22_conf_triggermode_u2;
                dbc_api.ivt_msg_cmd.val_23_conf_cycletime_u3 = msg->val_23_conf_cycletime_u3;
                dbc_api.ivt_msg_cmd.val_23_conf_endianess_u3 = msg->val_23_conf_endianess_u3;
                dbc_api.ivt_msg_cmd.val_23_conf_sign_u3 = msg->val_23_conf_sign_u3;
                dbc_api.ivt_msg_cmd.val_23_conf_triggermode_u3 = msg->val_23_conf_triggermode_u3;
                dbc_api.ivt_msg_cmd.val_24_conf_cycletime_t = msg->val_24_conf_cycletime_t;
                dbc_api.ivt_msg_cmd.val_24_conf_endianess_t = msg->val_24_conf_endianess_t;
                dbc_api.ivt_msg_cmd.val_24_conf_sign_t = msg->val_24_conf_sign_t;
                dbc_api.ivt_msg_cmd.val_24_conf_triggermode_t = msg->val_24_conf_triggermode_t;
                dbc_api.ivt_msg_cmd.val_25_conf_cycletime_w = msg->val_25_conf_cycletime_w;
                dbc_api.ivt_msg_cmd.val_25_conf_endianess_w = msg->val_25_conf_endianess_w;
                dbc_api.ivt_msg_cmd.val_25_conf_sign_w = msg->val_25_conf_sign_w;
                dbc_api.ivt_msg_cmd.val_25_conf_triggermode_w = msg->val_25_conf_triggermode_w;
                dbc_api.ivt_msg_cmd.val_26_conf_cycletime_as = msg->val_26_conf_cycletime_as;
                dbc_api.ivt_msg_cmd.val_26_conf_endianess_as = msg->val_26_conf_endianess_as;
                dbc_api.ivt_msg_cmd.val_26_conf_sign_as = msg->val_26_conf_sign_as;
                dbc_api.ivt_msg_cmd.val_26_conf_triggermode_as = msg->val_26_conf_triggermode_as;
                dbc_api.ivt_msg_cmd.val_27_conf_cycletime_wh = msg->val_27_conf_cycletime_wh;
                dbc_api.ivt_msg_cmd.val_27_conf_endianess_wh = msg->val_27_conf_endianess_wh;
                dbc_api.ivt_msg_cmd.val_27_conf_sign_wh = msg->val_27_conf_sign_wh;
                dbc_api.ivt_msg_cmd.val_27_conf_triggermode_wh = msg->val_27_conf_triggermode_wh;
                dbc_api.ivt_msg_cmd.val_30_reset_device_sn = msg->val_30_reset_device_sn;
                dbc_api.ivt_msg_cmd.val_30_reset_item = msg->val_30_reset_item;
                dbc_api.ivt_msg_cmd.val_31_00_trigger_i = msg->val_31_00_trigger_i;
                dbc_api.ivt_msg_cmd.val_31_01_trigger_u1 = msg->val_31_01_trigger_u1;
                dbc_api.ivt_msg_cmd.val_31_02_trigger_u2 = msg->val_31_02_trigger_u2;
                dbc_api.ivt_msg_cmd.val_31_03_trigger_u3 = msg->val_31_03_trigger_u3;
                dbc_api.ivt_msg_cmd.val_31_04_trigger_t = msg->val_31_04_trigger_t;
                dbc_api.ivt_msg_cmd.val_31_05_trigger_w = msg->val_31_05_trigger_w;
                dbc_api.ivt_msg_cmd.val_31_06_trigger_as = msg->val_31_06_trigger_as;
                dbc_api.ivt_msg_cmd.val_31_07_trigger_wh = msg->val_31_07_trigger_wh;
                dbc_api.ivt_msg_cmd.val_31_trigger_spare = msg->val_31_trigger_spare;
                dbc_api.ivt_msg_cmd.val_32_store_dummy = msg->val_32_store_dummy;
                dbc_api.ivt_msg_cmd.val_33_oc_test_time = msg->val_33_oc_test_time;
                dbc_api.ivt_msg_cmd.val_34_actual_mode = msg->val_34_actual_mode;
                dbc_api.ivt_msg_cmd.val_34_code_level = msg->val_34_code_level;
                dbc_api.ivt_msg_cmd.val_34_startup_mode = msg->val_34_startup_mode;
                dbc_api.ivt_msg_cmd.val_35_oc_pos_reset_threshold = msg->val_35_oc_pos_reset_threshold;
                dbc_api.ivt_msg_cmd.val_35_oc_pos_set_threshold = msg->val_35_oc_pos_set_threshold;
                dbc_api.ivt_msg_cmd.val_36_oc_neg_reset_threshold = msg->val_36_oc_neg_reset_threshold;
                dbc_api.ivt_msg_cmd.val_36_oc_neg_set_threshold = msg->val_36_oc_neg_set_threshold;
                dbc_api.ivt_msg_cmd.val_3a_restart_to_bitrate = msg->val_3a_restart_to_bitrate;
                dbc_api.ivt_msg_cmd.val_3d_restart_default_dummy = msg->val_3d_restart_default_dummy;
                dbc_api.ivt_msg_cmd.val_3f_restart_dummy = msg->val_3f_restart_dummy;
                dbc_api.ivt_msg_cmd.val_40_get_measerror_item = msg->val_40_get_measerror_item;
                dbc_api.ivt_msg_cmd.val_41_get_systemerror_item = msg->val_41_get_systemerror_item;
                dbc_api.ivt_msg_cmd.val_42_get_alllogdata_item = msg->val_42_get_alllogdata_item;
                dbc_api.ivt_msg_cmd.val_43_get_logdata_item = msg->val_43_get_logdata_item;
                dbc_api.ivt_msg_cmd.val_50_get_can_id_sn_i = msg->val_50_get_can_id_sn_i;
                dbc_api.ivt_msg_cmd.val_51_get_can_id_sn_u1 = msg->val_51_get_can_id_sn_u1;
                dbc_api.ivt_msg_cmd.val_52_get_can_id_sn_u2 = msg->val_52_get_can_id_sn_u2;
                dbc_api.ivt_msg_cmd.val_53_get_can_id_sn_u3 = msg->val_53_get_can_id_sn_u3;
                dbc_api.ivt_msg_cmd.val_54_get_can_id_sn_t = msg->val_54_get_can_id_sn_t;
                dbc_api.ivt_msg_cmd.val_55_get_can_id_sn_w = msg->val_55_get_can_id_sn_w;
                dbc_api.ivt_msg_cmd.val_56_get_can_id_sn_as = msg->val_56_get_can_id_sn_as;
                dbc_api.ivt_msg_cmd.val_57_get_can_id_sn_wh = msg->val_57_get_can_id_sn_wh;
                dbc_api.ivt_msg_cmd.val_5d_get_can_id_sn_command = msg->val_5d_get_can_id_sn_command;
                dbc_api.ivt_msg_cmd.val_5f_get_can_id_sn_response = msg->val_5f_get_can_id_sn_response;
                dbc_api.ivt_msg_cmd.val_60_get_result_dummy_i = msg->val_60_get_result_dummy_i;
                dbc_api.ivt_msg_cmd.val_61_get_result_dummy_u1 = msg->val_61_get_result_dummy_u1;
                dbc_api.ivt_msg_cmd.val_62_get_result_dummy_u2 = msg->val_62_get_result_dummy_u2;
                dbc_api.ivt_msg_cmd.val_63_get_result_dummy_u3 = msg->val_63_get_result_dummy_u3;
                dbc_api.ivt_msg_cmd.val_64_get_result_dummy_t = msg->val_64_get_result_dummy_t;
                dbc_api.ivt_msg_cmd.val_65_get_result_dummy_w = msg->val_65_get_result_dummy_w;
                dbc_api.ivt_msg_cmd.val_66_get_result_dummy_as = msg->val_66_get_result_dummy_as;
                dbc_api.ivt_msg_cmd.val_67_get_result_dummy_wh = msg->val_67_get_result_dummy_wh;
                dbc_api.ivt_msg_cmd.val_73_get_oc_test_dummy = msg->val_73_get_oc_test_dummy;
                dbc_api.ivt_msg_cmd.val_74_get_mode_dummy = msg->val_74_get_mode_dummy;
                dbc_api.ivt_msg_cmd.val_75_get_oc_pos_dummy = msg->val_75_get_oc_pos_dummy;
                dbc_api.ivt_msg_cmd.val_76_get_oc_neg_dummy = msg->val_76_get_oc_neg_dummy;
                dbc_api.ivt_msg_cmd.val_79_get_device_id_dummy = msg->val_79_get_device_id_dummy;
                dbc_api.ivt_msg_cmd.val_7a_get_sw_version_dummy = msg->val_7a_get_sw_version_dummy;
                dbc_api.ivt_msg_cmd.val_7b_get_serien_nr_dummy = msg->val_7b_get_serien_nr_dummy;
                dbc_api.ivt_msg_cmd.val_7c_get_art_nr_dummy = msg->val_7c_get_art_nr_dummy;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::IvtMsgResponse>(
        "/can/dbc/ivt_msg_response", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::IvtMsgResponse> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_response.a0_resp_cycletime_i = msg->a0_resp_cycletime_i;
                dbc_api.ivt_msg_response.a0_resp_endianess_i = msg->a0_resp_endianess_i;
                dbc_api.ivt_msg_response.a0_resp_sign_i = msg->a0_resp_sign_i;
                dbc_api.ivt_msg_response.a0_resp_triggermode_i = msg->a0_resp_triggermode_i;
                dbc_api.ivt_msg_response.a1_resp_cycletime_u1 = msg->a1_resp_cycletime_u1;
                dbc_api.ivt_msg_response.a1_resp_endianess_u1 = msg->a1_resp_endianess_u1;
                dbc_api.ivt_msg_response.a1_resp_sign_u1 = msg->a1_resp_sign_u1;
                dbc_api.ivt_msg_response.a1_resp_triggermode_u1 = msg->a1_resp_triggermode_u1;
                dbc_api.ivt_msg_response.a2_resp_cycletime_u2 = msg->a2_resp_cycletime_u2;
                dbc_api.ivt_msg_response.a2_resp_endianess_u2 = msg->a2_resp_endianess_u2;
                dbc_api.ivt_msg_response.a2_resp_sign_u2 = msg->a2_resp_sign_u2;
                dbc_api.ivt_msg_response.a2_resp_triggermode_u2 = msg->a2_resp_triggermode_u2;
                dbc_api.ivt_msg_response.a3_resp_cycletime_u3 = msg->a3_resp_cycletime_u3;
                dbc_api.ivt_msg_response.a3_resp_endianess_u3 = msg->a3_resp_endianess_u3;
                dbc_api.ivt_msg_response.a3_resp_sign_u3 = msg->a3_resp_sign_u3;
                dbc_api.ivt_msg_response.a3_resp_triggermode_u3 = msg->a3_resp_triggermode_u3;
                dbc_api.ivt_msg_response.a4_resp_cycletime_t = msg->a4_resp_cycletime_t;
                dbc_api.ivt_msg_response.a4_resp_endianess_t = msg->a4_resp_endianess_t;
                dbc_api.ivt_msg_response.a4_resp_sign_t = msg->a4_resp_sign_t;
                dbc_api.ivt_msg_response.a4_resp_triggermode_t = msg->a4_resp_triggermode_t;
                dbc_api.ivt_msg_response.a5_resp_cycletime_w = msg->a5_resp_cycletime_w;
                dbc_api.ivt_msg_response.a5_resp_endianess_w = msg->a5_resp_endianess_w;
                dbc_api.ivt_msg_response.a5_resp_sign_w = msg->a5_resp_sign_w;
                dbc_api.ivt_msg_response.a5_resp_triggermode_w = msg->a5_resp_triggermode_w;
                dbc_api.ivt_msg_response.a6_resp_cycletime_as = msg->a6_resp_cycletime_as;
                dbc_api.ivt_msg_response.a6_resp_endianess_as = msg->a6_resp_endianess_as;
                dbc_api.ivt_msg_response.a6_resp_sign_as = msg->a6_resp_sign_as;
                dbc_api.ivt_msg_response.a6_resp_triggermode_as = msg->a6_resp_triggermode_as;
                dbc_api.ivt_msg_response.a7_resp_cycletime_wh = msg->a7_resp_cycletime_wh;
                dbc_api.ivt_msg_response.a7_resp_endianess_wh = msg->a7_resp_endianess_wh;
                dbc_api.ivt_msg_response.a7_resp_sign_wh = msg->a7_resp_sign_wh;
                dbc_api.ivt_msg_response.a7_resp_triggermode_wh = msg->a7_resp_triggermode_wh;
                dbc_api.ivt_msg_response.b0_resp_reset_device_sn = msg->b0_resp_reset_device_sn;
                dbc_api.ivt_msg_response.b0_resp_reset_item = msg->b0_resp_reset_item;
                dbc_api.ivt_msg_response.b1_00_trigger_i = msg->b1_00_trigger_i;
                dbc_api.ivt_msg_response.b1_01_trigger_u1 = msg->b1_01_trigger_u1;
                dbc_api.ivt_msg_response.b1_02_trigger_u2 = msg->b1_02_trigger_u2;
                dbc_api.ivt_msg_response.b1_03_trigger_u3 = msg->b1_03_trigger_u3;
                dbc_api.ivt_msg_response.b1_04_trigger_t = msg->b1_04_trigger_t;
                dbc_api.ivt_msg_response.b1_05_trigger_w = msg->b1_05_trigger_w;
                dbc_api.ivt_msg_response.b1_06_trigger_as = msg->b1_06_trigger_as;
                dbc_api.ivt_msg_response.b1_07_trigger_wh = msg->b1_07_trigger_wh;
                dbc_api.ivt_msg_response.b1_trigger_spare = msg->b1_trigger_spare;
                dbc_api.ivt_msg_response.b2_resp_store_device_sn = msg->b2_resp_store_device_sn;
                dbc_api.ivt_msg_response.b2_resp_store_dummy = msg->b2_resp_store_dummy;
                dbc_api.ivt_msg_response.b3_resp_oc_test_time = msg->b3_resp_oc_test_time;
                dbc_api.ivt_msg_response.b4_resp_actual_mode = msg->b4_resp_actual_mode;
                dbc_api.ivt_msg_response.b4_resp_code_level = msg->b4_resp_code_level;
                dbc_api.ivt_msg_response.b4_resp_startup_mode = msg->b4_resp_startup_mode;
                dbc_api.ivt_msg_response.b5_resp_oc_pos_reset_threshold = msg->b5_resp_oc_pos_reset_threshold;
                dbc_api.ivt_msg_response.b5_resp_oc_pos_set_threshold = msg->b5_resp_oc_pos_set_threshold;
                dbc_api.ivt_msg_response.b6_resp_oc_neg_reset_threshold = msg->b6_resp_oc_neg_reset_threshold;
                dbc_api.ivt_msg_response.b6_resp_oc_neg_set_threshold = msg->b6_resp_oc_neg_set_threshold;
                dbc_api.ivt_msg_response.b9_resp_device_com = msg->b9_resp_device_com;
                dbc_api.ivt_msg_response.b9_resp_device_current = msg->b9_resp_device_current;
                dbc_api.ivt_msg_response.b9_resp_device_toi = msg->b9_resp_device_toi;
                dbc_api.ivt_msg_response.b9_resp_device_type = msg->b9_resp_device_type;
                dbc_api.ivt_msg_response.b9_resp_device_v_supply = msg->b9_resp_device_v_supply;
                dbc_api.ivt_msg_response.b9_resp_device_voltage_chan = msg->b9_resp_device_voltage_chan;
                dbc_api.ivt_msg_response.ba_resp_sw_ver_day = msg->ba_resp_sw_ver_day;
                dbc_api.ivt_msg_response.ba_resp_sw_ver_internal = msg->ba_resp_sw_ver_internal;
                dbc_api.ivt_msg_response.ba_resp_sw_ver_major_hw = msg->ba_resp_sw_ver_major_hw;
                dbc_api.ivt_msg_response.ba_resp_sw_ver_minor = msg->ba_resp_sw_ver_minor;
                dbc_api.ivt_msg_response.ba_resp_sw_ver_month = msg->ba_resp_sw_ver_month;
                dbc_api.ivt_msg_response.ba_resp_sw_ver_revision = msg->ba_resp_sw_ver_revision;
                dbc_api.ivt_msg_response.ba_resp_sw_ver_vear = msg->ba_resp_sw_ver_vear;
                dbc_api.ivt_msg_response.bb_resp_serien_nr = msg->bb_resp_serien_nr;
                dbc_api.ivt_msg_response.bc_resp_art_n = msg->bc_resp_art_n;
                dbc_api.ivt_msg_response.bf_restart_alive_cmd_id = msg->bf_restart_alive_cmd_id;
                dbc_api.ivt_msg_response.bf_restart_alive_sn = msg->bf_restart_alive_sn;
                dbc_api.ivt_msg_response.ff_wrong_command = msg->ff_wrong_command;
                dbc_api.ivt_msg_response.ivt_id_response = msg->ivt_id_response;
                dbc_api.ivt_msg_response.val_80_resp_measerror_count_mask = msg->val_80_resp_measerror_count_mask;
                dbc_api.ivt_msg_response.val_80_resp_measerror_item = msg->val_80_resp_measerror_item;
                dbc_api.ivt_msg_response.val_81_resp_systemerror_count_mask = msg->val_81_resp_systemerror_count_mask;
                dbc_api.ivt_msg_response.val_81_resp_systemerror_item = msg->val_81_resp_systemerror_item;
                dbc_api.ivt_msg_response.val_82_resp_alllogdata_counter = msg->val_82_resp_alllogdata_counter;
                dbc_api.ivt_msg_response.val_82_resp_alllogdata_item = msg->val_82_resp_alllogdata_item;
                dbc_api.ivt_msg_response.val_83_resp_logdata_counter = msg->val_83_resp_logdata_counter;
                dbc_api.ivt_msg_response.val_83_resp_logdata_item = msg->val_83_resp_logdata_item;
                dbc_api.ivt_msg_response.val_90_resp_can_id_sn_i = msg->val_90_resp_can_id_sn_i;
                dbc_api.ivt_msg_response.val_90_resp_can_id_val_i = msg->val_90_resp_can_id_val_i;
                dbc_api.ivt_msg_response.val_91_resp_can_id_sn_u1 = msg->val_91_resp_can_id_sn_u1;
                dbc_api.ivt_msg_response.val_91_resp_can_id_val_u1 = msg->val_91_resp_can_id_val_u1;
                dbc_api.ivt_msg_response.val_92_resp_can_id_sn_u2 = msg->val_92_resp_can_id_sn_u2;
                dbc_api.ivt_msg_response.val_92_resp_can_id_val_u2 = msg->val_92_resp_can_id_val_u2;
                dbc_api.ivt_msg_response.val_93_resp_can_id_sn_u3 = msg->val_93_resp_can_id_sn_u3;
                dbc_api.ivt_msg_response.val_93_resp_can_id_val_u3 = msg->val_93_resp_can_id_val_u3;
                dbc_api.ivt_msg_response.val_94_resp_can_id_sn_t = msg->val_94_resp_can_id_sn_t;
                dbc_api.ivt_msg_response.val_94_resp_can_id_val_t = msg->val_94_resp_can_id_val_t;
                dbc_api.ivt_msg_response.val_95_resp_can_id_sn_w = msg->val_95_resp_can_id_sn_w;
                dbc_api.ivt_msg_response.val_95_resp_can_id_val_w = msg->val_95_resp_can_id_val_w;
                dbc_api.ivt_msg_response.val_96_resp_can_id_sn_as = msg->val_96_resp_can_id_sn_as;
                dbc_api.ivt_msg_response.val_96_resp_can_id_val_as = msg->val_96_resp_can_id_val_as;
                dbc_api.ivt_msg_response.val_97_resp_can_id_sn_wh = msg->val_97_resp_can_id_sn_wh;
                dbc_api.ivt_msg_response.val_97_resp_can_id_val_wh = msg->val_97_resp_can_id_val_wh;
                dbc_api.ivt_msg_response.val_9d_resp_can_id_sn_command = msg->val_9d_resp_can_id_sn_command;
                dbc_api.ivt_msg_response.val_9d_resp_can_id_val_command = msg->val_9d_resp_can_id_val_command;
                dbc_api.ivt_msg_response.val_9f_resp_can_id_sn_response = msg->val_9f_resp_can_id_sn_response;
                dbc_api.ivt_msg_response.val_9f_resp_can_id_val_response = msg->val_9f_resp_can_id_val_response;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::IvtMsgResultAs>(
        "/can/dbc/ivt_msg_result_as", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::IvtMsgResultAs> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.ivt_msg_result_as.ivt_id_result_as = msg->ivt_id_result_as;
                dbc_api.ivt_msg_result_as.ivt_msgcount_result_as = msg->ivt_msgcount_result_as;
                dbc_api.ivt_msg_result_as.ivt_result_as = msg->ivt_result_as;
                dbc_api.ivt_msg_result_as.ivt_result_as_channel_error = msg->ivt_result_as_channel_error;
                dbc_api.ivt_msg_result_as.ivt_result_as_measurement_error = msg->ivt_result_as_measurement_error;
                dbc_api.ivt_msg_result_as.ivt_result_as_ocs = msg->ivt_result_as_ocs;
                dbc_api.ivt_msg_result_as.ivt_result_as_system_error = msg->ivt_result_as_system_error;
            }
        }));
}
#endif
#endif
