#include "ui.h"
#include "vars.h"
#include "eez-flow.h"

extern "C" void ui_set_speed(float speed_kph) {
    (void)speed_kph;
    eez::flow::setGlobalVariable(
        FLOW_GLOBAL_VARIABLE_SPEED,
        eez::FloatValue(speed_kph)
    );
}