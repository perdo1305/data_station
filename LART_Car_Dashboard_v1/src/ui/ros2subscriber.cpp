#include "ros2subscriber.h"
#include "dbc_api.h"

#include <atomic>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <strings.h>
#include <memory>
#include <mutex>
#include <vector>

#ifndef LART_UI_HAVE_RCLCPP
#define LART_UI_HAVE_RCLCPP 0
#endif

#if defined(__GNUC__) || defined(__clang__)
#define LART_WEAK __attribute__((weak))
#else
#define LART_WEAK
#endif

#if LART_UI_HAVE_RCLCPP

#include <rclcpp/rclcpp.hpp>
#include <rclcpp/executors/single_threaded_executor.hpp>
#include <rclcpp/qos.hpp>
#include <std_msgs/msg/float32.hpp>
#include <std_msgs/msg/int32.hpp>
#include <std_msgs/msg/string.hpp>

#if !defined(LART_HAVE_DASHBOARD_STATE_MSG)
#if defined(__has_include)
#if __has_include(<lart_msgs/msg/dashboard_state.hpp>)
#define LART_HAVE_DASHBOARD_STATE_MSG 1
#else
#define LART_HAVE_DASHBOARD_STATE_MSG 0
#endif
#else
#define LART_HAVE_DASHBOARD_STATE_MSG 0
#endif
#endif

#if LART_HAVE_DASHBOARD_STATE_MSG
#include <lart_msgs/msg/dashboard_state.hpp>
#endif

#if !defined(LART_HAVE_CAN_FRAME_MSG)
#if defined(__has_include)
#if __has_include(<lart_msgs/msg/can_frame.hpp>)
#define LART_HAVE_CAN_FRAME_MSG 1
#else
#define LART_HAVE_CAN_FRAME_MSG 0
#endif
#else
#define LART_HAVE_CAN_FRAME_MSG 0
#endif
#endif

#if LART_HAVE_CAN_FRAME_MSG
#include <lart_msgs/msg/can_frame.hpp>
#endif

namespace {
constexpr const char *DEFAULT_SPEED_TOPIC = "/can/dbc/dv_dynamics_1/speed_actual";
constexpr const char *DEFAULT_HV_TOPIC = "/can/dbc/vcu_hv/hv";
constexpr const char *DEFAULT_DASHBOARD_STATE_TOPIC = "/vehicle/dashboard_state";
constexpr const char *DEFAULT_SET_SCREEN_TOPIC = "/dashboard/set_screen";

std::atomic<int> g_is_initialized(0);
std::atomic<int> g_has_speed(0);
std::atomic<float> g_latest_speed_kph(0.0f);
std::atomic<int> g_has_hv(0);
std::atomic<float> g_latest_hv(0.0f);
std::atomic<int> g_screen_change_requested(0);
std::atomic<int> g_requested_screen_id(0);

std::mutex g_telemetry_mutex;
TelemetryData g_telemetry;
std::vector<rclcpp::SubscriptionBase::SharedPtr> g_subs;

std::mutex g_can_log_mutex;
std::vector<std::string> g_can_log_frames;

bool g_did_init_rclcpp = false;
std::shared_ptr<rclcpp::Node> g_node;
std::shared_ptr<rclcpp::executors::SingleThreadedExecutor> g_exec;
rclcpp::SubscriptionBase::SharedPtr g_sub;
rclcpp::SubscriptionBase::SharedPtr g_sub_hv;
rclcpp::SubscriptionBase::SharedPtr g_sub_screen;
rclcpp::SubscriptionBase::SharedPtr g_sub_can_frames;
rclcpp::SubscriptionBase::SharedPtr g_sub_notifications;
rclcpp::SubscriptionBase::SharedPtr g_sub_notification_ack;
rclcpp::Publisher<std_msgs::msg::String>::SharedPtr g_pub_notification_ack;

const char *env_or_default(const char *name, const char *fallback) {
    const char *value = std::getenv(name);
    if (value == nullptr || *value == '\0') {
        return fallback;
    }
    return value;
}
}  // namespace

LART_WEAK int ros2subscriber_init(void) {
    if (g_is_initialized.load()) {
        return 0;
    }

    if (!rclcpp::ok()) {
        g_did_init_rclcpp = true;
        int argc = 0;
        char **argv = nullptr;
        rclcpp::init(argc, argv);
    }

    g_node = std::make_shared<rclcpp::Node>("lart_dashboard_listener");
    g_exec = std::make_shared<rclcpp::executors::SingleThreadedExecutor>();
    g_exec->add_node(g_node);

    const char *topic = env_or_default("LART_ROS2_SPEED_TOPIC", DEFAULT_SPEED_TOPIC);
    const char *hv_topic = env_or_default("LART_ROS2_HV_TOPIC", DEFAULT_HV_TOPIC);

    auto callback = [](const std_msgs::msg::Float32::SharedPtr msg) {
        if (msg) {
            g_latest_speed_kph.store(msg->data);
            g_has_speed.store(1);
        }
    };

    auto hv_callback = [](const std_msgs::msg::Float32::SharedPtr msg) {
        if (msg) {
            g_latest_hv.store(msg->data);
            g_has_hv.store(1);
        }
    };

    // Use BEST_EFFORT to match the can_bridge publisher's QoS profile
    auto sensor_qos = rclcpp::QoS(10).best_effort();

    g_sub = g_node->create_subscription<std_msgs::msg::Float32>(topic, sensor_qos, callback);
    g_sub_hv = g_node->create_subscription<std_msgs::msg::Float32>(hv_topic, sensor_qos, hv_callback);

    // Screen change subscription
    const char *screen_topic = env_or_default("LART_ROS2_SET_SCREEN_TOPIC", DEFAULT_SET_SCREEN_TOPIC);

    auto screen_callback = [](const std_msgs::msg::Int32::SharedPtr msg) {
        if (msg) {
            int id = msg->data;
            // Valid 0-based screen range: 0=Driver View, 1=Autonomous, 2-6=Debug 1-5, 7-11=Debug Autonomous 1-5
            if (id < 0 || id > 11) {
                return;  // Ignore out-of-range IDs silently
            }
            g_requested_screen_id.store(id);
            g_screen_change_requested.store(1);
        }
    };

    g_sub_screen = g_node->create_subscription<std_msgs::msg::Int32>(screen_topic, rclcpp::QoS(10), screen_callback);

    // Direct notification subscription
    auto notifications_callback = [](const std_msgs::msg::String::SharedPtr msg) {
        if (msg) {
            ui_add_notification(msg->data.c_str(), "System Alert", msg->data.c_str());
        }
    };
    g_sub_notifications = g_node->create_subscription<std_msgs::msg::String>(
        "/vehicle/notifications", rclcpp::QoS(10), notifications_callback);

    // Notification ACK subscription
    auto notification_ack_callback = [](const std_msgs::msg::String::SharedPtr msg) {
        if (msg) {
            if (msg->data.empty() || msg->data == "all") {
                ui_clear_all_notifications();
            } else {
                ui_clear_notification(msg->data.c_str());
            }
        }
    };
    g_sub_notification_ack = g_node->create_subscription<std_msgs::msg::String>(
        "/vehicle/notification_ack", rclcpp::QoS(10), notification_ack_callback);

    g_pub_notification_ack = g_node->create_publisher<std_msgs::msg::String>(
        "/vehicle/notification_ack", rclcpp::QoS(10));

#if LART_HAVE_CAN_FRAME_MSG
    auto can_frame_callback = [](const lart_msgs::msg::CanFrame::SharedPtr msg) {
        if (msg) {
            std::lock_guard<std::mutex> lock(g_can_log_mutex);
            char buf[128];
            int offset = snprintf(buf, sizeof(buf), "0x%03X [%u]", msg->can_id, (unsigned int)msg->dlc);
            for (size_t i = 0; i < msg->dlc && i < msg->data.size() && offset < (int)sizeof(buf) - 3; ++i) {
                offset += snprintf(buf + offset, sizeof(buf) - offset, " %02X", msg->data[i]);
            }
            g_can_log_frames.push_back(std::string(buf));
            if (g_can_log_frames.size() > 8) {
                g_can_log_frames.erase(g_can_log_frames.begin());
            }
        }
    };
    g_sub_can_frames = g_node->create_subscription<lart_msgs::msg::CanFrame>("/can/frames", sensor_qos, can_frame_callback);
#endif

    // Setup dynamic high-density telemetry subscriptions
#define SUB_FLOAT_MAP(field, dbcapi_field, topic_name) \
    g_subs.push_back(g_node->create_subscription<std_msgs::msg::Float32>( \
        topic_name, sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) { \
            if (msg) { \
                std::lock_guard<std::mutex> lock(g_telemetry_mutex); \
                g_telemetry.field = msg->data; \
                dbc_api.dbcapi_field = msg->data; \
            } \
        }))

    // AMS Status
    SUB_FLOAT_MAP(ams_state, master_msc_id_1.master_state, "/can/dbc/master_msc_id_1/master_state");
    SUB_FLOAT_MAP(ams_soc, master_soc_accumulator.soc_float, "/can/dbc/master_soc_accumulator/soc_float");
    SUB_FLOAT_MAP(ams_runtime, master_msc_id_1.ams_current_draw, "/can/dbc/master_msc_id_1/ams_current_draw");
    SUB_FLOAT_MAP(ams_fans, master_msc_id_1.master_fan_pwm, "/can/dbc/master_msc_id_1/master_fan_pwm");
    SUB_FLOAT_MAP(ams_mcu_vref, master_msc_id_1.mcu_vref, "/can/dbc/master_msc_id_1/mcu_vref");
    SUB_FLOAT_MAP(ams_mcu_temp, master_msc_id_1.mcu_temperature, "/can/dbc/master_msc_id_1/mcu_temperature");
    SUB_FLOAT_MAP(ams_pec_err, master_msc_id_1.adbms_pec_error, "/can/dbc/master_msc_id_1/adbms_pec_error");
    SUB_FLOAT_MAP(ams_fault_cnt, master_msc_id_1.fault_counter, "/can/dbc/master_msc_id_1/fault_counter");
    SUB_FLOAT_MAP(ams_slaves, master_msc_id_4.slaves_detected, "/can/dbc/master_msc_id_4/slaves_detected");
    SUB_FLOAT_MAP(ams_fw, master_msc_id_1.master_firmware_version, "/can/dbc/master_msc_id_1/master_firmware_version");

    // Precharge
    SUB_FLOAT_MAP(precharge_state, master_precharge_id_1.precharge_state, "/can/dbc/master_precharge_id_1/precharge_state");
    SUB_FLOAT_MAP(air_pos, master_precharge_id_1.precharge_ctc_air_pos_state, "/can/dbc/master_precharge_id_1/precharge_ctc_air_pos_state");
    SUB_FLOAT_MAP(air_min, master_precharge_id_1.precharge_ctc_air_min_state, "/can/dbc/master_precharge_id_1/precharge_ctc_air_min_state");
    SUB_FLOAT_MAP(ctc_charge, master_precharge_id_1.precharge_ctc_charge_state, "/can/dbc/master_precharge_id_1/precharge_ctc_charge_state");
    SUB_FLOAT_MAP(ctc_discharge, master_precharge_id_1.precharge_ctc_discharge_state, "/can/dbc/master_precharge_id_1/precharge_ctc_discharge_state");

    // Inverter DTI HV500
    SUB_FLOAT_MAP(inv_erpm, hv500_erpm_duty_voltage.actual_erpm, "/can/dbc/hv500_erpm_duty_voltage/actual_erpm");
    SUB_FLOAT_MAP(inv_duty, hv500_erpm_duty_voltage.actual_duty, "/can/dbc/hv500_erpm_duty_voltage/actual_duty");
    SUB_FLOAT_MAP(inv_vin, hv500_erpm_duty_voltage.actual_inputvoltage, "/can/dbc/hv500_erpm_duty_voltage/actual_inputvoltage");
    SUB_FLOAT_MAP(inv_fault, hv500_temperatures.actual_faultcode, "/can/dbc/hv500_temperatures/actual_faultcode");
    SUB_FLOAT_MAP(inv_ac_curr, hv500_ac_dc_current.actual_accurrent, "/can/dbc/hv500_ac_dc_current/actual_accurrent");
    SUB_FLOAT_MAP(inv_dc_curr, hv500_ac_dc_current.actual_dccurrent, "/can/dbc/hv500_ac_dc_current/actual_dccurrent");
    SUB_FLOAT_MAP(inv_temp_ctrl, hv500_temperatures.actual_tempcontroller, "/can/dbc/hv500_temperatures/actual_tempcontroller");
    SUB_FLOAT_MAP(inv_temp_mot, hv500_temperatures.actual_tempmotor, "/can/dbc/hv500_temperatures/actual_tempmotor");
    SUB_FLOAT_MAP(inv_throttle, hv500_misc.actual_throttle, "/can/dbc/hv500_misc/actual_throttle");
    SUB_FLOAT_MAP(inv_brake, hv500_misc.actual_brake, "/can/dbc/hv500_misc/actual_brake");
    SUB_FLOAT_MAP(inv_foc_id, hv500_foc.actual_foc_id, "/can/dbc/hv500_foc/actual_foc_id");
    SUB_FLOAT_MAP(inv_foc_iq, hv500_foc.actual_foc_iq, "/can/dbc/hv500_foc/actual_foc_iq");
    SUB_FLOAT_MAP(inv_drive_en, hv500_misc.drive_enable, "/can/dbc/hv500_misc/drive_enable");

    // IVT
    SUB_FLOAT_MAP(ivt_current, ivt_msg_result_i.ivt_result_i, "/can/dbc/ivt_msg_result_i/ivt_result_i");
    SUB_FLOAT_MAP(ivt_u1, ivt_msg_result_u1.ivt_result_u1, "/can/dbc/ivt_msg_result_u1/ivt_result_u1");
    SUB_FLOAT_MAP(ivt_u2, ivt_msg_result_u2.ivt_result_u2, "/can/dbc/ivt_msg_result_u2/ivt_result_u2");
    SUB_FLOAT_MAP(ivt_u3, ivt_msg_result_u3.ivt_result_u3, "/can/dbc/ivt_msg_result_u3/ivt_result_u3");
    SUB_FLOAT_MAP(ivt_temp, ivt_msg_result_t.ivt_result_t, "/can/dbc/ivt_msg_result_t/ivt_result_t");
    SUB_FLOAT_MAP(ivt_power, ivt_msg_result_w.ivt_result_w, "/can/dbc/ivt_msg_result_w/ivt_result_w");
    SUB_FLOAT_MAP(ivt_energy, ivt_msg_result_wh.ivt_result_wh, "/can/dbc/ivt_msg_result_wh/ivt_result_wh");

    // Pedals
    SUB_FLOAT_MAP(apps1, pedal_box.apps1, "/can/dbc/pedal_box/apps1");
    SUB_FLOAT_MAP(apps2, pedal_box.apps2, "/can/dbc/pedal_box/apps2");

    // Wheels (AQT2/3/5/6)
    SUB_FLOAT_MAP(spd_fl, aqt2.wheel_spd, "/can/dbc/aqt2/wheel_spd");
    SUB_FLOAT_MAP(temp_fl, aqt2.tire_temp, "/can/dbc/aqt2/tire_temp");
    SUB_FLOAT_MAP(brk_fl, aqt2.brake_temp, "/can/dbc/aqt2/brake_temp");

    SUB_FLOAT_MAP(spd_fr, aqt3.wheel_spd, "/can/dbc/aqt3/wheel_spd");
    SUB_FLOAT_MAP(temp_fr, aqt3.tire_temp, "/can/dbc/aqt3/tire_temp");
    SUB_FLOAT_MAP(brk_fr, aqt3.brake_temp, "/can/dbc/aqt3/brake_temp");

    // SUB_FLOAT_MAP(spd_rl, aqt5.spd_wheel, "/can/dbc/aqt5/spd_wheel");
    SUB_FLOAT_MAP(temp_rl, aqt5.tire_temp, "/can/dbc/aqt5/tire_temp");
    SUB_FLOAT_MAP(brk_rl, aqt5.brake_temp, "/can/dbc/aqt5/brake_temp");

    // SUB_FLOAT_MAP(spd_rr, aqt6.spd_wheel, "/can/dbc/aqt6/spd_wheel");
    SUB_FLOAT_MAP(temp_rr, aqt6.tire_temp, "/can/dbc/aqt6/tire_temp");
    SUB_FLOAT_MAP(brk_rr, aqt6.brake_temp, "/can/dbc/aqt6/brake_temp");

    // Rear Sensors (AQT8 and AQT7)
    SUB_FLOAT_MAP(ntc1, aqt8.ntc1, "/can/dbc/aqt8/ntc1");
    SUB_FLOAT_MAP(ntc2, aqt8.ntc2, "/can/dbc/aqt8/ntc2");
    SUB_FLOAT_MAP(ntc3, aqt8.ntc3, "/can/dbc/aqt8/ntc3");
    SUB_FLOAT_MAP(susp_r, aqt7.susp_r, "/can/dbc/aqt7/susp_r");
    SUB_FLOAT_MAP(susp_l, aqt7.susp_l, "/can/dbc/aqt7/susp_l");

    // Autonomous: ACU
    SUB_FLOAT_MAP(acu_state, acu.acu_state, "/can/dbc/acu/acu_state");
    SUB_FLOAT_MAP(acu_as_state, acu.as_state, "/can/dbc/acu/as_state");
    SUB_FLOAT_MAP(acu_assi_state, acu.assi_state, "/can/dbc/acu/assi_state");
    SUB_FLOAT_MAP(acu_mission, acu.mission_select, "/can/dbc/acu/mission_select");
    SUB_FLOAT_MAP(acu_asms, acu.asms, "/can/dbc/acu/asms");
    SUB_FLOAT_MAP(acu_ign, acu.ign, "/can/dbc/acu/ign");
    SUB_FLOAT_MAP(acu_emergency, acu.emergency, "/can/dbc/acu/emergency");
    SUB_FLOAT_MAP(acu_emerg_cause, acu.emergency_cause, "/can/dbc/acu/emergency_cause");
    SUB_FLOAT_MAP(acu_cpu_temp, acu.acu_cpu_temp, "/can/dbc/acu/acu_cpu_temp");

    // Jetson
    SUB_FLOAT_MAP(jetson_as_state, jetson.as_state, "/can/dbc/jetson/as_state");
    SUB_FLOAT_MAP(jetson_mission, jetson.as_mission, "/can/dbc/jetson/as_mission");
    SUB_FLOAT_MAP(jetson_temp, jetson.temperature, "/can/dbc/jetson/temperature");
    SUB_FLOAT_MAP(jetson_cpu, jetson.cpu, "/can/dbc/jetson/cpu");
    SUB_FLOAT_MAP(jetson_gpu, jetson.gpu, "/can/dbc/jetson/gpu");
    SUB_FLOAT_MAP(jetson_emerg_cause, jetson.emergency_cause, "/can/dbc/jetson/emergency_cause");

    // VCU
    SUB_FLOAT_MAP(vcu_ign_man, vcu_ign_r2d.ignition_manual, "/can/dbc/vcu_ign_r2d/ignition_manual");
    SUB_FLOAT_MAP(vcu_ign_auto, vcu_ign_r2d.ignition_auto, "/can/dbc/vcu_ign_r2d/ignition_auto");
    SUB_FLOAT_MAP(vcu_r2d_man, vcu_ign_r2d.r2d_manual, "/can/dbc/vcu_ign_r2d/r2d_manual");
    SUB_FLOAT_MAP(vcu_r2d_auto, vcu_ign_r2d.r2d_auto, "/can/dbc/vcu_ign_r2d/r2d_auto");
    SUB_FLOAT_MAP(vcu_shutdown, vcu_ign_r2d.shutdown_signal, "/can/dbc/vcu_ign_r2d/shutdown_signal");
    SUB_FLOAT_MAP(vcu_state, vcu_ign_r2d.vcu_state, "/can/dbc/vcu_ign_r2d/vcu_state");
    SUB_FLOAT_MAP(vcu_r2d_raw, vcu_ign_r2d.r2d_button_raw, "/can/dbc/vcu_ign_r2d/r2d_button_raw");
    SUB_FLOAT_MAP(vcu_ign_raw, vcu_ign_r2d.ignition_switch_raw, "/can/dbc/vcu_ign_r2d/ignition_switch_raw");
    SUB_FLOAT_MAP(vcu_hv, vcu_hv.hv, "/can/dbc/vcu_hv/hv");
    SUB_FLOAT_MAP(vcu_brkf, vcu_hv.brake_pressure_front, "/can/dbc/vcu_hv/brake_pressure_front");
    SUB_FLOAT_MAP(vcu_brkr, vcu_hv.brake_pressure_rear, "/can/dbc/vcu_hv/brake_pressure_rear");
    SUB_FLOAT_MAP(vcu_rpm_act, vcu_rpm.motor_rpm_left, "/can/dbc/vcu_rpm/motor_rpm_left");
    SUB_FLOAT_MAP(vcu_rpm_tgt, vcu_rpm_target.rpm_target, "/can/dbc/vcu_rpm_target/rpm_target");
    SUB_FLOAT_MAP(vcu_torque_tgt, vcu_torque_target.torque_target, "/can/dbc/vcu_torque_target/torque_target");

    // DV Dynamics
    SUB_FLOAT_MAP(dv_spd_act, dv_dynamics_1.speed_actual, "/can/dbc/dv_dynamics_1/speed_actual");
    SUB_FLOAT_MAP(dv_spd_tgt, dv_dynamics_1.speed_target, "/can/dbc/dv_dynamics_1/speed_target");
    SUB_FLOAT_MAP(dv_str_act, dv_dynamics_1.steering_angle_actual, "/can/dbc/dv_dynamics_1/steering_angle_actual");
    SUB_FLOAT_MAP(dv_str_tgt, dv_dynamics_1.steering_angle_target, "/can/dbc/dv_dynamics_1/steering_angle_target");
    SUB_FLOAT_MAP(dv_brk_act, dv_dynamics_1.brake_hydr_actual, "/can/dbc/dv_dynamics_1/brake_hydr_actual");
    SUB_FLOAT_MAP(dv_brk_tgt, dv_dynamics_1.brake_hydr_target, "/can/dbc/dv_dynamics_1/brake_hydr_target");
    SUB_FLOAT_MAP(dv_mot_act, dv_dynamics_1.motor_moment_actual, "/can/dbc/dv_dynamics_1/motor_moment_actual");
    SUB_FLOAT_MAP(dv_mot_tgt, dv_dynamics_1.motor_moment_target, "/can/dbc/dv_dynamics_1/motor_moment_target");
    SUB_FLOAT_MAP(dv_acc_lon, dv_dynamics_2.acceleration_longitudinal, "/can/dbc/dv_dynamics_2/acceleration_longitudinal");
    SUB_FLOAT_MAP(dv_acc_lat, dv_dynamics_2.acceleration_lateral, "/can/dbc/dv_dynamics_2/acceleration_lateral");
    SUB_FLOAT_MAP(dv_yaw, dv_dynamics_2.yaw_rate, "/can/dbc/dv_dynamics_2/yaw_rate");

    // DV Status
    SUB_FLOAT_MAP(dv_as_status, dv_status.as_status, "/can/dbc/dv_status/as_status");
    SUB_FLOAT_MAP(dv_ebs_state, dv_status.asb_ebs_state, "/can/dbc/dv_status/asb_ebs_state");
    SUB_FLOAT_MAP(dv_ami_state, dv_status.ami_state, "/can/dbc/dv_status/ami_state");
    SUB_FLOAT_MAP(dv_steer_state, dv_status.steering_state, "/can/dbc/dv_status/steering_state");
    SUB_FLOAT_MAP(dv_ebs_red_state, dv_status.asb_redundancy_state, "/can/dbc/dv_status/asb_redundancy_state");

    // Pressures
    SUB_FLOAT_MAP(ebs_tank_f, asf_signals.ebs_pressure_tank_front, "/can/dbc/asf_signals/ebs_pressure_tank_front");
    SUB_FLOAT_MAP(ebs_tank_r, asf_signals.ebs_pressure_tank_rear, "/can/dbc/asf_signals/ebs_pressure_tank_rear");
    SUB_FLOAT_MAP(brk_press_f, asf_signals.brake_pressure_front, "/can/dbc/asf_signals/brake_pressure_front");
    SUB_FLOAT_MAP(brk_press_r, asf_signals.brake_pressure_rear, "/can/dbc/asf_signals/brake_pressure_rear");

    // Steering
    SUB_FLOAT_MAP(steer_pos, cubemars_feedback.position, "/can/dbc/cubemars_feedback/position");
    SUB_FLOAT_MAP(steer_spd, cubemars_feedback.speed_rpm, "/can/dbc/cubemars_feedback/speed_rpm");
    SUB_FLOAT_MAP(steer_curr, cubemars_feedback.current, "/can/dbc/cubemars_feedback/current");
    SUB_FLOAT_MAP(steer_temp, cubemars_feedback.driver_temp, "/can/dbc/cubemars_feedback/driver_temp");
    SUB_FLOAT_MAP(steer_err, cubemars_feedback.error_code, "/can/dbc/cubemars_feedback/error_code");
    SUB_FLOAT_MAP(steer_pos_tgt, cubemars_position_loop.position, "/can/dbc/cubemars_position_loop/position");

    // SLAM
    SUB_FLOAT_MAP(slam_laps, slam_stats_can.lap_counter, "/can/dbc/slam_stats_can/lap_counter");
    SUB_FLOAT_MAP(slam_cones, slam_stats_can.cones_count_actual, "/can/dbc/slam_stats_can/cones_count_actual");
    SUB_FLOAT_MAP(slam_all, slam_stats_can.cones_count_all, "/can/dbc/slam_stats_can/cones_count_all");

    // RES
    SUB_FLOAT_MAP(res_signal, res.signal, "/can/dbc/res/signal");

    // AQT1
    SUB_FLOAT_MAP(aqt1_brkp, aqt1.frt_brk_press, "/can/dbc/aqt1/frt_brk_press");
    SUB_FLOAT_MAP(aqt1_res, aqt1.res, "/can/dbc/aqt1/res");
    SUB_FLOAT_MAP(aqt1_bots, aqt1.bots, "/can/dbc/aqt1/bots");

    // AQT2 / 3 / 4 / 7
    // SUB_FLOAT_MAP(aqt2_whl_ang, aqt2.wheel_angle, "/can/dbc/aqt2/wheel_angle");
    // SUB_FLOAT_MAP(aqt3_whl_ang, aqt3.wheel_angle, "/can/dbc/aqt3/wheel_angle");
    SUB_FLOAT_MAP(aqt4_st_ang, aqt4.st_angle, "/can/dbc/aqt4/st_angle");
    SUB_FLOAT_MAP(aqt4_inertia, aqt4.inertia, "/can/dbc/aqt4/inertia");
    SUB_FLOAT_MAP(aqt4_emer, aqt4.emergency, "/can/dbc/aqt4/emergency");
    SUB_FLOAT_MAP(aqt7_brkp, aqt7.rear_brk_press, "/can/dbc/aqt7/rear_brk_press");

    // Missing/Additional DBC Subscriptions
    SUB_FLOAT_MAP(v_max, master_msc_id_3.overall_maximum_voltage, "/can/dbc/master_msc_id_3/overall_maximum_voltage");
    SUB_FLOAT_MAP(v_min, master_msc_id_3.overall_minimum_voltage, "/can/dbc/master_msc_id_3/overall_minimum_voltage");
    SUB_FLOAT_MAP(t_max, master_msc_id_3.overall_maximum_temperature, "/can/dbc/master_msc_id_3/overall_maximum_temperature");
    SUB_FLOAT_MAP(t_min, master_msc_id_3.overall_minimum_temperature, "/can/dbc/master_msc_id_3/overall_minimum_temperature");
    SUB_FLOAT_MAP(rear_ign, rear_wheel_l.ignition, "/can/dbc/rear_wheel_l/ignition");
    SUB_FLOAT_MAP(rear_r2d, rear_wheel_l.shutdown_circuit, "/can/dbc/rear_wheel_l/shutdown_circuit");

#undef SUB_FLOAT_MAP

    init_dbc_api_subscribers(g_node, g_subs);

    g_is_initialized.store(1);
    g_has_speed.store(0);
    g_has_hv.store(0);
    g_screen_change_requested.store(0);
    return 0;
}

LART_WEAK int ros2subscriber_spin_some(void) {
    if (!g_is_initialized.load() || !g_exec) {
        return 1;
    }

    try {
        g_exec->spin_some();
    } catch (...) {
    }
    return 0;
}

LART_WEAK int ros2subscriber_get_latest_speed(float *speed_kph) {
    if (!g_is_initialized.load() || !g_has_speed.load() || speed_kph == nullptr) {
        return 0;
    }

    *speed_kph = g_latest_speed_kph.load();
    return 1;
}

LART_WEAK int ros2subscriber_get_latest_hv(float *hv_value) {
    if (!g_is_initialized.load() || !g_has_hv.load() || hv_value == nullptr) {
        return 0;
    }

    *hv_value = g_latest_hv.load();
    return 1;
}

LART_WEAK int ros2subscriber_get_screen_change_request(int *screen_id) {
    if (!g_is_initialized.load() || !g_screen_change_requested.load() || screen_id == nullptr) {
        return 0;
    }

    *screen_id = g_requested_screen_id.load();
    g_screen_change_requested.store(0);
    return 1;
}

LART_WEAK int ros2subscriber_get_telemetry(TelemetryData *out) {
    if (!g_is_initialized.load() || out == nullptr) {
        return 0;
    }
    std::lock_guard<std::mutex> lock(g_telemetry_mutex);
    *out = g_telemetry;
    return 1;
}

LART_WEAK int ros2subscriber_get_can_log(char *buffer, size_t max_len) {
    if (!g_is_initialized.load() || buffer == nullptr || max_len == 0) {
        return 0;
    }
    std::lock_guard<std::mutex> lock(g_can_log_mutex);
    buffer[0] = '\0';
    for (const auto& line : g_can_log_frames) {
        if (strlen(buffer) + line.length() + 2 < max_len) {
            strncat(buffer, line.c_str(), max_len - strlen(buffer) - 1);
            strncat(buffer, "\n", max_len - strlen(buffer) - 1);
        }
    }
    return 1;
}

LART_WEAK void ros2subscriber_fini(void) {
    if (!g_is_initialized.load()) {
        return;
    }

    try {
        if (g_exec) {
            g_exec->cancel();
        }
    } catch (...) {
    }

    g_sub.reset();
    g_sub_hv.reset();
    g_sub_screen.reset();
    g_sub_can_frames.reset();
    g_sub_notifications.reset();
    g_sub_notification_ack.reset();
    g_pub_notification_ack.reset();
    g_subs.clear();

    {
        std::lock_guard<std::mutex> lock(g_can_log_mutex);
        g_can_log_frames.clear();
    }

    if (g_exec && g_node) {
        try {
            g_exec->remove_node(g_node);
        } catch (...) {
        }
    }

    g_node.reset();
    g_exec.reset();

    if (g_did_init_rclcpp && rclcpp::ok()) {
        try {
            rclcpp::shutdown();
        } catch (...) {
        }
    }
    g_did_init_rclcpp = false;

    g_is_initialized.store(0);
    g_has_speed.store(0);
    g_latest_speed_kph.store(0.0f);
    g_has_hv.store(0);
    g_latest_hv.store(0.0f);
    g_screen_change_requested.store(0);
    g_requested_screen_id.store(0);
}

LART_WEAK void ros2subscriber_publish_ack(const char *id) {
    if (g_pub_notification_ack && id) {
        std_msgs::msg::String msg;
        msg.data = id;
        g_pub_notification_ack->publish(msg);
    }
}

#else

LART_WEAK int ros2subscriber_init(void) {
    return 0;
}

LART_WEAK int ros2subscriber_spin_some(void) {
    return 0;
}

LART_WEAK int ros2subscriber_get_latest_speed(float *speed_kph) {
    if (speed_kph != nullptr) {
        *speed_kph = 0.0f;
    }
    return 0;
}

LART_WEAK int ros2subscriber_get_latest_hv(float *hv_value) {
    if (hv_value != nullptr) {
        *hv_value = 0.0f;
    }
    return 0;
}

LART_WEAK int ros2subscriber_get_screen_change_request(int *screen_id) {
    (void)screen_id;
    return 0;
}

LART_WEAK int ros2subscriber_get_telemetry(TelemetryData *out) {
    if (out != nullptr) {
        std::memset(out, 0, sizeof(TelemetryData));
    }
    return 0;
}

LART_WEAK int ros2subscriber_get_can_log(char *buffer, size_t max_len) {
    if (buffer != nullptr && max_len > 0) {
        buffer[0] = '\0';
    }
    return 0;
}

LART_WEAK void ros2subscriber_fini(void) {
}

LART_WEAK void ros2subscriber_publish_ack(const char *id) {
    (void)id;
}

#endif