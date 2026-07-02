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

    // Per-signal DBC telemetry now arrives via the aggregated per-message
    // topics (e.g. "/can/dbc/aqt2") set up below, not the old split
    // per-signal topics.
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