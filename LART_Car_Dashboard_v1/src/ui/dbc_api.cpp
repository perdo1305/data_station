// Auto-generated from DBC files by generate_dbc_api.py. Do not edit.
#include "dbc_api.h"
#include <string>
#include <cctype>
#include <cstring>

DbcApi dbc_api = {};

#if defined(LART_UI_HAVE_RCLCPP) && LART_UI_HAVE_RCLCPP
#include <mutex>
std::mutex dbc_api_mutex;

#if defined(LART_HAVE_LART_MSGS) && LART_HAVE_LART_MSGS
#include <rclcpp/rclcpp.hpp>
#include <vector>
extern void init_dbc_api_subscribers_chunk_0(std::shared_ptr<rclcpp::Node> node, std::vector<rclcpp::SubscriptionBase::SharedPtr>& subs);
extern void init_dbc_api_subscribers_chunk_1(std::shared_ptr<rclcpp::Node> node, std::vector<rclcpp::SubscriptionBase::SharedPtr>& subs);
extern void init_dbc_api_subscribers_chunk_2(std::shared_ptr<rclcpp::Node> node, std::vector<rclcpp::SubscriptionBase::SharedPtr>& subs);
extern void init_dbc_api_subscribers_chunk_3(std::shared_ptr<rclcpp::Node> node, std::vector<rclcpp::SubscriptionBase::SharedPtr>& subs);
extern void init_dbc_api_subscribers_chunk_4(std::shared_ptr<rclcpp::Node> node, std::vector<rclcpp::SubscriptionBase::SharedPtr>& subs);
extern void init_dbc_api_subscribers_chunk_5(std::shared_ptr<rclcpp::Node> node, std::vector<rclcpp::SubscriptionBase::SharedPtr>& subs);

void init_dbc_api_subscribers(std::shared_ptr<rclcpp::Node> node, std::vector<rclcpp::SubscriptionBase::SharedPtr>& subs) {
    init_dbc_api_subscribers_chunk_0(node, subs);
    init_dbc_api_subscribers_chunk_1(node, subs);
    init_dbc_api_subscribers_chunk_2(node, subs);
    init_dbc_api_subscribers_chunk_3(node, subs);
    init_dbc_api_subscribers_chunk_4(node, subs);
    init_dbc_api_subscribers_chunk_5(node, subs);
}
#endif
#endif

// Combined from power_api.cpp and speed_api.cpp
#include "ui.h"
#include "vars.h"
#include "eez-flow.h"
#include "ros2subscriber.h"
#include <cstdint>

// Last LV value shown on the dashboard (Volts) — used by the screen tick
// code to render the LV label with one decimal.
static float ui_lv_value = 20.0f;

extern "C" const char *ui_get_lv_str() {
    static char buf[16];
    snprintf(buf, sizeof(buf), "%.1f V", ui_lv_value);
    return buf;
}

extern "C" void ui_set_hv(float hv_value) {
    (void)hv_value;
}

extern "C" void ui_set_lv(float lv_value) {
    eez::flow::setGlobalVariable(
        FLOW_GLOBAL_VARIABLE_LV,
        eez::FloatValue(lv_value)
    );
}

extern "C" void ui_set_speed(float speed_kph) {
    (void)speed_kph;
    eez::flow::setGlobalVariable(
        FLOW_GLOBAL_VARIABLE_SPEED,
        eez::FloatValue(speed_kph)
    );
}

// Ethernet link state for the driver_view corner LED. Polled from sysfs
// rather than a ROS2 heartbeat, so it reflects the physical cable/link
// state even if no other node is publishing.
static bool ui_eth_connected = false;

extern "C" void ui_update_network_status() {
    static uint32_t last_check_ms = 0;
    uint32_t now_ms = lv_tick_get();
    if (last_check_ms != 0 && lv_tick_elaps(last_check_ms) < 500) {
        return;
    }
    last_check_ms = now_ms;

    bool carrier_up = false;
    if (FILE *f = fopen("/sys/class/net/eth0/carrier", "r")) {
        int val = 0;
        carrier_up = (fscanf(f, "%d", &val) == 1 && val == 1);
        fclose(f);
    }

    bool operstate_up = false;
    if (FILE *f = fopen("/sys/class/net/eth0/operstate", "r")) {
        char state[16] = {0};
        if (fscanf(f, "%15s", state) == 1) {
            operstate_up = (strcmp(state, "up") == 0);
        }
        fclose(f);
    }

    ui_eth_connected = carrier_up && operstate_up;
}

extern "C" bool ui_is_ethernet_connected() {
    return ui_eth_connected;
}

extern "C" void ui_update_telemetry_vars(const void *t_ptr) {
    if (t_ptr) {
        const TelemetryData *t = static_cast<const TelemetryData *>(t_ptr);
        dbc_api.asf_signals.brake_pressure_front = t->brk_press_f;
        dbc_api.asf_signals.brake_pressure_rear = t->brk_press_r;
        dbc_api.vcu_hv.brake_pressure_front = t->vcu_brkf;
        dbc_api.vcu_hv.brake_pressure_rear = t->vcu_brkr;
        dbc_api.inv1_misc.inv1_actual_brake = t->inv_brake;
        dbc_api.inv1_misc.inv1_actual_throttle = t->inv_throttle;
        dbc_api.master_soc_accumulator.soc_float = t->ams_soc;
        dbc_api.ivt_msg_result_u3.ivt_result_u3 = t->ivt_u3;
        dbc_api.master_msc_id_1.mcu_vref = t->ams_mcu_vref;
        dbc_api.vcu_ign_r2d.r2d_manual = t->vcu_r2d_man;
        dbc_api.vcu_ign_r2d.r2d_auto = t->vcu_r2d_auto;
        dbc_api.acu.acu_state = t->acu_state;
        dbc_api.dv_dynamics_1.speed_actual = t->dv_spd_act;
        dbc_api.inv1_temperatures.inv1_actual_tempcontroller = t->inv_temp_ctrl;
        dbc_api.inv1_temperatures.inv1_actual_tempmotor = t->inv_temp_mot;
        dbc_api.slam_stats_can.lap_counter = t->slam_laps;
        dbc_api.acu.mission_select = t->acu_mission;
    }

    // 1. BRAKE PEDAL PRESSURE (0 to 100)
    float max_brk = dbc_api.asf_signals.brake_pressure_front;
    if (dbc_api.asf_signals.brake_pressure_rear > max_brk) max_brk = dbc_api.asf_signals.brake_pressure_rear;
    if (dbc_api.vcu_hv.brake_pressure_front > max_brk) max_brk = dbc_api.vcu_hv.brake_pressure_front;
    if (dbc_api.vcu_hv.brake_pressure_rear > max_brk) max_brk = dbc_api.vcu_hv.brake_pressure_rear;
    if (dbc_api.inv1_misc.inv1_actual_brake > max_brk) max_brk = dbc_api.inv1_misc.inv1_actual_brake;
    int brake_val = static_cast<int>(max_brk);
    if (brake_val < 0) brake_val = 0;
    if (brake_val > 100) brake_val = 100;
    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_BRAKE_PEDAL_PRESSURE, eez::IntegerValue(brake_val));

    // 2. ACCELL PEDAL PRESSURE (0 to 100) — driven by INV1 target relative current (%)
    int acc_val = static_cast<int>(dbc_api.inv1_setrelcurrent.inv1_cmd_targetrelativecurrent);
    if (acc_val < 0) acc_val = 0;
    if (acc_val > 100) acc_val = 100;
    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_ACCELL_PEDAL_PRESSURE, eez::IntegerValue(acc_val));

    // 3. HV bar (0 to 100) — driven by AMS Master_SOC_Accumulator SOC_Float (%)
    int soc_val = static_cast<int>(dbc_api.master_soc_accumulator.soc_float);
    if (soc_val < 0) soc_val = 0;
    if (soc_val > 100) soc_val = 100;
    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_SOC, eez::IntegerValue(soc_val));

    // 4. LV (Low Voltage, Volts) — PDM_LV LV_Voltage_mV (decoded to Volts),
    // falling back to IVT U3 then AMS MCU_VRef when PDM data is absent.
    float lv_val = dbc_api.pdm_lv.lv_voltage_mv;
    if (lv_val < 5.0f || lv_val > 30.0f) {
        lv_val = dbc_api.ivt_msg_result_u3.ivt_result_u3 / 1000.0f;
    }
    if (lv_val < 5.0f || lv_val > 30.0f) {
        if (dbc_api.master_msc_id_1.mcu_vref >= 5.0f && dbc_api.master_msc_id_1.mcu_vref <= 30.0f) {
            lv_val = dbc_api.master_msc_id_1.mcu_vref;
        } else {
            // No valid LV data received — set to bar minimum (20.0 V) so the
            // indicator appears empty rather than partially filled.
            lv_val = 20.0f;
        }
    }
    ui_lv_value = lv_val;
    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_LV, eez::FloatValue(lv_val));

    // 5. READY (String "READY" / "NOT READY")
    bool is_ready = (dbc_api.vcu_ign_r2d.r2d_manual == 1.0f || 
                    dbc_api.vcu_ign_r2d.r2d_auto == 1.0f || 
                    dbc_api.vcu_ign_r2d.shutdown_signal == 1.0f || 
                    dbc_api.acu.acu_state == 4.0f || 
                    dbc_api.acu.acu_state == 5.0f);
    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_READY, eez::StringValue(is_ready ? "READY" : "NOT READY"));

    // 6. SPEED (km/h)
    float speed_val = dbc_api.dv_dynamics_1.speed_actual >= 0.0f ? dbc_api.dv_dynamics_1.speed_actual : 0.0f;
    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_SPEED, eez::FloatValue(speed_val));

    // 7. TEMP_INV (Inverter Temp, Celsius)
    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_TEMP_INV, eez::FloatValue(dbc_api.inv1_temperatures.inv1_actual_tempcontroller));

    // 8. TEMP_MOTOR (Motor Temp, Celsius)
    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_TEMP_MOTOR, eez::FloatValue(dbc_api.inv1_temperatures.inv1_actual_tempmotor));

    // 9. LAP_COUNT (Integer)
    int laps_val = static_cast<int>(dbc_api.slam_stats_can.lap_counter);
    if (laps_val < 0) laps_val = 0;
    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_LAP_COUNT, eez::IntegerValue(laps_val));

    // 10. MISSION (String "ACCEL", "SKIDPAD", etc.)
    int mission_id = 0;
    if (dbc_api.acu.mission_select > 0.0f) {
        mission_id = static_cast<int>(dbc_api.acu.mission_select);
    } else if (dbc_api.jetson.as_mission > 0.0f) {
        mission_id = static_cast<int>(dbc_api.jetson.as_mission);
    } else if (dbc_api.dv_status.ami_state > 0.0f) {
        mission_id = static_cast<int>(dbc_api.dv_status.ami_state);
    }
    const char *mission_str = ui_get_mission_name(mission_id);
    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_MISSION, eez::StringValue(mission_str));
}

extern "C" const char *ui_get_mission_str() {
    const char *str = eez::flow::getGlobalVariable(FLOW_GLOBAL_VARIABLE_MISSION).getString();
    return str ? str : "";
}

extern "C" const char *ui_get_mission_name(int mission_id) {
    switch (mission_id) {
        case 1: return "ACCEL";
        case 2: return "SKIDPAD";
        case 3: return "TRACKDRIVE";
        case 4: return "BRAKE TEST";
        case 5: return "INSPECTION";
        case 6: return "AUTOCROSS";
        default: return "MANUAL";
    }
}

extern "C" void check_dbc_errors(void (*on_error)(const char *id, const char *msg_name, const char *sig_name, float value, const char *choice_label)) {
    if (!on_error) return;

    {
        float val = dbc_api.master_msc_id_1.adbms_pec_error;
        if (val > 0.5f) {
            on_error("master_msc_id_1.adbms_pec_error", "Master_MSC_ID_1", "adbms_pec_error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.master_msc_id_1.fault_counter;
        if (val > 0.5f) {
            on_error("master_msc_id_1.fault_counter", "Master_MSC_ID_1", "fault_counter", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_wh.ivt_result_wh_system_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_wh.ivt_result_wh_system_error", "IVT_Msg_Result_Wh", "IVT_Result_Wh_System_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_wh.ivt_result_wh_measurement_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_wh.ivt_result_wh_measurement_error", "IVT_Msg_Result_Wh", "IVT_Result_Wh_Measurement_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_wh.ivt_result_wh_channel_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_wh.ivt_result_wh_channel_error", "IVT_Msg_Result_Wh", "IVT_Result_Wh_Channel_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_as.ivt_result_as_system_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_as.ivt_result_as_system_error", "IVT_Msg_Result_As", "IVT_Result_As_System_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_as.ivt_result_as_measurement_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_as.ivt_result_as_measurement_error", "IVT_Msg_Result_As", "IVT_Result_As_Measurement_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_as.ivt_result_as_channel_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_as.ivt_result_as_channel_error", "IVT_Msg_Result_As", "IVT_Result_As_Channel_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_w.ivt_result_w_system_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_w.ivt_result_w_system_error", "IVT_Msg_Result_W", "IVT_Result_W_System_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_w.ivt_result_w_measurement_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_w.ivt_result_w_measurement_error", "IVT_Msg_Result_W", "IVT_Result_W_Measurement_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_w.ivt_result_w_channel_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_w.ivt_result_w_channel_error", "IVT_Msg_Result_W", "IVT_Result_W_Channel_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_response.val_80_resp_measerror_item;
        if (val > 0.5f) {
            on_error("ivt_msg_response.val_80_resp_measerror_item", "IVT_Msg_Response", "_80_Resp_MeasError_Item", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_response.val_81_resp_systemerror_item;
        if (val > 0.5f) {
            on_error("ivt_msg_response.val_81_resp_systemerror_item", "IVT_Msg_Response", "_81_Resp_SystemError_Item", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_response.val_80_resp_measerror_count_mask;
        if (val > 0.5f) {
            on_error("ivt_msg_response.val_80_resp_measerror_count_mask", "IVT_Msg_Response", "_80_Resp_MeasError_Count_Mask", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_response.val_81_resp_systemerror_count_mask;
        if (val > 0.5f) {
            on_error("ivt_msg_response.val_81_resp_systemerror_count_mask", "IVT_Msg_Response", "_81_Resp_SystemError_Count_Mask", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_t.ivt_result_t_system_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_t.ivt_result_t_system_error", "IVT_Msg_Result_T", "IVT_Result_T_System_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_t.ivt_result_t_measurement_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_t.ivt_result_t_measurement_error", "IVT_Msg_Result_T", "IVT_Result_T_Measurement_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_t.ivt_result_t_channel_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_t.ivt_result_t_channel_error", "IVT_Msg_Result_T", "IVT_Result_T_Channel_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_u3.ivt_result_u3_system_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_u3.ivt_result_u3_system_error", "IVT_Msg_Result_U3", "IVT_Result_U3_System_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_u3.ivt_result_u3_measurement_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_u3.ivt_result_u3_measurement_error", "IVT_Msg_Result_U3", "IVT_Result_U3_Measurement_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_u3.ivt_result_u3_channel_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_u3.ivt_result_u3_channel_error", "IVT_Msg_Result_U3", "IVT_Result_U3_Channel_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_u2.ivt_result_u2_system_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_u2.ivt_result_u2_system_error", "IVT_Msg_Result_U2", "IVT_Result_U2_System_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_u2.ivt_result_u2_measurement_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_u2.ivt_result_u2_measurement_error", "IVT_Msg_Result_U2", "IVT_Result_U2_Measurement_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_u2.ivt_result_u2_channel_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_u2.ivt_result_u2_channel_error", "IVT_Msg_Result_U2", "IVT_Result_U2_Channel_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_u1.ivt_result_u1_system_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_u1.ivt_result_u1_system_error", "IVT_Msg_Result_U1", "IVT_Result_U1_System_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_u1.ivt_result_u1_measurement_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_u1.ivt_result_u1_measurement_error", "IVT_Msg_Result_U1", "IVT_Result_U1_Measurement_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_u1.ivt_result_u1_channel_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_u1.ivt_result_u1_channel_error", "IVT_Msg_Result_U1", "IVT_Result_U1_Channel_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_i.ivt_result_i_system_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_i.ivt_result_i_system_error", "IVT_Msg_Result_I", "IVT_Result_I_System_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_i.ivt_result_i_measurement_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_i.ivt_result_i_measurement_error", "IVT_Msg_Result_I", "IVT_Result_I_Measurement_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_result_i.ivt_result_i_channel_error;
        if (val > 0.5f) {
            on_error("ivt_msg_result_i.ivt_result_i_channel_error", "IVT_Msg_Result_I", "IVT_Result_I_Channel_Error", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_cmd.val_3d_restart_default_dummy;
        if (val > 0.5f) {
            on_error("ivt_msg_cmd.val_3d_restart_default_dummy", "IVT_Msg_Cmd", "_3D_Restart_Default_Dummy", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_cmd.val_40_get_measerror_item;
        if (val > 0.5f) {
            on_error("ivt_msg_cmd.val_40_get_measerror_item", "IVT_Msg_Cmd", "_40_Get_MeasError_Item", val, "ERROR");
        }
    }
    {
        float val = dbc_api.ivt_msg_cmd.val_41_get_systemerror_item;
        if (val > 0.5f) {
            on_error("ivt_msg_cmd.val_41_get_systemerror_item", "IVT_Msg_Cmd", "_41_Get_SystemError_Item", val, "ERROR");
        }
    }
    {
        float val = dbc_api.master_msc_id_2.fault1_code;
        int val_int = static_cast<int>(val);
        const char *label = nullptr;
        if (val_int == 0) label = "FAULT_OVERVOLTAGE";
        if (val_int == 1) label = "FAULT_UNDERVOLTAGE";
        if (val_int == 2) label = "FAULT_OVERTEMPERATURE";
        if (val_int == 3) label = "FAULT_UNDERTEMPERATURE";
        if (val_int == 4) label = "FAULT_OVERTEMPERATURE_DISCHARGE";
        if (val_int == 5) label = "FAULT_UNDERTEMPERATURE_CHARGE";
        if (val_int == 6) label = "FAULT_TEMP_SENSOR_OPEN";
        if (val_int == 7) label = "FAULT_SLAVE_NOT_DETECTED";
        if (val_int == 8) label = "FAULT_PEC_ERROR";
        if (val_int == 9) label = "FAULT_OPEN_WIRE";
        if (val_int == 10) label = "FAULT_ACQUISITION_TIMEOUT";
        if (val_int == 11) label = "FAULT_CAN_SEND_ERROR";
        if (val_int == 12) label = "FAULT_CAN_INIT_ERROR";
        if (val_int == 13) label = "FAULT_CAN_MAILBOX_FULL";
        if (val_int == 14) label = "FAULT_CAN_BUS_OFF";
        if (val_int == 15) label = "FAULT_CAN_RECEIVE_ERROR";
        if (val_int == 16) label = "FAULT_INVERTER_TIMEOUT";
        if (val_int == 17) label = "FAULT_VCU_TIMEOUT";
        if (val_int == 18) label = "FAULT_PDM_TIMEOUT";
        if (val_int == 19) label = "FAULT_CHARGER_TIMEOUT";
        if (val_int == 20) label = "FAULT_ACQUISITION_NODE_TIMEOUT";
        if (val_int == 21) label = "FAULT_CONTACTOR_MISMATCH";
        if (val_int == 22) label = "FAULT_PRECHARGE_TIMEOUT";
        if (val_int == 23) label = "FAULT_PRECHARGE_FAILURE";
        if (val_int == 24) label = "FAULT_SDC_TRIGGERED";
        if (val_int == 25) label = "FAULT_BALANCING_ERROR";
        if (val_int == 26) label = "FAULT_BALANCING_OVERTEMP";
        if (val_int == 27) label = "FAULT_ADC_ERROR";
        if (val_int == 28) label = "FAULT_CURRENT_SENSOR_ERROR";
        if (val_int == 29) label = "FAULT_SOC_CRITICAL_LOW";
        if (val_int == 30) label = "FAULT_PACK_VOLTAGE_MISMATCH";
        if (val_int == 31) label = "FAULT_EEPROM_READ_ERROR";
        if (val_int == 32) label = "FAULT_EEPROM_WRITE_ERROR";
        if (val_int == 33) label = "FAULT_EEPROM_VALIDATION_ERROR";
        if (val_int == 34) label = "FAULT_STARTUP_FAILURE";
        if (val_int == 35) label = "FAULT_WATCHDOG_RESET";
        if (val_int == 36) label = "FAULT_STACK_OVERFLOW";
        if (val_int == 37) label = "FAULT_UART1_TX";
        if (val_int == 38) label = "FAULT_UART1_RX";
        if (val_int == 39) label = "FAULT_UART2_TX";
        if (val_int == 40) label = "FAULT_UART2_RX";
        if (val_int == 41) label = "FAULT_OW_DETECTED_CELL";
        if (val_int == 42) label = "FAULT_OW_DETECTED_RTH";
        if (val_int == 255) label = "EMPTY";
        if (label) {
            bool is_safe = false;
            std::string l_lower = label;
            for (auto &c : l_lower) c = std::tolower(c);
            if (l_lower == "none" || l_lower == "empty" || l_lower == "no fault" ||
                l_lower == "ok" || l_lower == "normal" || l_lower == "off" ||
                l_lower == "deactivated" || l_lower == "no open wire" || l_lower == "false") {
                is_safe = true;
            }
            if (!is_safe) {
                on_error("master_msc_id_2.fault1_code", "Master_MSC_ID_2", "fault1_code", val, label);
            }
        }
    }
    {
        float val = dbc_api.master_msc_id_2.fault1_index_type;
        int val_int = static_cast<int>(val);
        const char *label = nullptr;
        if (val_int == 0) label = "SLAVE";
        if (val_int == 1) label = "CELL";
        if (val_int == 2) label = "CHANNEL";
        if (val_int == 3) label = "CONTACTORS";
        if (val_int == 4) label = "SLAVE_AND_CELL";
        if (val_int == 5) label = "THERMISTOR";
        if (val_int == 6) label = "SLAVE_AND_THERMISTOR";
        if (val_int == 7) label = "NONE";
        if (label) {
            bool is_safe = false;
            std::string l_lower = label;
            for (auto &c : l_lower) c = std::tolower(c);
            if (l_lower == "none" || l_lower == "empty" || l_lower == "no fault" ||
                l_lower == "ok" || l_lower == "normal" || l_lower == "off" ||
                l_lower == "deactivated" || l_lower == "no open wire" || l_lower == "false") {
                is_safe = true;
            }
            if (!is_safe) {
                on_error("master_msc_id_2.fault1_index_type", "Master_MSC_ID_2", "fault1_index_type", val, label);
            }
        }
    }
    {
        float val = dbc_api.master_msc_id_2.fault1_index_value;
        if (val > 0.5f) {
            on_error("master_msc_id_2.fault1_index_value", "Master_MSC_ID_2", "fault1_index_value", val, "ERROR");
        }
    }
    {
        float val = dbc_api.master_msc_id_2.fault2_code;
        int val_int = static_cast<int>(val);
        const char *label = nullptr;
        if (val_int == 0) label = "FAULT_OVERVOLTAGE";
        if (val_int == 1) label = "FAULT_UNDERVOLTAGE";
        if (val_int == 2) label = "FAULT_OVERTEMPERATURE";
        if (val_int == 3) label = "FAULT_UNDERTEMPERATURE";
        if (val_int == 4) label = "FAULT_OVERTEMPERATURE_DISCHARGE";
        if (val_int == 5) label = "FAULT_UNDERTEMPERATURE_CHARGE";
        if (val_int == 6) label = "FAULT_TEMP_SENSOR_OPEN";
        if (val_int == 7) label = "FAULT_SLAVE_NOT_DETECTED";
        if (val_int == 8) label = "FAULT_PEC_ERROR";
        if (val_int == 9) label = "FAULT_OPEN_WIRE";
        if (val_int == 10) label = "FAULT_ACQUISITION_TIMEOUT";
        if (val_int == 11) label = "FAULT_CAN_SEND_ERROR";
        if (val_int == 12) label = "FAULT_CAN_INIT_ERROR";
        if (val_int == 13) label = "FAULT_CAN_MAILBOX_FULL";
        if (val_int == 14) label = "FAULT_CAN_BUS_OFF";
        if (val_int == 15) label = "FAULT_CAN_RECEIVE_ERROR";
        if (val_int == 16) label = "FAULT_INVERTER_TIMEOUT";
        if (val_int == 17) label = "FAULT_VCU_TIMEOUT";
        if (val_int == 18) label = "FAULT_PDM_TIMEOUT";
        if (val_int == 19) label = "FAULT_CHARGER_TIMEOUT";
        if (val_int == 20) label = "FAULT_ACQUISITION_NODE_TIMEOUT";
        if (val_int == 21) label = "FAULT_CONTACTOR_MISMATCH";
        if (val_int == 22) label = "FAULT_PRECHARGE_TIMEOUT";
        if (val_int == 23) label = "FAULT_PRECHARGE_FAILURE";
        if (val_int == 24) label = "FAULT_SDC_TRIGGERED";
        if (val_int == 25) label = "FAULT_BALANCING_ERROR";
        if (val_int == 26) label = "FAULT_BALANCING_OVERTEMP";
        if (val_int == 27) label = "FAULT_ADC_ERROR";
        if (val_int == 28) label = "FAULT_CURRENT_SENSOR_ERROR";
        if (val_int == 29) label = "FAULT_SOC_CRITICAL_LOW";
        if (val_int == 30) label = "FAULT_PACK_VOLTAGE_MISMATCH";
        if (val_int == 31) label = "FAULT_EEPROM_READ_ERROR";
        if (val_int == 32) label = "FAULT_EEPROM_WRITE_ERROR";
        if (val_int == 33) label = "FAULT_EEPROM_VALIDATION_ERROR";
        if (val_int == 34) label = "FAULT_STARTUP_FAILURE";
        if (val_int == 35) label = "FAULT_WATCHDOG_RESET";
        if (val_int == 36) label = "FAULT_STACK_OVERFLOW";
        if (val_int == 37) label = "FAULT_UART1_TX";
        if (val_int == 38) label = "FAULT_UART1_RX";
        if (val_int == 39) label = "FAULT_UART2_TX";
        if (val_int == 40) label = "FAULT_UART2_RX";
        if (val_int == 41) label = "FAULT_OW_DETECTED_CELL";
        if (val_int == 42) label = "FAULT_OW_DETECTED_RTH";
        if (val_int == 255) label = "EMPTY";
        if (label) {
            bool is_safe = false;
            std::string l_lower = label;
            for (auto &c : l_lower) c = std::tolower(c);
            if (l_lower == "none" || l_lower == "empty" || l_lower == "no fault" ||
                l_lower == "ok" || l_lower == "normal" || l_lower == "off" ||
                l_lower == "deactivated" || l_lower == "no open wire" || l_lower == "false") {
                is_safe = true;
            }
            if (!is_safe) {
                on_error("master_msc_id_2.fault2_code", "Master_MSC_ID_2", "fault2_code", val, label);
            }
        }
    }
    {
        float val = dbc_api.master_msc_id_2.fault2_index_type;
        int val_int = static_cast<int>(val);
        const char *label = nullptr;
        if (val_int == 0) label = "SLAVE";
        if (val_int == 1) label = "CELL";
        if (val_int == 2) label = "CHANNEL";
        if (val_int == 3) label = "CONTACTORS";
        if (val_int == 4) label = "SLAVE_AND_CELL";
        if (val_int == 5) label = "THERMISTOR";
        if (val_int == 6) label = "SLAVE_AND_THERMISTOR";
        if (val_int == 7) label = "NONE";
        if (label) {
            bool is_safe = false;
            std::string l_lower = label;
            for (auto &c : l_lower) c = std::tolower(c);
            if (l_lower == "none" || l_lower == "empty" || l_lower == "no fault" ||
                l_lower == "ok" || l_lower == "normal" || l_lower == "off" ||
                l_lower == "deactivated" || l_lower == "no open wire" || l_lower == "false") {
                is_safe = true;
            }
            if (!is_safe) {
                on_error("master_msc_id_2.fault2_index_type", "Master_MSC_ID_2", "fault2_index_type", val, label);
            }
        }
    }
    {
        float val = dbc_api.master_msc_id_2.fault2_index_value;
        if (val > 0.5f) {
            on_error("master_msc_id_2.fault2_index_value", "Master_MSC_ID_2", "fault2_index_value", val, "ERROR");
        }
    }
    {
        float val = dbc_api.inv1_temperatures.inv1_actual_faultcode;
        if (val > 0.5f) {
            on_error("inv1_temperatures.inv1_actual_faultcode", "INV1_Temperatures", "INV1_Actual_FaultCode", val, "ERROR");
        }
    }
    {
        float val = dbc_api.inv2_temperatures.inv2_actual_faultcode;
        if (val > 0.5f) {
            on_error("inv2_temperatures.inv2_actual_faultcode", "INV2_Temperatures", "INV2_Actual_FaultCode", val, "ERROR");
        }
    }
    {
        float val = dbc_api.acu.emergency;
        int val_int = static_cast<int>(val);
        const char *label = nullptr;
        if (val_int == 0) label = "OFF";
        if (val_int == 1) label = "ON";
        if (label) {
            bool is_safe = false;
            std::string l_lower = label;
            for (auto &c : l_lower) c = std::tolower(c);
            if (l_lower == "none" || l_lower == "empty" || l_lower == "no fault" ||
                l_lower == "ok" || l_lower == "normal" || l_lower == "off" ||
                l_lower == "deactivated" || l_lower == "no open wire" || l_lower == "false") {
                is_safe = true;
            }
            if (!is_safe) {
                on_error("acu.emergency", "ACU", "EMERGENCY", val, label);
            }
        }
    }
    {
        float val = dbc_api.acu.emergency_cause;
        int val_int = static_cast<int>(val);
        const char *label = nullptr;
        if (val_int == 0) label = "None";
        if (val_int == 1) label = "SDC_OPEN";
        if (val_int == 2) label = "RES";
        if (val_int == 3) label = "Pressure_checks";
        if (val_int == 4) label = "VCU_timeout";
        if (val_int == 5) label = "Jetson_timeout";
        if (val_int == 6) label = "ACU_WDT_TRIGGERED";
        if (val_int == 7) label = "dir_actuator_timeout";
        if (val_int == 8) label = "CAN_Dynamics_Pressure_timeout";
        if (label) {
            bool is_safe = false;
            std::string l_lower = label;
            for (auto &c : l_lower) c = std::tolower(c);
            if (l_lower == "none" || l_lower == "empty" || l_lower == "no fault" ||
                l_lower == "ok" || l_lower == "normal" || l_lower == "off" ||
                l_lower == "deactivated" || l_lower == "no open wire" || l_lower == "false") {
                is_safe = true;
            }
            if (!is_safe) {
                on_error("acu.emergency_cause", "ACU", "EMERGENCY_cause", val, label);
            }
        }
    }
    {
        float val = dbc_api.cubemars_feedback.error_code;
        int val_int = static_cast<int>(val);
        const char *label = nullptr;
        if (val_int == 0) label = "None";
        if (val_int == 1) label = "OVER-VOLTAGE";
        if (val_int == 2) label = "UNDER-VOLTAGE";
        if (val_int == 3) label = "DRIVER-FAULT";
        if (val_int == 4) label = "OVER-CURRENT";
        if (val_int == 5) label = "MOSFET-OVER-TEMP";
        if (val_int == 6) label = "MOTOR-OVER-TEMP";
        if (val_int == 7) label = "GATE-DRIVE-OVER-VOLTAGE";
        if (val_int == 8) label = "GATE-DRIVE-UNDER-VOLTAGE";
        if (val_int == 9) label = "MCU-UNDER-VOLTAGE";
        if (val_int == 10) label = "REBOOTING-FROM-WATCHDOG";
        if (val_int == 11) label = "ENCODER-FAULT";
        if (val_int == 12) label = "MIN-ENCODER-LIMIT-EXCEEDED";
        if (val_int == 13) label = "MAX-ENCODER-LIMIT-EXCEEDED";
        if (val_int == 14) label = "FLASH-FAULT";
        if (val_int == 15) label = "HIGH-OFFSET-CURRENT-SENSOR1";
        if (val_int == 16) label = "HIGH-OFFSET-CURRENT-SENSOR2";
        if (label) {
            bool is_safe = false;
            std::string l_lower = label;
            for (auto &c : l_lower) c = std::tolower(c);
            if (l_lower == "none" || l_lower == "empty" || l_lower == "no fault" ||
                l_lower == "ok" || l_lower == "normal" || l_lower == "off" ||
                l_lower == "deactivated" || l_lower == "no open wire" || l_lower == "false") {
                is_safe = true;
            }
            if (!is_safe) {
                on_error("cubemars_feedback.error_code", "CubeMars_Feedback", "Error_Code", val, label);
            }
        }
    }
    {
        float val = dbc_api.aqt4.emergency;
        if (val > 0.5f) {
            on_error("aqt4.emergency", "AQT4", "EMERGENCY", val, "ERROR");
        }
    }
    {
        float val = dbc_api.jetson.emergency_cause;
        int val_int = static_cast<int>(val);
        const char *label = nullptr;
        if (val_int == 0) label = "None";
        if (val_int == 1) label = "RES";
        if (val_int == 2) label = "ACU";
        if (val_int == 3) label = "ZED";
        if (val_int == 4) label = "Steering_error";
        if (val_int == 5) label = "Steering_timeout";
        if (val_int == 6) label = "Planner";
        if (val_int == 7) label = "SLAM";
        if (val_int == 8) label = "P-Puma";
        if (val_int == 9) label = "IMU";
        if (label) {
            bool is_safe = false;
            std::string l_lower = label;
            for (auto &c : l_lower) c = std::tolower(c);
            if (l_lower == "none" || l_lower == "empty" || l_lower == "no fault" ||
                l_lower == "ok" || l_lower == "normal" || l_lower == "off" ||
                l_lower == "deactivated" || l_lower == "no open wire" || l_lower == "false") {
                is_safe = true;
            }
            if (!is_safe) {
                on_error("jetson.emergency_cause", "JETSON", "EMERGENCY_CAUSE", val, label);
            }
        }
    }
}
