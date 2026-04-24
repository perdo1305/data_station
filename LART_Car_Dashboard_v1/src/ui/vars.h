#ifndef EEZ_LVGL_UI_VARS_H
#define EEZ_LVGL_UI_VARS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// enum declarations

// Flow global variables

enum FlowGlobalVariables {
    FLOW_GLOBAL_VARIABLE_BRAKE_PRESSURE = 0,
    FLOW_GLOBAL_VARIABLE_ACCELL_PRESSURE = 1,
    FLOW_GLOBAL_VARIABLE_HV = 2,
    FLOW_GLOBAL_VARIABLE_LV = 3,
    FLOW_GLOBAL_VARIABLE_READY = 4,
    FLOW_GLOBAL_VARIABLE_SPEED = 5,
    FLOW_GLOBAL_VARIABLE_TEMP_INV = 6,
    FLOW_GLOBAL_VARIABLE_TEMP_MOTOR = 7,
    FLOW_GLOBAL_VARIABLE_LOADING = 8,
    FLOW_GLOBAL_VARIABLE_MQTT_HOST = 9,
    FLOW_GLOBAL_VARIABLE_MQTT_PORT = 10,
    FLOW_GLOBAL_VARIABLE_MQTT_CONNECTED = 11,
    FLOW_GLOBAL_VARIABLE_MQTT_CONNECTION_STRING = 12
};

// Native global variables

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_VARS_H*/