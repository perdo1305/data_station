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
#include "dbc_api.h"

extern "C" void ui_update_telemetry_vars(const void *t_ptr) {
    if (t_ptr) {
        const TelemetryData *t = static_cast<const TelemetryData *>(t_ptr);
        dbc_api.asf_signals.brake_pressure_front = t->brk_press_f;
        dbc_api.asf_signals.brake_pressure_rear = t->brk_press_r;
        dbc_api.vcu_hv.brake_pressure_front = t->vcu_brkf;
        dbc_api.vcu_hv.brake_pressure_rear = t->vcu_brkr;
        dbc_api.hv500_misc.actual_brake = t->inv_brake;
        dbc_api.pedal_box.apps1 = t->apps1;
        dbc_api.pedal_box.apps2 = t->apps2;
        dbc_api.hv500_misc.actual_throttle = t->inv_throttle;
        dbc_api.master_soc_accumulator.soc_float = t->ams_soc;
        dbc_api.ivt_msg_result_u3.ivt_result_u3 = t->ivt_u3;
        dbc_api.master_msc_id_1.mcu_vref = t->ams_mcu_vref;
        dbc_api.vcu_ign_r2d.r2d_manual = t->vcu_r2d_man;
        dbc_api.vcu_ign_r2d.r2d_auto = t->vcu_r2d_auto;
        dbc_api.rear_wheel_l.shutdown_circuit = t->rear_r2d;
        dbc_api.acu.acu_state = t->acu_state;
        dbc_api.dv_dynamics_1.speed_actual = t->dv_spd_act;
        dbc_api.hv500_temperatures.actual_tempcontroller = t->inv_temp_ctrl;
        dbc_api.hv500_temperatures.actual_tempmotor = t->inv_temp_mot;
        dbc_api.slam_stats_can.lap_counter = t->slam_laps;
        dbc_api.acu.mission_select = t->acu_mission;
    }

    // 1. BRAKE PEDAL PRESSURE (0 to 100)
    float max_brk = dbc_api.asf_signals.brake_pressure_front;
    if (dbc_api.asf_signals.brake_pressure_rear > max_brk) max_brk = dbc_api.asf_signals.brake_pressure_rear;
    if (dbc_api.vcu_hv.brake_pressure_front > max_brk) max_brk = dbc_api.vcu_hv.brake_pressure_front;
    if (dbc_api.vcu_hv.brake_pressure_rear > max_brk) max_brk = dbc_api.vcu_hv.brake_pressure_rear;
    if (dbc_api.hv500_misc.actual_brake > max_brk) max_brk = dbc_api.hv500_misc.actual_brake;
    int brake_val = static_cast<int>(max_brk);
    if (brake_val < 0) brake_val = 0;
    if (brake_val > 100) brake_val = 100;
    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_BRAKE_PEDAL_PRESSURE, eez::IntegerValue(brake_val));

    // 2. ACCELL PEDAL PRESSURE (0 to 100)
    float max_acc = dbc_api.pedal_box.apps1;
    if (dbc_api.pedal_box.apps2 > max_acc) max_acc = dbc_api.pedal_box.apps2;
    if (dbc_api.hv500_misc.actual_throttle > max_acc) max_acc = dbc_api.hv500_misc.actual_throttle;
    int acc_val = static_cast<int>(max_acc);
    if (acc_val < 0) acc_val = 0;
    if (acc_val > 100) acc_val = 100;
    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_ACCELL_PEDAL_PRESSURE, eez::IntegerValue(acc_val));

    // 3. SOC (State of Charge, 0 to 100)
    int soc_val = static_cast<int>(dbc_api.master_soc_accumulator.soc_float);
    if (soc_val < 0) soc_val = 0;
    if (soc_val > 100) soc_val = 100;
    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_SOC, eez::IntegerValue(soc_val));

    // 4. LV (Low Voltage, Volts)
    float lv_val = dbc_api.ivt_msg_result_u3.ivt_result_u3 / 1000.0f;
    if (lv_val < 20.0f || lv_val > 28.0f) {
        if (dbc_api.master_msc_id_1.mcu_vref >= 20.0f && dbc_api.master_msc_id_1.mcu_vref <= 28.0f) {
            lv_val = dbc_api.master_msc_id_1.mcu_vref;
        } else {
            lv_val = 24.0f;
        }
    }
    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_LV, eez::FloatValue(lv_val));

    // 5. READY (String "READY" / "NOT READY")
    bool is_ready = (dbc_api.vcu_ign_r2d.r2d_manual == 1.0f || 
                    dbc_api.vcu_ign_r2d.r2d_auto == 1.0f || 
                    dbc_api.rear_wheel_l.shutdown_circuit == 1.0f || 
                    dbc_api.acu.acu_state == 4.0f || 
                    dbc_api.acu.acu_state == 5.0f);
    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_READY, eez::StringValue(is_ready ? "READY" : "NOT READY"));

    // 6. SPEED (km/h)
    float speed_val = dbc_api.dv_dynamics_1.speed_actual >= 0.0f ? dbc_api.dv_dynamics_1.speed_actual : 0.0f;
    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_SPEED, eez::FloatValue(speed_val));

    // 7. TEMP_INV (Inverter Temp, Celsius)
    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_TEMP_INV, eez::FloatValue(dbc_api.hv500_temperatures.actual_tempcontroller));

    // 8. TEMP_MOTOR (Motor Temp, Celsius)
    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_TEMP_MOTOR, eez::FloatValue(dbc_api.hv500_temperatures.actual_tempmotor));

    // 9. LAP_COUNT (Integer)
    int laps_val = static_cast<int>(dbc_api.slam_stats_can.lap_counter);
    if (laps_val < 0) laps_val = 0;
    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_LAP_COUNT, eez::IntegerValue(laps_val));

    // 10. MISSION (String "ACCEL", "SKIDPAD", etc.)
    const char *mission_str = "NONE";
    int mission_id = static_cast<int>(dbc_api.acu.mission_select);
    if (mission_id == 1) mission_str = "ACCEL";
    else if (mission_id == 2) mission_str = "SKIDPAD";
    else if (mission_id == 3) mission_str = "AUTOCROSS";
    else if (mission_id == 4) mission_str = "ENDURANCE";
    else if (mission_id == 5) mission_str = "TRACKDRIVE";
    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_MISSION, eez::StringValue(mission_str));
}

extern "C" const char *ui_get_mission_str() {
    const char *str = eez::flow::getGlobalVariable(FLOW_GLOBAL_VARIABLE_MISSION).getString();
    return str ? str : "";
}
