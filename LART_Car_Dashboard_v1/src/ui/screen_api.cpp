#include "ui.h"
#include "vars.h"
#include "eez-flow.h"

extern "C" void ui_set_screen_var(int screen_id) {
    eez::flow::setGlobalVariable(
        FLOW_GLOBAL_VARIABLE_SCREEN_CHANGER_VAR,
        eez::IntegerValue(screen_id)
    );
}
