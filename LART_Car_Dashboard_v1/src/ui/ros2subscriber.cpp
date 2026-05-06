#include "ros2subscriber.h"

#include <atomic>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <strings.h>
#include <memory>

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
#include <std_msgs/msg/float32.hpp>

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
constexpr const char *DEFAULT_SPEED_TOPIC = "/vehicle/speed_kph";
constexpr const char *DEFAULT_DASHBOARD_STATE_TOPIC = "/vehicle/dashboard_state";

std::atomic<int> g_is_initialized(0);
std::atomic<int> g_has_speed(0);
std::atomic<float> g_latest_speed_kph(0.0f);

bool g_did_init_rclcpp = false;
std::shared_ptr<rclcpp::Node> g_node;
std::shared_ptr<rclcpp::executors::SingleThreadedExecutor> g_exec;
rclcpp::SubscriptionBase::SharedPtr g_sub;

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

    auto callback = [](const std_msgs::msg::Float32::SharedPtr msg) {
        if (msg) {
            g_latest_speed_kph.store(msg->data);
            g_has_speed.store(1);
        }
    };

    g_sub = g_node->create_subscription<std_msgs::msg::Float32>(topic, rclcpp::QoS(10), callback);

    g_is_initialized.store(1);
    g_has_speed.store(0);
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

LART_WEAK void ros2subscriber_fini(void) {
}

#endif