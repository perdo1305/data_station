#include <cstdint>

#include "ui.h"
#include "vars.h"
#include "eez-flow.h"

extern "C" void ui_set_hv(float hv_value) {
    (void)hv_value;
}

extern "C" void ui_set_lv(float lv_value) {
    eez::flow::setGlobalVariable(
        FLOW_GLOBAL_VARIABLE_LV,
        eez::FloatValue(lv_value)
    );
}
