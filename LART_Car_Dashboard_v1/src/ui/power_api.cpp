#include <cstdint>

#include "ui.h"
#include "vars.h"
#include "eez-flow.h"

extern "C" void ui_set_hv(float hv_value) {
    int32_t hv_int = static_cast<int32_t>(hv_value);
    eez::flow::setGlobalVariable(
        FLOW_GLOBAL_VARIABLE_HV,
        eez::IntegerValue(hv_int)
    );
}

extern "C" void ui_set_lv(float lv_value) {
    eez::flow::setGlobalVariable(
        FLOW_GLOBAL_VARIABLE_LV,
        eez::FloatValue(lv_value)
    );
}
