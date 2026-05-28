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

#include "ros2subscriber.h"

extern "C" void ui_update_telemetry_vars(const void *t_ptr) {
    if (!t_ptr) return;
    const TelemetryData *t = static_cast<const TelemetryData *>(t_ptr);

    // 1. BRAKE PEDAL PRESSURE (0 to 100)
    float max_brk = t->brk_press_f;
    if (t->brk_press_r > max_brk) max_brk = t->brk_press_r;
    if (t->vcu_brkf > max_brk) max_brk = t->vcu_brkf;
    if (t->vcu_brkr > max_brk) max_brk = t->vcu_brkr;
    if (t->inv_brake > max_brk) max_brk = t->inv_brake;
    int brake_val = static_cast<int>(max_brk);
    if (brake_val < 0) brake_val = 0;
    if (brake_val > 100) brake_val = 100;
    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_BRAKE_PEDAL_PRESSURE, eez::IntegerValue(brake_val));

    // 2. ACCELL PEDAL PRESSURE (0 to 100)
    float max_acc = t->apps1;
    if (t->apps2 > max_acc) max_acc = t->apps2;
    if (t->inv_throttle > max_acc) max_acc = t->inv_throttle;
    int acc_val = static_cast<int>(max_acc);
    if (acc_val < 0) acc_val = 0;
    if (acc_val > 100) acc_val = 100;
    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_ACCELL_PEDAL_PRESSURE, eez::IntegerValue(acc_val));

    // 3. SOC (State of Charge, 0 to 100)
    int soc_val = static_cast<int>(t->ams_soc);
    if (soc_val < 0) soc_val = 0;
    if (soc_val > 100) soc_val = 100;
    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_SOC, eez::IntegerValue(soc_val));

    // 4. LV (Low Voltage, Volts)
    float lv_val = t->ivt_u3 / 1000.0f;
    if (lv_val <= 0.0f) {
        lv_val = t->ams_mcu_vref > 0.0f ? t->ams_mcu_vref : 24.0f;
    }
    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_LV, eez::FloatValue(lv_val));

    // 5. READY (String "READY" / "NOT READY")
    bool is_ready = (t->vcu_r2d_man == 1.0f || t->vcu_r2d_auto == 1.0f || t->rear_r2d == 1.0f || t->acu_state == 4.0f || t->acu_state == 5.0f);
    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_READY, eez::StringValue(is_ready ? "READY" : "NOT READY"));

    // 6. SPEED (km/h)
    float speed_val = t->dv_spd_act >= 0.0f ? t->dv_spd_act : 0.0f;
    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_SPEED, eez::FloatValue(speed_val));

    // 7. TEMP_INV (Inverter Temp, Celsius)
    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_TEMP_INV, eez::FloatValue(t->inv_temp_ctrl));

    // 8. TEMP_MOTOR (Motor Temp, Celsius)
    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_TEMP_MOTOR, eez::FloatValue(t->inv_temp_mot));

    // 9. LAP_COUNT (Integer)
    int laps_val = static_cast<int>(t->slam_laps);
    if (laps_val < 0) laps_val = 0;
    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_LAP_COUNT, eez::IntegerValue(laps_val));

    // 10. MISSION (String "ACCEL", "SKIDPAD", etc.)
    const char *mission_str = "NONE";
    int mission_id = static_cast<int>(t->acu_mission);
    if (mission_id == 1) mission_str = "ACCEL";
    else if (mission_id == 2) mission_str = "SKIDPAD";
    else if (mission_id == 3) mission_str = "AUTOCROSS";
    else if (mission_id == 4) mission_str = "ENDURANCE";
    else if (mission_id == 5) mission_str = "TRACKDRIVE";
    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_MISSION, eez::StringValue(mission_str));
}
