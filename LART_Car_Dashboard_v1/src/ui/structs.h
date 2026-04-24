#ifndef EEZ_LVGL_UI_STRUCTS_H
#define EEZ_LVGL_UI_STRUCTS_H

#include "eez-flow.h"

#include <stdint.h>
#include <stdbool.h>

#include "vars.h"

using namespace eez;

enum FlowStructures {
    FLOW_STRUCTURE_SUBSCRIPTION = 16384
};

enum FlowArrayOfStructures {
    FLOW_ARRAY_OF_STRUCTURE_SUBSCRIPTION = 81920
};

enum SubscriptionFlowStructureFields {
    FLOW_STRUCTURE_SUBSCRIPTION_FIELD_TOPIC = 0,
    FLOW_STRUCTURE_SUBSCRIPTION_FIELD_PAYLOAD = 1,
    FLOW_STRUCTURE_SUBSCRIPTION_NUM_FIELDS
};

struct SubscriptionValue {
    Value value;
    
    SubscriptionValue() {
        value = Value::makeArrayRef(FLOW_STRUCTURE_SUBSCRIPTION_NUM_FIELDS, FLOW_STRUCTURE_SUBSCRIPTION, 0);
    }
    
    SubscriptionValue(Value value) : value(value) {}
    
    operator Value() const { return value; }
    
    operator bool() const { return value.isArray(); }
    
    const char *topic() {
        return value.getArray()->values[FLOW_STRUCTURE_SUBSCRIPTION_FIELD_TOPIC].getString();
    }
    void topic(const char *topic) {
        value.getArray()->values[FLOW_STRUCTURE_SUBSCRIPTION_FIELD_TOPIC] = StringValue(topic);
    }
    
    const char *payload() {
        return value.getArray()->values[FLOW_STRUCTURE_SUBSCRIPTION_FIELD_PAYLOAD].getString();
    }
    void payload(const char *payload) {
        value.getArray()->values[FLOW_STRUCTURE_SUBSCRIPTION_FIELD_PAYLOAD] = StringValue(payload);
    }
};

typedef ArrayOf<SubscriptionValue, FLOW_ARRAY_OF_STRUCTURE_SUBSCRIPTION> ArrayOfSubscriptionValue;

typedef struct {
    const char *topic;
    const char *payload;
} MQTTMessage;

#endif /*EEZ_LVGL_UI_STRUCTS_H*/