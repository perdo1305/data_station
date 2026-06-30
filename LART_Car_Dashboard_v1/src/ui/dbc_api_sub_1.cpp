// Auto-generated from DBC files by generate_dbc_api.py. Do not edit.
#include "dbc_api.h"
#if defined(LART_UI_HAVE_RCLCPP) && LART_UI_HAVE_RCLCPP
#if defined(LART_HAVE_LART_MSGS) && LART_HAVE_LART_MSGS
#include <rclcpp/rclcpp.hpp>
#include <mutex>
#include <vector>
extern std::mutex dbc_api_mutex;
#include <lart_msgs/msg/hv500_setdriveenable.hpp>
#include <lart_msgs/msg/hv500_seterpm.hpp>
#include <lart_msgs/msg/hv500_setmaxacbrakecurrent.hpp>
#include <lart_msgs/msg/hv500_setmaxaccurrent.hpp>
#include <lart_msgs/msg/hv500_setmaxdcbrakecurrent.hpp>
#include <lart_msgs/msg/hv500_setmaxdccurrent.hpp>
#include <lart_msgs/msg/hv500_setposition.hpp>
#include <lart_msgs/msg/hv500_setrelbrakecurrent.hpp>
#include <lart_msgs/msg/hv500_setrelcurrent.hpp>
#include <lart_msgs/msg/hv500_targetiq.hpp>
#include <lart_msgs/msg/hv500_temperatures.hpp>
#include <lart_msgs/msg/ivt_msg_cmd.hpp>
#include <lart_msgs/msg/ivt_msg_response.hpp>
#include <lart_msgs/msg/ivt_msg_result_as.hpp>
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
#include <lart_msgs/msg/pedal_box.hpp>
#include <lart_msgs/msg/rear_wheel_l.hpp>

void init_dbc_api_subscribers_chunk_1(std::shared_ptr<rclcpp::Node> node, std::vector<rclcpp::SubscriptionBase::SharedPtr>& subs) {
    auto sensor_qos = rclcpp::QoS(10).best_effort();

    subs.push_back(node->create_subscription<lart_msgs::msg::Hv500Setdriveenable>(
        "/can/dbc/hv500_setdriveenable", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Hv500Setdriveenable> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_setdriveenable.cmd_driveenable = msg->cmd_driveenable;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Hv500Seterpm>(
        "/can/dbc/hv500_seterpm", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Hv500Seterpm> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_seterpm.cmd_targetspeed = msg->cmd_targetspeed;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Hv500Setmaxacbrakecurrent>(
        "/can/dbc/hv500_setmaxacbrakecurrent", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Hv500Setmaxacbrakecurrent> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_setmaxacbrakecurrent.cmd_maxacbrakecurrent = msg->cmd_maxacbrakecurrent;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Hv500Setmaxaccurrent>(
        "/can/dbc/hv500_setmaxaccurrent", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Hv500Setmaxaccurrent> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_setmaxaccurrent.cmd_maxaccurrent = msg->cmd_maxaccurrent;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Hv500Setmaxdcbrakecurrent>(
        "/can/dbc/hv500_setmaxdcbrakecurrent", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Hv500Setmaxdcbrakecurrent> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_setmaxdcbrakecurrent.cmd_maxdcbrakecurrent = msg->cmd_maxdcbrakecurrent;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Hv500Setmaxdccurrent>(
        "/can/dbc/hv500_setmaxdccurrent", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Hv500Setmaxdccurrent> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_setmaxdccurrent.cmd_maxdccurrent = msg->cmd_maxdccurrent;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Hv500Setposition>(
        "/can/dbc/hv500_setposition", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Hv500Setposition> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_setposition.cmd_targetposition = msg->cmd_targetposition;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Hv500Setrelbrakecurrent>(
        "/can/dbc/hv500_setrelbrakecurrent", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Hv500Setrelbrakecurrent> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_setrelbrakecurrent.cmd_targerelativebrakecurrent = msg->cmd_targerelativebrakecurrent;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Hv500Setrelcurrent>(
        "/can/dbc/hv500_setrelcurrent", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Hv500Setrelcurrent> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_setrelcurrent.cmd_targetrelativecurrent = msg->cmd_targetrelativecurrent;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Hv500Targetiq>(
        "/can/dbc/hv500_targetiq", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Hv500Targetiq> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_targetiq.controlmode = msg->controlmode;
                dbc_api.hv500_targetiq.ismotorstill = msg->ismotorstill;
                dbc_api.hv500_targetiq.motorposition = msg->motorposition;
                dbc_api.hv500_targetiq.targetiq = msg->targetiq;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::Hv500Temperatures>(
        "/can/dbc/hv500_temperatures", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::Hv500Temperatures> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.hv500_temperatures.actual_faultcode = msg->actual_faultcode;
                dbc_api.hv500_temperatures.actual_tempcontroller = msg->actual_tempcontroller;
                dbc_api.hv500_temperatures.actual_tempmotor = msg->actual_tempmotor;
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
    subs.push_back(node->create_subscription<lart_msgs::msg::PedalBox>(
        "/can/dbc/pedal_box", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::PedalBox> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.pedal_box.apps1 = msg->apps1;
                dbc_api.pedal_box.apps2 = msg->apps2;
            }
        }));
    subs.push_back(node->create_subscription<lart_msgs::msg::RearWheelL>(
        "/can/dbc/rear_wheel_l", sensor_qos, [](const std::shared_ptr<lart_msgs::msg::RearWheelL> msg) {
            if (msg) {
                std::lock_guard<std::mutex> lock(dbc_api_mutex);
                dbc_api.rear_wheel_l.ignition = msg->ignition;
                dbc_api.rear_wheel_l.mission_select = msg->mission_select;
                dbc_api.rear_wheel_l.shutdown_circuit = msg->shutdown_circuit;
            }
        }));
}
#endif
#endif
