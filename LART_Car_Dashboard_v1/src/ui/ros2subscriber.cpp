#include "ros2subscriber.h"

#include <stdio.h>

// Keep this file buildable in non-ROS toolchains used by the UI simulator.
#if defined(__has_include)
#if __has_include(<rclcpp/rclcpp.hpp>) && __has_include(<std_msgs/msg/float32.hpp>)
#define LART_HAVE_ROS2_CPP 1
#define LART_ROS2_CPP_LAYOUT_SHORT 1
#elif __has_include(<rclcpp/rclcpp/rclcpp.hpp>) && __has_include(<std_msgs/std_msgs/msg/float32.hpp>)
#define LART_HAVE_ROS2_CPP 1
#define LART_ROS2_CPP_LAYOUT_NESTED 1
#endif
#endif

#if !defined(LART_HAVE_ROS2_CPP)
#define LART_HAVE_ROS2_CPP 0
#endif

#if LART_HAVE_ROS2_CPP

#if defined(LART_ROS2_CPP_LAYOUT_NESTED)
#include <rclcpp/rclcpp/rclcpp.hpp>
#include <std_msgs/std_msgs/msg/float32.hpp>
#else
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float32.hpp>
#endif

#include <exception>
#include <memory>

#if defined(__GNUC__) || defined(__clang__)
#define LART_WEAK __attribute__((weak))
#else
#define LART_WEAK
#endif

// Make the exported symbols weak so a stale duplicate object does not break linking.
namespace {
constexpr const char *TOPIC_NAME = "/vehicle/speed_kph";

int g_is_initialized = 0;
int g_has_speed = 0;
float g_latest_speed_kph = 0.0f;

std::shared_ptr<rclcpp::Node> g_node;
rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr g_subscriber;
std::unique_ptr<rclcpp::executors::SingleThreadedExecutor> g_executor;
}  // namespace

LART_WEAK int ros2subscriber_init(void) {
    if (g_is_initialized) {
        return 0;
    }

    try {
        if (!rclcpp::ok()) {
            int argc = 0;
            char **argv = nullptr;
            rclcpp::init(argc, argv);
        }

        g_node = rclcpp::Node::make_shared("lart_dashboard_listener");
        g_subscriber = g_node->create_subscription<std_msgs::msg::Float32>(
            TOPIC_NAME,
            10,
            [](const std_msgs::msg::Float32 &msg) {
                g_latest_speed_kph = msg.data;
                g_has_speed = 1;
            });

        g_executor = std::make_unique<rclcpp::executors::SingleThreadedExecutor>();
        g_executor->add_node(g_node);
    } catch (const std::exception &ex) {
        fprintf(stderr, "ros2subscriber init failed: %s\n", ex.what());
        g_subscriber.reset();
        g_node.reset();
        g_executor.reset();
        g_has_speed = 0;
        return 1;
    }

    g_is_initialized = 1;
    return 0;
}

LART_WEAK int ros2subscriber_spin_some(void) {
    if (!g_is_initialized || !g_executor) {
        return 1;
    }

    try {
        g_executor->spin_some();
    } catch (const std::exception &ex) {
        fprintf(stderr, "ros2subscriber spin failed: %s\n", ex.what());
        return 1;
    }

    return 0;
}

LART_WEAK int ros2subscriber_get_latest_speed(float *speed_kph) {
    if (!g_is_initialized || !g_has_speed || speed_kph == nullptr) {
        return 0;
    }

    *speed_kph = g_latest_speed_kph;
    return 1;
}

LART_WEAK void ros2subscriber_fini(void) {
    if (!g_is_initialized) {
        return;
    }

    g_subscriber.reset();

    if (g_executor && g_node) {
        g_executor->remove_node(g_node);
    }

    g_executor.reset();
    g_node.reset();

    if (rclcpp::ok()) {
        rclcpp::shutdown();
    }

    g_is_initialized = 0;
    g_has_speed = 0;
}

#else

LART_WEAK int ros2subscriber_init(void) {
    fprintf(stderr, "ROS 2 C++ headers not available in current include path; ros2subscriber disabled.\n");
    return 1;
}

LART_WEAK int ros2subscriber_spin_some(void) {
    return 1;
}

LART_WEAK int ros2subscriber_get_latest_speed(float *speed_kph) {
    (void)speed_kph;
    return 0;
}

LART_WEAK void ros2subscriber_fini(void) {
}

#endif
