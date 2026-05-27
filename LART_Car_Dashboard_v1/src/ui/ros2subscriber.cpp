#include "ros2subscriber.h"

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

bool g_did_init_rclcpp = false;
std::shared_ptr<rclcpp::Node> g_node;
std::shared_ptr<rclcpp::executors::SingleThreadedExecutor> g_exec;
rclcpp::SubscriptionBase::SharedPtr g_sub;
rclcpp::SubscriptionBase::SharedPtr g_sub_hv;
rclcpp::SubscriptionBase::SharedPtr g_sub_screen;

bool env_is_true(const char *name, bool default_value) {
    const char *value = std::getenv(name);
    if (value == nullptr || *value == '\0') {
        return default_value;
    }
    return std::strcmp(value, "1") == 0 || strcasecmp(value, "true") == 0 || strcasecmp(value, "yes") == 0;
}

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
            // Valid 0-based screen range: 0=Driver, 1=Autonomous, 2=Debug, 3=Debug Autonomous 1/2, 4=Debug Autonomous 2/2
            if (id < 0 || id > 4) {
                return;  // Ignore out-of-range IDs silently
            }
            g_requested_screen_id.store(id);
            g_screen_change_requested.store(1);
        }
    };

    g_sub_screen = g_node->create_subscription<std_msgs::msg::Int32>(screen_topic, rclcpp::QoS(10), screen_callback);

    // Setup dynamic high-density telemetry subscriptions
#define SUB_FLOAT(field, topic_name) \
    g_subs.push_back(g_node->create_subscription<std_msgs::msg::Float32>( \
        topic_name, sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) { \
            if (msg) { \
                std::lock_guard<std::mutex> lock(g_telemetry_mutex); \
                g_telemetry.field = msg->data; \
            } \
        }))

    // AMS Status
    SUB_FLOAT(ams_state, "/can/dbc/master_msc_id_1/master_state");
    SUB_FLOAT(ams_soc, "/can/dbc/master_soc_accumulator/soc_float");
    SUB_FLOAT(ams_runtime, "/can/dbc/master_msc_id_1/ams_current_draw");
    SUB_FLOAT(ams_fans, "/can/dbc/master_msc_id_1/master_fan_pwm");
    SUB_FLOAT(ams_mcu_vref, "/can/dbc/master_msc_id_1/mcu_vref");
    SUB_FLOAT(ams_mcu_temp, "/can/dbc/master_msc_id_1/mcu_temperature");
    SUB_FLOAT(ams_pec_err, "/can/dbc/master_msc_id_1/adbms_pec_error");
    SUB_FLOAT(ams_fault_cnt, "/can/dbc/master_msc_id_1/fault_counter");
    SUB_FLOAT(ams_slaves, "/can/dbc/master_msc_id_4/slaves_detected");
    SUB_FLOAT(ams_fw, "/can/dbc/master_msc_id_1/master_firmware_version");

    // Precharge
    SUB_FLOAT(precharge_state, "/can/dbc/master_precharge_id_1/precharge_state");
    SUB_FLOAT(air_pos, "/can/dbc/master_precharge_id_1/precharge_ctc_air_pos_state");
    SUB_FLOAT(air_min, "/can/dbc/master_precharge_id_1/precharge_ctc_air_min_state");
    SUB_FLOAT(ctc_charge, "/can/dbc/master_precharge_id_1/precharge_ctc_charge_state");
    SUB_FLOAT(ctc_discharge, "/can/dbc/master_precharge_id_1/precharge_ctc_discharge_state");

    // Inverter DTI HV500
    SUB_FLOAT(inv_erpm, "/can/dbc/hv500_erpm_duty_voltage/actual_erpm");
    SUB_FLOAT(inv_duty, "/can/dbc/hv500_erpm_duty_voltage/actual_duty");
    SUB_FLOAT(inv_vin, "/can/dbc/hv500_erpm_duty_voltage/actual_inputvoltage");
    SUB_FLOAT(inv_fault, "/can/dbc/hv500_temperatures/actual_faultcode");
    SUB_FLOAT(inv_ac_curr, "/can/dbc/hv500_ac_dc_current/actual_accurrent");
    SUB_FLOAT(inv_dc_curr, "/can/dbc/hv500_ac_dc_current/actual_dccurrent");
    SUB_FLOAT(inv_temp_ctrl, "/can/dbc/hv500_temperatures/actual_tempcontroller");
    SUB_FLOAT(inv_temp_mot, "/can/dbc/hv500_temperatures/actual_tempmotor");
    SUB_FLOAT(inv_throttle, "/can/dbc/hv500_misc/actual_throttle");
    SUB_FLOAT(inv_brake, "/can/dbc/hv500_misc/actual_brake");
    SUB_FLOAT(inv_foc_id, "/can/dbc/hv500_foc/actual_foc_id");
    SUB_FLOAT(inv_foc_iq, "/can/dbc/hv500_foc/actual_foc_iq");
    SUB_FLOAT(inv_drive_en, "/can/dbc/hv500_misc/drive_enable");

    // IVT
    SUB_FLOAT(ivt_current, "/can/dbc/ivt_msg_result_i/ivt_result_i");
    SUB_FLOAT(ivt_u1, "/can/dbc/ivt_msg_result_u1/ivt_result_u1");
    SUB_FLOAT(ivt_u2, "/can/dbc/ivt_msg_result_u2/ivt_result_u2");
    SUB_FLOAT(ivt_u3, "/can/dbc/ivt_msg_result_u3/ivt_result_u3");
    SUB_FLOAT(ivt_temp, "/can/dbc/ivt_msg_result_t/ivt_result_t");
    SUB_FLOAT(ivt_power, "/can/dbc/ivt_msg_result_w/ivt_result_w");
    SUB_FLOAT(ivt_energy, "/can/dbc/ivt_msg_result_wh/ivt_result_wh");

    // Pedals
    SUB_FLOAT(apps1, "/can/dbc/pedal_box/apps1");
    SUB_FLOAT(apps2, "/can/dbc/pedal_box/apps2");

    // Wheels (AQT2/3/5/6)
    SUB_FLOAT(spd_fl, "/can/dbc/aqt2/spd_wheel");
    SUB_FLOAT(temp_fl, "/can/dbc/aqt2/tire_temp");
    SUB_FLOAT(brk_fl, "/can/dbc/aqt2/brake_temp");

    SUB_FLOAT(spd_fr, "/can/dbc/aqt3/spd_wheel");
    SUB_FLOAT(temp_fr, "/can/dbc/aqt3/tire_temp");
    SUB_FLOAT(brk_fr, "/can/dbc/aqt3/brake_temp");

    SUB_FLOAT(spd_rl, "/can/dbc/aqt5/spd_wheel");
    SUB_FLOAT(temp_rl, "/can/dbc/aqt5/tire_temp");
    SUB_FLOAT(brk_rl, "/can/dbc/aqt5/brake_temp");

    SUB_FLOAT(spd_rr, "/can/dbc/aqt6/spd_wheel");
    SUB_FLOAT(temp_rr, "/can/dbc/aqt6/tire_temp");
    SUB_FLOAT(brk_rr, "/can/dbc/aqt6/brake_temp");

    // Rear Sensors AQT7
    SUB_FLOAT(ntc1, "/can/dbc/aqt7_1/ntc_1");
    SUB_FLOAT(ntc2, "/can/dbc/aqt7_1/ntc_2");
    SUB_FLOAT(ntc3, "/can/dbc/aqt7_1/ntc_3");
    SUB_FLOAT(susp_r, "/can/dbc/aqt7_2/susp_r");
    SUB_FLOAT(susp_l, "/can/dbc/aqt7_2/susp_l");

    // Autonomous: ACU
    SUB_FLOAT(acu_state, "/can/dbc/acu/acu_state");
    SUB_FLOAT(acu_as_state, "/can/dbc/acu/as_state");
    SUB_FLOAT(acu_assi_state, "/can/dbc/acu/assi_state");
    SUB_FLOAT(acu_mission, "/can/dbc/acu/mission_select");
    SUB_FLOAT(acu_asms, "/can/dbc/acu/asms");
    SUB_FLOAT(acu_ign, "/can/dbc/acu/ign");
    SUB_FLOAT(acu_emergency, "/can/dbc/acu/emergency");
    SUB_FLOAT(acu_emerg_cause, "/can/dbc/acu/emergency_cause");
    SUB_FLOAT(acu_cpu_temp, "/can/dbc/acu/acu_cpu_temp");

    // Jetson
    SUB_FLOAT(jetson_as_state, "/can/dbc/jetson/as_state");
    SUB_FLOAT(jetson_mission, "/can/dbc/jetson/as_mission");
    SUB_FLOAT(jetson_temp, "/can/dbc/jetson/temperature");
    SUB_FLOAT(jetson_cpu, "/can/dbc/jetson/cpu");
    SUB_FLOAT(jetson_gpu, "/can/dbc/jetson/gpu");
    SUB_FLOAT(jetson_emerg_cause, "/can/dbc/jetson/emergency_cause");

    // VCU
    SUB_FLOAT(vcu_ign_man, "/can/dbc/vcu_ign_r2d/ignition_manual");
    SUB_FLOAT(vcu_ign_auto, "/can/dbc/vcu_ign_r2d/ignition_auto");
    SUB_FLOAT(vcu_r2d_man, "/can/dbc/vcu_ign_r2d/r2d_manual");
    SUB_FLOAT(vcu_r2d_auto, "/can/dbc/vcu_ign_r2d/r2d_auto");
    SUB_FLOAT(vcu_shutdown, "/can/dbc/vcu_ign_r2d/shutdown_signal");
    SUB_FLOAT(vcu_state, "/can/dbc/vcu_ign_r2d/vcu_state");
    SUB_FLOAT(vcu_r2d_raw, "/can/dbc/vcu_ign_r2d/r2d_button_raw");
    SUB_FLOAT(vcu_ign_raw, "/can/dbc/vcu_ign_r2d/ignition_switch_raw");
    SUB_FLOAT(vcu_hv, "/can/dbc/vcu_hv/hv");
    SUB_FLOAT(vcu_brkf, "/can/dbc/vcu_hv/brake_pressure_front");
    SUB_FLOAT(vcu_brkr, "/can/dbc/vcu_hv/brake_pressure_rear");
    SUB_FLOAT(vcu_rpm_act, "/can/dbc/vcu_rpm/rpm_actual");
    SUB_FLOAT(vcu_rpm_tgt, "/can/dbc/vcu_rpm_target/rpm_target");
    SUB_FLOAT(vcu_torque_tgt, "/can/dbc/vcu_torque_target/torque_target");

    // DV Dynamics
    SUB_FLOAT(dv_spd_act, "/can/dbc/dv_dynamics_1/speed_actual");
    SUB_FLOAT(dv_spd_tgt, "/can/dbc/dv_dynamics_1/speed_target");
    SUB_FLOAT(dv_str_act, "/can/dbc/dv_dynamics_1/steering_angle_actual");
    SUB_FLOAT(dv_str_tgt, "/can/dbc/dv_dynamics_1/steering_angle_target");
    SUB_FLOAT(dv_brk_act, "/can/dbc/dv_dynamics_1/brake_hydr_actual");
    SUB_FLOAT(dv_brk_tgt, "/can/dbc/dv_dynamics_1/brake_hydr_target");
    SUB_FLOAT(dv_mot_act, "/can/dbc/dv_dynamics_1/motor_moment_actual");
    SUB_FLOAT(dv_mot_tgt, "/can/dbc/dv_dynamics_1/motor_moment_target");
    SUB_FLOAT(dv_acc_lon, "/can/dbc/dv_dynamics_2/acceleration_longitudinal");
    SUB_FLOAT(dv_acc_lat, "/can/dbc/dv_dynamics_2/acceleration_lateral");
    SUB_FLOAT(dv_yaw, "/can/dbc/dv_dynamics_2/yaw_rate");

    // DV Status
    SUB_FLOAT(dv_as_status, "/can/dbc/dv_status/as_status");
    SUB_FLOAT(dv_ebs_state, "/can/dbc/dv_status/asb_ebs_state");
    SUB_FLOAT(dv_ami_state, "/can/dbc/dv_status/ami_state");
    SUB_FLOAT(dv_steer_state, "/can/dbc/dv_status/steering_state");
    SUB_FLOAT(dv_ebs_red_state, "/can/dbc/dv_status/asb_redundancy_state");

    // Pressures
    SUB_FLOAT(ebs_tank_f, "/can/dbc/asf_signals/ebs_pressure_tank_front");
    SUB_FLOAT(ebs_tank_r, "/can/dbc/asf_signals/ebs_pressure_tank_rear");
    SUB_FLOAT(brk_press_f, "/can/dbc/asf_signals/brake_pressure_front");
    SUB_FLOAT(brk_press_r, "/can/dbc/asf_signals/brake_pressure_rear");

    // Steering
    SUB_FLOAT(steer_pos, "/can/dbc/cubemars_feedback/position");
    SUB_FLOAT(steer_spd, "/can/dbc/cubemars_feedback/speed_rpm");
    SUB_FLOAT(steer_curr, "/can/dbc/cubemars_feedback/current");
    SUB_FLOAT(steer_temp, "/can/dbc/cubemars_feedback/driver_temp");
    SUB_FLOAT(steer_err, "/can/dbc/cubemars_feedback/error_code");
    SUB_FLOAT(steer_pos_tgt, "/can/dbc/cubemars_position_loop/position");

    // SLAM
    SUB_FLOAT(slam_laps, "/can/dbc/slam_stats_can/lap_counter");
    SUB_FLOAT(slam_cones, "/can/dbc/slam_stats_can/cones_count_actual");
    SUB_FLOAT(slam_all, "/can/dbc/slam_stats_can/cones_count_all");

    // RES
    SUB_FLOAT(res_signal, "/can/dbc/res/signal");

    // AQT1
    SUB_FLOAT(aqt1_brkp, "/can/dbc/aqt1/brk_press");
    SUB_FLOAT(aqt1_res, "/can/dbc/aqt1/res");
    SUB_FLOAT(aqt1_bots, "/can/dbc/aqt1/bots");

    // AQT2 / 3 / 4 / 7
    SUB_FLOAT(aqt2_whl_ang, "/can/dbc/aqt2/wheel_angle");
    SUB_FLOAT(aqt3_whl_ang, "/can/dbc/aqt3/wheel_angle");
    SUB_FLOAT(aqt4_st_ang, "/can/dbc/aqt4/st_angle");
    SUB_FLOAT(aqt4_inertia, "/can/dbc/aqt4/inertia");
    SUB_FLOAT(aqt4_emer, "/can/dbc/aqt4/emer_button");
    SUB_FLOAT(aqt7_brkp, "/can/dbc/aqt7/brk_press");
#undef SUB_FLOAT

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
    g_subs.clear();

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

LART_WEAK void ros2subscriber_fini(void) {
}

#endif