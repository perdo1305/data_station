#include "ros2subscriber.h"
#include <rclcpp/rclcpp.hpp>
#include <lart_msgs/msg/acu.hpp>
#include <std_msgs/msg/float32.hpp>
#include <iostream>
#include <cassert>
#include <thread>
#include <chrono>
#include <cstdlib>
#include "eez-flow.h"
#include "dbc_api.h"

// Stub implementations of eez functions to satisfy the linker for the test executable
namespace eez {
    void free(void *ptr) {
        ::free(ptr);
    }

    Value Value::makeStringRef(const char *str, int len, uint32_t id) {
        (void)len;
        (void)id;
        Value val;
        val.type = VALUE_TYPE_STRING;
        val.options = 0;
        val.strValue = str;
        return val;
    }

    const char *Value::getString() const {
        return type == VALUE_TYPE_STRING ? strValue : nullptr;
    }

    namespace flow {
        void setGlobalVariable(uint32_t globalVariableIndex, const Value &value) {
            (void)globalVariableIndex;
            (void)value;
        }

        Value getGlobalVariable(uint32_t globalVariableIndex) {
            (void)globalVariableIndex;
            return Value();
        }
    }
}

// Stub UI notification callbacks (normally provided by ui_runner, not linked in this test)
extern "C" {
    void ui_add_notification(const char *id, const char *title, const char *message) {
        (void)id; (void)title; (void)message;
    }
    void ui_clear_notification(const char *id) {
        (void)id;
    }
    void ui_clear_all_notifications(void) {}
}

int main(int argc, char **argv) {
    // Sourcing rclcpp dependencies and initializing execution context
    rclcpp::init(argc, argv);
    
    // ros2subscriber_init sets up the dashboard subscriptions.
    int init_res = ros2subscriber_init();
    assert(init_res == 0);
    
    auto pub_node = std::make_shared<rclcpp::Node>("test_publisher");
    auto asms_pub = pub_node->create_publisher<lart_msgs::msg::Acu>("/can/acu", rclcpp::QoS(10).best_effort());

    auto asms_msg = std::make_shared<lart_msgs::msg::Acu>();
    asms_msg->asms = 1.0f;
    
    // We spin and publish multiple times. This is needed because ROS2 discovery
    // of subscription matches can take some time.
    for (int i = 0; i < 50; ++i) {
        asms_pub->publish(*asms_msg);
        ros2subscriber_spin_some();
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    // Verify ASMS telemetry propagation
    std::cout << "Verifying ASMS telemetry: dbc_api.acu.asms = " << dbc_api.acu.asms << std::endl;
    if (dbc_api.acu.asms != 1.0f) {
        std::cerr << "Test failed! ASMS value did not propagate." << std::endl;
        ros2subscriber_fini();
        rclcpp::shutdown();
        return 1;
    }
    
    std::cout << "Test passed!" << std::endl;
    ros2subscriber_fini();
    rclcpp::shutdown();
    return 0;
}
