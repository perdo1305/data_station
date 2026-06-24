import os
import re
import glob
import cantools

_UNSAFE = re.compile(r'[^a-zA-Z0-9_]')

def _ros_name(raw: str) -> str:
    """Convert a DBC identifier to a valid, lowercase C/C++ struct member/variable segment."""
    slug = _UNSAFE.sub('_', raw).strip('_').lower()
    if slug and slug[0].isdigit():
        slug = 'val_' + slug
    return slug

def main():
    script_dir = os.path.dirname(os.path.abspath(__file__))
    dbc_dir = os.path.abspath(os.path.join(script_dir, "../../../dbc_signals"))
    
    db = cantools.database.Database()
    dbc_files = sorted(glob.glob(os.path.join(dbc_dir, "*.dbc")))
    for df in dbc_files:
        db.add_dbc_file(df)
        
    # Group signals by sanitized message name slug to deduplicate messages
    message_signals = {}
    for msg in db.messages:
        if not msg.signals:
            continue
        msg_slug = _ros_name(msg.name)
        if msg_slug not in message_signals:
            message_signals[msg_slug] = set()
        for sig in msg.signals:
            message_signals[msg_slug].add(_ros_name(sig.name))
            
    # Generate Header
    header_lines = [
        "// Auto-generated from DBC files by generate_dbc_api.py. Do not edit.",
        "#ifndef LART_DBC_API_H",
        "#define LART_DBC_API_H",
        "",
        "#ifdef __cplusplus",
        "extern \"C\" {",
        "#endif",
        "",
        "typedef struct {"
    ]
    
    for msg_slug in sorted(message_signals.keys()):
        header_lines.append(f"    struct {{")
        for sig_slug in sorted(message_signals[msg_slug]):
            header_lines.append(f"        float {sig_slug};")
        header_lines.append(f"    }} {msg_slug};")
        
    header_lines.extend([
        "} DbcApi;",
        "",
        "extern DbcApi dbc_api;",
        "",
        "#ifdef __cplusplus",
        "}",
        "#if defined(LART_UI_HAVE_RCLCPP) && LART_UI_HAVE_RCLCPP",
        "#include <memory>",
        "#include <vector>",
        "#include <rclcpp/rclcpp.hpp>",
        "void init_dbc_api_subscribers(std::shared_ptr<rclcpp::Node> node, std::vector<rclcpp::SubscriptionBase::SharedPtr>& subs);",
        "#endif",
        "#endif",
        "",
        "#endif // LART_DBC_API_H"
    ])
    
    header_path = os.path.join(script_dir, "dbc_api.h")
    with open(header_path, "w") as f:
        f.write("\n".join(header_lines) + "\n")
    print(f"Generated: {header_path}")
    
    # Generate Source
    source_lines = [
        "// Auto-generated from DBC files by generate_dbc_api.py. Do not edit.",
        "#include \"dbc_api.h\"",
        "",
        "DbcApi dbc_api = {};",
        "",
        "#if defined(LART_UI_HAVE_RCLCPP) && LART_UI_HAVE_RCLCPP",
        "#include <rclcpp/rclcpp.hpp>",
        "#include <std_msgs/msg/float32.hpp>",
        "#include <mutex>",
        "#include <vector>",
        "",
        "static std::mutex dbc_api_mutex;",
        "",
        "void init_dbc_api_subscribers(std::shared_ptr<rclcpp::Node> node, std::vector<rclcpp::SubscriptionBase::SharedPtr>& subs) {",
        "    auto sensor_qos = rclcpp::QoS(10).best_effort();",
        ""
    ]
    
    for msg_slug in sorted(message_signals.keys()):
        for sig_slug in sorted(message_signals[msg_slug]):
            topic = f"/can/dbc/{msg_slug}/{sig_slug}"
            source_lines.extend([
                f"    subs.push_back(node->create_subscription<std_msgs::msg::Float32>(",
                f"        \"{topic}\", sensor_qos, [](const std_msgs::msg::Float32::SharedPtr msg) {{",
                f"            if (msg) {{",
                f"                std::lock_guard<std::mutex> lock(dbc_api_mutex);",
                f"                dbc_api.{msg_slug}.{sig_slug} = msg->data;",
                f"            }}",
                f"        }}));"
            ])
            
    source_lines.extend([
        "}",
        "#endif",
        "",
        "// Combined from power_api.cpp and speed_api.cpp",
        "#include \"ui.h\"",
        "#include \"vars.h\"",
        "#include \"eez-flow.h\"",
        "#include \"ros2subscriber.h\"",
        "#include <cstdint>",
        "",
        "extern \"C\" void ui_set_hv(float hv_value) {",
        "    (void)hv_value;",
        "}",
        "",
        "extern \"C\" void ui_set_lv(float lv_value) {",
        "    eez::flow::setGlobalVariable(",
        "        FLOW_GLOBAL_VARIABLE_LV,",
        "        eez::FloatValue(lv_value)",
        "    );",
        "}",
        "",
        "extern \"C\" void ui_set_speed(float speed_kph) {",
        "    (void)speed_kph;",
        "    eez::flow::setGlobalVariable(",
        "        FLOW_GLOBAL_VARIABLE_SPEED,",
        "        eez::FloatValue(speed_kph)",
        "    );",
        "}",
        "",
        "extern \"C\" void ui_update_telemetry_vars(const void *t_ptr) {",
        "    if (t_ptr) {",
        "        const TelemetryData *t = static_cast<const TelemetryData *>(t_ptr);",
        "        dbc_api.asf_signals.brake_pressure_front = t->brk_press_f;",
        "        dbc_api.asf_signals.brake_pressure_rear = t->brk_press_r;",
        "        dbc_api.vcu_hv.brake_pressure_front = t->vcu_brkf;",
        "        dbc_api.vcu_hv.brake_pressure_rear = t->vcu_brkr;",
        "        dbc_api.hv500_misc.actual_brake = t->inv_brake;",
        "        dbc_api.pedal_box.apps1 = t->apps1;",
        "        dbc_api.pedal_box.apps2 = t->apps2;",
        "        dbc_api.hv500_misc.actual_throttle = t->inv_throttle;",
        "        dbc_api.master_soc_accumulator.soc_float = t->ams_soc;",
        "        dbc_api.ivt_msg_result_u3.ivt_result_u3 = t->ivt_u3;",
        "        dbc_api.master_msc_id_1.mcu_vref = t->ams_mcu_vref;",
        "        dbc_api.vcu_ign_r2d.r2d_manual = t->vcu_r2d_man;",
        "        dbc_api.vcu_ign_r2d.r2d_auto = t->vcu_r2d_auto;",
        "        dbc_api.rear_wheel_l.shutdown_circuit = t->rear_r2d;",
        "        dbc_api.acu.acu_state = t->acu_state;",
        "        dbc_api.dv_dynamics_1.speed_actual = t->dv_spd_act;",
        "        dbc_api.hv500_temperatures.actual_tempcontroller = t->inv_temp_ctrl;",
        "        dbc_api.hv500_temperatures.actual_tempmotor = t->inv_temp_mot;",
        "        dbc_api.slam_stats_can.lap_counter = t->slam_laps;",
        "        dbc_api.acu.mission_select = t->acu_mission;",
        "    }",
        "",
        "    // 1. BRAKE PEDAL PRESSURE (0 to 100)",
        "    float max_brk = dbc_api.asf_signals.brake_pressure_front;",
        "    if (dbc_api.asf_signals.brake_pressure_rear > max_brk) max_brk = dbc_api.asf_signals.brake_pressure_rear;",
        "    if (dbc_api.vcu_hv.brake_pressure_front > max_brk) max_brk = dbc_api.vcu_hv.brake_pressure_front;",
        "    if (dbc_api.vcu_hv.brake_pressure_rear > max_brk) max_brk = dbc_api.vcu_hv.brake_pressure_rear;",
        "    if (dbc_api.hv500_misc.actual_brake > max_brk) max_brk = dbc_api.hv500_misc.actual_brake;",
        "    int brake_val = static_cast<int>(max_brk);",
        "    if (brake_val < 0) brake_val = 0;",
        "    if (brake_val > 100) brake_val = 100;",
        "    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_BRAKE_PEDAL_PRESSURE, eez::IntegerValue(brake_val));",
        "",
        "    // 2. ACCELL PEDAL PRESSURE (0 to 100)",
        "    float max_acc = dbc_api.pedal_box.apps1;",
        "    if (dbc_api.pedal_box.apps2 > max_acc) max_acc = dbc_api.pedal_box.apps2;",
        "    if (dbc_api.hv500_misc.actual_throttle > max_acc) max_acc = dbc_api.hv500_misc.actual_throttle;",
        "    int acc_val = static_cast<int>(max_acc);",
        "    if (acc_val < 0) acc_val = 0;",
        "    if (acc_val > 100) acc_val = 100;",
        "    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_ACCELL_PEDAL_PRESSURE, eez::IntegerValue(acc_val));",
        "",
        "    // 3. SOC (State of Charge, 0 to 100)",
        "    int soc_val = static_cast<int>(dbc_api.master_soc_accumulator.soc_float);",
        "    if (soc_val < 0) soc_val = 0;",
        "    if (soc_val > 100) soc_val = 100;",
        "    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_SOC, eez::IntegerValue(soc_val));",
        "",
        "    // 4. LV (Low Voltage, Volts)",
        "    float lv_val = dbc_api.ivt_msg_result_u3.ivt_result_u3 / 1000.0f;",
        "    if (lv_val < 20.0f || lv_val > 28.0f) {",
        "        if (dbc_api.master_msc_id_1.mcu_vref >= 20.0f && dbc_api.master_msc_id_1.mcu_vref <= 28.0f) {",
        "            lv_val = dbc_api.master_msc_id_1.mcu_vref;",
        "        } else {",
        "            lv_val = 24.0f;",
        "        }",
        "    }",
        "    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_LV, eez::FloatValue(lv_val));",
        "",
        "    // 5. READY (String \"READY\" / \"NOT READY\")",
        "    bool is_ready = (dbc_api.vcu_ign_r2d.r2d_manual == 1.0f || ",
        "                    dbc_api.vcu_ign_r2d.r2d_auto == 1.0f || ",
        "                    dbc_api.rear_wheel_l.shutdown_circuit == 1.0f || ",
        "                    dbc_api.acu.acu_state == 4.0f || ",
        "                    dbc_api.acu.acu_state == 5.0f);",
        "    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_READY, eez::StringValue(is_ready ? \"READY\" : \"NOT READY\"));",
        "",
        "    // 6. SPEED (km/h)",
        "    float speed_val = dbc_api.dv_dynamics_1.speed_actual >= 0.0f ? dbc_api.dv_dynamics_1.speed_actual : 0.0f;",
        "    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_SPEED, eez::FloatValue(speed_val));",
        "",
        "    // 7. TEMP_INV (Inverter Temp, Celsius)",
        "    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_TEMP_INV, eez::FloatValue(dbc_api.hv500_temperatures.actual_tempcontroller));",
        "",
        "    // 8. TEMP_MOTOR (Motor Temp, Celsius)",
        "    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_TEMP_MOTOR, eez::FloatValue(dbc_api.hv500_temperatures.actual_tempmotor));",
        "",
        "    // 9. LAP_COUNT (Integer)",
        "    int laps_val = static_cast<int>(dbc_api.slam_stats_can.lap_counter);",
        "    if (laps_val < 0) laps_val = 0;",
        "    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_LAP_COUNT, eez::IntegerValue(laps_val));",
        "",
        "    // 10. MISSION (String \"ACCEL\", \"SKIDPAD\", etc.)",
        "    int mission_id = 0;",
        "    if (dbc_api.acu.mission_select > 0.0f) {",
        "        mission_id = static_cast<int>(dbc_api.acu.mission_select);",
        "    } else if (dbc_api.jetson.as_mission > 0.0f) {",
        "        mission_id = static_cast<int>(dbc_api.jetson.as_mission);",
        "    } else if (dbc_api.dv_status.ami_state > 0.0f) {",
        "        mission_id = static_cast<int>(dbc_api.dv_status.ami_state);",
        "    }",
        "    const char *mission_str = ui_get_mission_name(mission_id);",
        "    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_MISSION, eez::StringValue(mission_str));",
        "}",
        "",
        "extern \"C\" const char *ui_get_mission_str() {",
        "    const char *str = eez::flow::getGlobalVariable(FLOW_GLOBAL_VARIABLE_MISSION).getString();",
        "    return str ? str : \"\";",
        "}",
        "",
        "extern \"C\" const char *ui_get_mission_name(int mission_id) {",
        "    switch (mission_id) {",
        "        case 1: return \"ACCEL\";",
        "        case 2: return \"SKIDPAD\";",
        "        case 3: return \"TRACKDRIVE\";",
        "        case 4: return \"BRAKE TEST\";",
        "        case 5: return \"INSPECTION\";",
        "        case 6: return \"AUTOCROSS\";",
        "        default: return \"NONE\";",
        "    }",
        "}"
    ])

    source_path = os.path.join(script_dir, "dbc_api.cpp")
    with open(source_path, "w") as f:
        f.write("\n".join(source_lines) + "\n")
    print(f"Generated: {source_path}")

if __name__ == "__main__":
    main()
