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

    // Setup dynamic high-density telemetry subscriptions via aggregated DbcApi subscribers
#if defined(LART_HAVE_LART_MSGS) && LART_HAVE_LART_MSGS
    init_dbc_api_subscribers(g_node, g_subs);
#endif

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

        // Update g_telemetry from dbc_api atomically
        {
            std::lock_guard<std::mutex> lock(g_telemetry_mutex);
            g_telemetry.ams_state = dbc_api.master_msc_id_1.master_state;
            g_telemetry.ams_soc = dbc_api.master_soc_accumulator.soc_float;
            g_telemetry.ams_runtime = dbc_api.master_msc_id_1.ams_current_draw;
            g_telemetry.ams_fans = dbc_api.master_msc_id_1.master_fan_pwm;
            g_telemetry.ams_mcu_vref = dbc_api.master_msc_id_1.mcu_vref;
            g_telemetry.ams_mcu_temp = dbc_api.master_msc_id_1.mcu_temperature;
            g_telemetry.ams_pec_err = dbc_api.master_msc_id_1.adbms_pec_error;
            g_telemetry.ams_fault_cnt = dbc_api.master_msc_id_1.fault_counter;
            g_telemetry.ams_slaves = dbc_api.master_msc_id_4.slaves_detected;
            g_telemetry.ams_fw = dbc_api.master_msc_id_1.master_firmware_version;

            g_telemetry.precharge_state = dbc_api.master_precharge_id_1.precharge_state;
            g_telemetry.air_pos = dbc_api.master_precharge_id_1.precharge_ctc_air_pos_state;
            g_telemetry.air_min = dbc_api.master_precharge_id_1.precharge_ctc_air_min_state;
            g_telemetry.ctc_charge = dbc_api.master_precharge_id_1.precharge_ctc_charge_state;
            g_telemetry.ctc_discharge = dbc_api.master_precharge_id_1.precharge_ctc_discharge_state;

            g_telemetry.inv_erpm = dbc_api.hv500_erpm_duty_voltage.actual_erpm;
            g_telemetry.inv_duty = dbc_api.hv500_erpm_duty_voltage.actual_duty;
            g_telemetry.inv_vin = dbc_api.hv500_erpm_duty_voltage.actual_inputvoltage;
            g_telemetry.inv_fault = dbc_api.hv500_temperatures.actual_faultcode;
            g_telemetry.inv_ac_curr = dbc_api.hv500_ac_dc_current.actual_accurrent;
            g_telemetry.inv_dc_curr = dbc_api.hv500_ac_dc_current.actual_dccurrent;
            g_telemetry.inv_temp_ctrl = dbc_api.hv500_temperatures.actual_tempcontroller;
            g_telemetry.inv_temp_mot = dbc_api.hv500_temperatures.actual_tempmotor;
            g_telemetry.inv_throttle = dbc_api.hv500_misc.actual_throttle;
            g_telemetry.inv_brake = dbc_api.hv500_misc.actual_brake;
            g_telemetry.inv_foc_id = dbc_api.hv500_foc.actual_foc_id;
            g_telemetry.inv_foc_iq = dbc_api.hv500_foc.actual_foc_iq;
            g_telemetry.inv_drive_en = dbc_api.hv500_misc.drive_enable;

            g_telemetry.ivt_current = dbc_api.ivt_msg_result_i.ivt_result_i;
            g_telemetry.ivt_u1 = dbc_api.ivt_msg_result_u1.ivt_result_u1;
            g_telemetry.ivt_u2 = dbc_api.ivt_msg_result_u2.ivt_result_u2;
            g_telemetry.ivt_u3 = dbc_api.ivt_msg_result_u3.ivt_result_u3;
            g_telemetry.ivt_temp = dbc_api.ivt_msg_result_t.ivt_result_t;
            g_telemetry.ivt_power = dbc_api.ivt_msg_result_w.ivt_result_w;
            g_telemetry.ivt_energy = dbc_api.ivt_msg_result_wh.ivt_result_wh;

            g_telemetry.apps1 = dbc_api.pedal_box.apps1;
            g_telemetry.apps2 = dbc_api.pedal_box.apps2;

            g_telemetry.spd_fl = dbc_api.aqt2.wheel_spd;
            g_telemetry.temp_fl = dbc_api.aqt2.tire_temp;
            g_telemetry.brk_fl = dbc_api.aqt2.brake_temp;

            g_telemetry.spd_fr = dbc_api.aqt3.wheel_spd;
            g_telemetry.temp_fr = dbc_api.aqt3.tire_temp;
            g_telemetry.brk_fr = dbc_api.aqt3.brake_temp;

            g_telemetry.temp_rl = dbc_api.aqt5.tire_temp;
            g_telemetry.brk_rl = dbc_api.aqt5.brake_temp;

            g_telemetry.temp_rr = dbc_api.aqt6.tire_temp;
            g_telemetry.brk_rr = dbc_api.aqt6.brake_temp;

            g_telemetry.ntc1 = dbc_api.aqt8.ntc1;
            g_telemetry.ntc2 = dbc_api.aqt8.ntc2;
            g_telemetry.ntc3 = dbc_api.aqt8.ntc3;
            g_telemetry.susp_r = dbc_api.aqt7.susp_r;
            g_telemetry.susp_l = dbc_api.aqt7.susp_l;

            g_telemetry.acu_state = dbc_api.acu.acu_state;
            g_telemetry.acu_as_state = dbc_api.acu.as_state;
            g_telemetry.acu_assi_state = dbc_api.acu.assi_state;
            g_telemetry.acu_mission = dbc_api.acu.mission_select;
            g_telemetry.acu_asms = dbc_api.acu.asms;
            g_telemetry.acu_ign = dbc_api.acu.ign;
            g_telemetry.acu_emergency = dbc_api.acu.emergency;
            g_telemetry.acu_emerg_cause = dbc_api.acu.emergency_cause;
            g_telemetry.acu_cpu_temp = dbc_api.acu.acu_cpu_temp;

            g_telemetry.jetson_as_state = dbc_api.jetson.as_state;
            g_telemetry.jetson_mission = dbc_api.jetson.as_mission;
            g_telemetry.jetson_temp = dbc_api.jetson.temperature;
            g_telemetry.jetson_cpu = dbc_api.jetson.cpu;
            g_telemetry.jetson_gpu = dbc_api.jetson.gpu;
            g_telemetry.jetson_emerg_cause = dbc_api.jetson.emergency_cause;

            g_telemetry.vcu_ign_man = dbc_api.vcu_ign_r2d.ignition_manual;
            g_telemetry.vcu_ign_auto = dbc_api.vcu_ign_r2d.ignition_auto;
            g_telemetry.vcu_r2d_man = dbc_api.vcu_ign_r2d.r2d_manual;
            g_telemetry.vcu_r2d_auto = dbc_api.vcu_ign_r2d.r2d_auto;
            g_telemetry.vcu_shutdown = dbc_api.vcu_ign_r2d.shutdown_signal;
            g_telemetry.vcu_state = dbc_api.vcu_ign_r2d.vcu_state;
            g_telemetry.vcu_r2d_raw = dbc_api.vcu_ign_r2d.r2d_button_raw;
            g_telemetry.vcu_ign_raw = dbc_api.vcu_ign_r2d.ignition_switch_raw;
            g_telemetry.vcu_hv = dbc_api.vcu_hv.hv;
            g_telemetry.vcu_brkf = dbc_api.vcu_hv.brake_pressure_front;
            g_telemetry.vcu_brkr = dbc_api.vcu_hv.brake_pressure_rear;
            g_telemetry.vcu_rpm_act = dbc_api.vcu_rpm.motor_rpm_left;
            g_telemetry.vcu_rpm_tgt = dbc_api.vcu_rpm_target.rpm_target;
            g_telemetry.vcu_torque_tgt = dbc_api.vcu_torque_target.torque_target;

            g_telemetry.dv_spd_act = dbc_api.dv_dynamics_1.speed_actual;
            g_telemetry.dv_spd_tgt = dbc_api.dv_dynamics_1.speed_target;
            g_telemetry.dv_str_act = dbc_api.dv_dynamics_1.steering_angle_actual;
            g_telemetry.dv_str_tgt = dbc_api.dv_dynamics_1.steering_angle_target;
            g_telemetry.dv_brk_act = dbc_api.dv_dynamics_1.brake_hydr_actual;
            g_telemetry.dv_brk_tgt = dbc_api.dv_dynamics_1.brake_hydr_target;
            g_telemetry.dv_mot_act = dbc_api.dv_dynamics_1.motor_moment_actual;
            g_telemetry.dv_mot_tgt = dbc_api.dv_dynamics_1.motor_moment_target;
            g_telemetry.dv_acc_lon = dbc_api.dv_dynamics_2.acceleration_longitudinal;
            g_telemetry.dv_acc_lat = dbc_api.dv_dynamics_2.acceleration_lateral;
            g_telemetry.dv_yaw = dbc_api.dv_dynamics_2.yaw_rate;

            g_telemetry.dv_as_status = dbc_api.dv_status.as_status;
            g_telemetry.dv_ebs_state = dbc_api.dv_status.asb_ebs_state;
            g_telemetry.dv_ami_state = dbc_api.dv_status.ami_state;
            g_telemetry.dv_steer_state = dbc_api.dv_status.steering_state;
            g_telemetry.dv_ebs_red_state = dbc_api.dv_status.asb_redundancy_state;

            g_telemetry.ebs_tank_f = dbc_api.asf_signals.ebs_pressure_tank_front;
            g_telemetry.ebs_tank_r = dbc_api.asf_signals.ebs_pressure_tank_rear;
            g_telemetry.brk_press_f = dbc_api.asf_signals.brake_pressure_front;
            g_telemetry.brk_press_r = dbc_api.asf_signals.brake_pressure_rear;

            g_telemetry.steer_pos = dbc_api.cubemars_feedback.position;
            g_telemetry.steer_spd = dbc_api.cubemars_feedback.speed_rpm;
            g_telemetry.steer_curr = dbc_api.cubemars_feedback.current;
            g_telemetry.steer_temp = dbc_api.cubemars_feedback.driver_temp;
            g_telemetry.steer_err = dbc_api.cubemars_feedback.error_code;
            g_telemetry.steer_pos_tgt = dbc_api.cubemars_position_loop.position;

            g_telemetry.slam_laps = dbc_api.slam_stats_can.lap_counter;
            g_telemetry.slam_cones = dbc_api.slam_stats_can.cones_count_actual;
            g_telemetry.slam_all = dbc_api.slam_stats_can.cones_count_all;

            g_telemetry.res_signal = dbc_api.res.signal;

            g_telemetry.aqt1_brkp = dbc_api.aqt1.frt_brk_press;
            g_telemetry.aqt1_res = dbc_api.aqt1.res;
            g_telemetry.aqt1_bots = dbc_api.aqt1.bots;

            g_telemetry.aqt4_st_ang = dbc_api.aqt4.st_angle;
            g_telemetry.aqt4_inertia = dbc_api.aqt4.inertia;
            g_telemetry.aqt4_emer = dbc_api.aqt4.emergency;
            g_telemetry.aqt7_brkp = dbc_api.aqt7.rear_brk_press;

            g_telemetry.v_max = dbc_api.master_msc_id_3.overall_maximum_voltage;
            g_telemetry.v_min = dbc_api.master_msc_id_3.overall_minimum_voltage;
            g_telemetry.t_max = dbc_api.master_msc_id_3.overall_maximum_temperature;
            g_telemetry.t_min = dbc_api.master_msc_id_3.overall_minimum_temperature;
            g_telemetry.rear_ign = dbc_api.rear_wheel_l.ignition;
            g_telemetry.rear_r2d = dbc_api.rear_wheel_l.shutdown_circuit;
        }
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