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

def _ros_header_name(class_name: str) -> str:
    s1 = re.sub('(.)([A-Z][a-z]+)', r'\1_\2', class_name)
    return re.sub('([a-z0-9])([A-Z])', r'\1_\2', s1).lower()

def main():
    script_dir = os.path.dirname(os.path.abspath(__file__))
    dbc_dir = os.path.abspath(os.path.join(script_dir, "../../../dbc_signals"))
    
    # Must match the file set loaded by generated/generate_can_bridge.py so the UI only
    # subscribes to topics that can_bridge actually publishes. aquisition_boards.dbc is the
    # pre-reorg file superseded by the AQT1-AQT8 messages in data_t26.dbc/autonomous_t26.dbc;
    # including it here reintroduces legacy topics (front_wheel_l, pedal_box, ...) that clutter
    # `ros2 topic list`.
    dbc_filenames = ["data_t26.dbc", "powertrain_t26.dbc", "autonomous_t26.dbc"]

    db = cantools.database.Database()
    dbc_files = [os.path.join(dbc_dir, fn) for fn in dbc_filenames]
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
            
    # 1. Generate ROS 2 message files and update lart_msgs/CMakeLists.txt
    lart_msgs_dir = os.path.abspath(os.path.join(script_dir, "../../../src/lart_msgs"))
    msg_dir = os.path.join(lart_msgs_dir, "msg")
    os.makedirs(msg_dir, exist_ok=True)
    
    generated_msg_files = []
    for msg_slug, sig_slugs in message_signals.items():
        class_name = ''.join(word.capitalize() for word in msg_slug.split('_') if word)
        msg_filename = f"{class_name}.msg"
        msg_path = os.path.join(msg_dir, msg_filename)
        
        lines = []
        for sig_slug in sorted(list(sig_slugs)):
            lines.append(f"float32 {sig_slug}")
            
        with open(msg_path, "w") as mf:
            mf.write("\n".join(lines) + "\n")
        generated_msg_files.append(msg_filename)
        
    cmake_path = os.path.join(lart_msgs_dir, "CMakeLists.txt")
    standard_msgs = [
        "msg/ButtonEvent.msg",
        "msg/EncoderDelta.msg",
        "msg/CanFrame.msg",
        "msg/DashboardState.msg"
    ]
    all_msgs = standard_msgs + [f"msg/{mf}" for mf in sorted(generated_msg_files)]
    
    cmake_lines = [
        "cmake_minimum_required(VERSION 3.8)",
        "project(lart_msgs)",
        "",
        "find_package(ament_cmake REQUIRED)",
        "find_package(rosidl_default_generators REQUIRED)",
        "find_package(builtin_interfaces REQUIRED)",
        "",
        "rosidl_generate_interfaces(${PROJECT_NAME}"
    ]
    for m in all_msgs:
        cmake_lines.append(f'  "{m}"')
    cmake_lines.extend([
        "  DEPENDENCIES builtin_interfaces",
        ")",
        "",
        "ament_package()"
    ])
    with open(cmake_path, "w") as cf:
        cf.write("\n".join(cmake_lines) + "\n")
    print(f"Generated {len(generated_msg_files)} .msg files and updated CMakeLists.txt")
            
    # Identify error signals for checking
    error_signals = []
    for msg in db.messages:
        if not msg.signals:
            continue
        msg_slug = _ros_name(msg.name)
        for sig in msg.signals:
            sig_slug = _ros_name(sig.name)
            if any(k in sig_slug.lower() for k in ('error', 'fault', 'emergency', 'fail')):
                error_signals.append({
                    'msg_name': msg.name,
                    'sig_name': sig.name,
                    'msg_slug': msg_slug,
                    'sig_slug': sig_slug,
                    'choices': getattr(sig, 'choices', None)
                })

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
        "void check_dbc_errors(void (*on_error)(const char *id, const char *msg_name, const char *sig_name, float value, const char *choice_label));",
        "",
        "#ifdef __cplusplus",
        "}",
        "#if defined(LART_UI_HAVE_RCLCPP) && LART_UI_HAVE_RCLCPP",
        "#if defined(LART_HAVE_LART_MSGS) && LART_HAVE_LART_MSGS",
        "#include <memory>",
        "#include <vector>",
        "#include <rclcpp/rclcpp.hpp>",
        "void init_dbc_api_subscribers(std::shared_ptr<rclcpp::Node> node, std::vector<rclcpp::SubscriptionBase::SharedPtr>& subs);",
        "#endif",
        "#endif",
        "#endif",
        "",
        "#endif // LART_DBC_API_H"
    ])
    
    header_path = os.path.join(script_dir, "dbc_api.h")
    with open(header_path, "w") as f:
        f.write("\n".join(header_lines) + "\n")
    print(f"Generated: {header_path}")
    
    # Clean up old sub-chunk files
    for old_file in glob.glob(os.path.join(script_dir, "dbc_api_sub_*.cpp")):
        try:
            os.remove(old_file)
        except Exception:
            pass

    sorted_slugs = sorted(message_signals.keys())
    chunk_size = 30
    chunks = [sorted_slugs[i:i + chunk_size] for i in range(0, len(sorted_slugs), chunk_size)]

    for idx, chunk_slugs in enumerate(chunks):
        sub_lines = [
            "// Auto-generated from DBC files by generate_dbc_api.py. Do not edit.",
            "#include \"dbc_api.h\"",
            "#if defined(LART_UI_HAVE_RCLCPP) && LART_UI_HAVE_RCLCPP",
            "#if defined(LART_HAVE_LART_MSGS) && LART_HAVE_LART_MSGS",
            "#include <rclcpp/rclcpp.hpp>",
            "#include <mutex>",
            "#include <vector>",
            "extern std::mutex dbc_api_mutex;"
        ]
        
        for msg_slug in sorted(chunk_slugs):
            class_name = ''.join(word.capitalize() for word in msg_slug.split('_') if word)
            header_name = _ros_header_name(class_name)
            sub_lines.append(f'#include <lart_msgs/msg/{header_name}.hpp>')
            
        sub_lines.extend([
            "",
            f"void init_dbc_api_subscribers_chunk_{idx}(std::shared_ptr<rclcpp::Node> node, std::vector<rclcpp::SubscriptionBase::SharedPtr>& subs) {{",
            "    auto sensor_qos = rclcpp::QoS(10).best_effort();",
            ""
        ])
        
        for msg_slug in sorted(chunk_slugs):
            class_name = ''.join(word.capitalize() for word in msg_slug.split('_') if word)
            msg_class = f"lart_msgs::msg::{class_name}"
            topic = f"/can/dbc/{msg_slug}"
            sub_lines.extend([
                f"    subs.push_back(node->create_subscription<{msg_class}>(",
                f"        \"{topic}\", sensor_qos, [](const std::shared_ptr<{msg_class}> msg) {{",
                f"            if (msg) {{",
                f"                std::lock_guard<std::mutex> lock(dbc_api_mutex);"
            ])
            for sig_slug in sorted(message_signals[msg_slug]):
                sub_lines.append(f"                dbc_api.{msg_slug}.{sig_slug} = msg->{sig_slug};")
            sub_lines.extend([
                f"            }}",
                f"        }}));"
            ])
            
        sub_lines.extend([
            "}",
            "#endif",
            "#endif"
        ])
        
        sub_path = os.path.join(script_dir, f"dbc_api_sub_{idx}.cpp")
        with open(sub_path, "w") as sf:
            sf.write("\n".join(sub_lines) + "\n")
        print(f"Generated: {sub_path}")

    # Generate Source (dbc_api.cpp)
    source_lines = [
        "// Auto-generated from DBC files by generate_dbc_api.py. Do not edit.",
        "#include \"dbc_api.h\"",
        "#include <string>",
        "#include <cctype>",
        "",
        "DbcApi dbc_api = {};",
        "",
        "#if defined(LART_UI_HAVE_RCLCPP) && LART_UI_HAVE_RCLCPP",
        "#if defined(LART_HAVE_LART_MSGS) && LART_HAVE_LART_MSGS",
        "#include <rclcpp/rclcpp.hpp>",
        "#include <mutex>",
        "#include <vector>",
        "",
        "std::mutex dbc_api_mutex;"
    ]
    
    for idx in range(len(chunks)):
        source_lines.append(f"extern void init_dbc_api_subscribers_chunk_{idx}(std::shared_ptr<rclcpp::Node> node, std::vector<rclcpp::SubscriptionBase::SharedPtr>& subs);")
        
    source_lines.extend([
        "",
        "void init_dbc_api_subscribers(std::shared_ptr<rclcpp::Node> node, std::vector<rclcpp::SubscriptionBase::SharedPtr>& subs) {",
    ])
    
    for idx in range(len(chunks)):
        source_lines.append(f"    init_dbc_api_subscribers_chunk_{idx}(node, subs);")
        
    source_lines.extend([
        "}",
        "#endif",
        "#endif",
        "",
        "// Combined from power_api.cpp and speed_api.cpp",
        "#include \"ui.h\"",
        "#include \"vars.h\"",
        "#include \"eez-flow.h\"",
        "#include \"ros2subscriber.h\"",
        "#include <cstdint>",
        "",
        "// Last LV value shown on the dashboard (Volts) — used by the screen tick",
        "// code to render the LV label with one decimal.",
        "static float ui_lv_value = 20.0f;",
        "",
        "extern \"C\" const char *ui_get_lv_str() {",
        "    static char buf[16];",
        "    snprintf(buf, sizeof(buf), \"%.1f V\", ui_lv_value);",
        "    return buf;",
        "}",
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
        "        dbc_api.inv1_misc.inv1_actual_brake = t->inv_brake;",
        "        dbc_api.inv1_misc.inv1_actual_throttle = t->inv_throttle;",
        "        dbc_api.master_soc_accumulator.soc_float = t->ams_soc;",
        "        dbc_api.ivt_msg_result_u3.ivt_result_u3 = t->ivt_u3;",
        "        dbc_api.master_msc_id_1.mcu_vref = t->ams_mcu_vref;",
        "        dbc_api.vcu_ign_r2d.r2d_manual = t->vcu_r2d_man;",
        "        dbc_api.vcu_ign_r2d.r2d_auto = t->vcu_r2d_auto;",
        "        dbc_api.acu.acu_state = t->acu_state;",
        "        dbc_api.dv_dynamics_1.speed_actual = t->dv_spd_act;",
        "        dbc_api.inv1_temperatures.inv1_actual_tempcontroller = t->inv_temp_ctrl;",
        "        dbc_api.inv1_temperatures.inv1_actual_tempmotor = t->inv_temp_mot;",
        "        dbc_api.slam_stats_can.lap_counter = t->slam_laps;",
        "        dbc_api.acu.mission_select = t->acu_mission;",
        "    }",
        "",
        "    // 1. BRAKE PEDAL PRESSURE (0 to 100)",
        "    float max_brk = dbc_api.asf_signals.brake_pressure_front;",
        "    if (dbc_api.asf_signals.brake_pressure_rear > max_brk) max_brk = dbc_api.asf_signals.brake_pressure_rear;",
        "    if (dbc_api.vcu_hv.brake_pressure_front > max_brk) max_brk = dbc_api.vcu_hv.brake_pressure_front;",
        "    if (dbc_api.vcu_hv.brake_pressure_rear > max_brk) max_brk = dbc_api.vcu_hv.brake_pressure_rear;",
        "    if (dbc_api.inv1_misc.inv1_actual_brake > max_brk) max_brk = dbc_api.inv1_misc.inv1_actual_brake;",
        "    int brake_val = static_cast<int>(max_brk);",
        "    if (brake_val < 0) brake_val = 0;",
        "    if (brake_val > 100) brake_val = 100;",
        "    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_BRAKE_PEDAL_PRESSURE, eez::IntegerValue(brake_val));",
        "",
        "    // 2. ACCELL PEDAL PRESSURE (0 to 100) — driven by INV1 target relative current (%)",
        "    int acc_val = static_cast<int>(dbc_api.inv1_setrelcurrent.inv1_cmd_targetrelativecurrent);",
        "    if (acc_val < 0) acc_val = 0;",
        "    if (acc_val > 100) acc_val = 100;",
        "    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_ACCELL_PEDAL_PRESSURE, eez::IntegerValue(acc_val));",
        "",
        "    // 3. HV bar (0 to 100) — driven by AMS Master_SOC_Accumulator SOC_Float (%)",
        "    int soc_val = static_cast<int>(dbc_api.master_soc_accumulator.soc_float);",
        "    if (soc_val < 0) soc_val = 0;",
        "    if (soc_val > 100) soc_val = 100;",
        "    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_SOC, eez::IntegerValue(soc_val));",
        "",
        "    // 4. LV (Low Voltage, Volts) — PDM_LV LV_Voltage_mV (decoded to Volts),",
        "    // falling back to IVT U3 then AMS MCU_VRef when PDM data is absent.",
        "    float lv_val = dbc_api.pdm_lv.lv_voltage_mv;",
        "    if (lv_val < 5.0f || lv_val > 30.0f) {",
        "        lv_val = dbc_api.ivt_msg_result_u3.ivt_result_u3 / 1000.0f;",
        "    }",
        "    if (lv_val < 5.0f || lv_val > 30.0f) {",
        "        if (dbc_api.master_msc_id_1.mcu_vref >= 5.0f && dbc_api.master_msc_id_1.mcu_vref <= 30.0f) {",
        "            lv_val = dbc_api.master_msc_id_1.mcu_vref;",
        "        } else {",
        "            // No valid LV data received — set to bar minimum (20.0 V) so the",
        "            // indicator appears empty rather than partially filled.",
        "            lv_val = 20.0f;",
        "        }",
        "    }",
        "    ui_lv_value = lv_val;",
        "    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_LV, eez::FloatValue(lv_val));",
        "",
        "    // 5. READY (String \"READY\" / \"NOT READY\")",
        "    bool is_ready = (dbc_api.vcu_ign_r2d.r2d_manual == 1.0f || ",
        "                    dbc_api.vcu_ign_r2d.r2d_auto == 1.0f || ",
        "                    dbc_api.vcu_ign_r2d.shutdown_signal == 1.0f || ",
        "                    dbc_api.acu.acu_state == 4.0f || ",
        "                    dbc_api.acu.acu_state == 5.0f);",
        "    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_READY, eez::StringValue(is_ready ? \"READY\" : \"NOT READY\"));",
        "",
        "    // 6. SPEED (km/h)",
        "    float speed_val = dbc_api.dv_dynamics_1.speed_actual >= 0.0f ? dbc_api.dv_dynamics_1.speed_actual : 0.0f;",
        "    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_SPEED, eez::FloatValue(speed_val));",
        "",
        "    // 7. TEMP_INV (Inverter Temp, Celsius)",
        "    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_TEMP_INV, eez::FloatValue(dbc_api.inv1_temperatures.inv1_actual_tempcontroller));",
        "",
        "    // 8. TEMP_MOTOR (Motor Temp, Celsius)",
        "    eez::flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_TEMP_MOTOR, eez::FloatValue(dbc_api.inv1_temperatures.inv1_actual_tempmotor));",
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
        "        default: return \"MANUAL\";",
        "    }",
        "}"
    ])

    error_checking_lines = [
        "",
        "extern \"C\" void check_dbc_errors(void (*on_error)(const char *id, const char *msg_name, const char *sig_name, float value, const char *choice_label)) {",
        "    if (!on_error) return;",
        ""
    ]
    
    for err in error_signals:
        msg_slug = err['msg_slug']
        sig_slug = err['sig_slug']
        msg_name = err['msg_name'].replace('"', '\\"')
        sig_name = err['sig_name'].replace('"', '\\"')
        sig_id = f"{msg_slug}.{sig_slug}"
        
        error_checking_lines.append(f"    {{")
        error_checking_lines.append(f"        float val = dbc_api.{msg_slug}.{sig_slug};")
        
        if err['choices'] and len(err['choices']) > 0:
            error_checking_lines.append(f"        int val_int = static_cast<int>(val);")
            error_checking_lines.append(f"        const char *label = nullptr;")
            for val_code, choice in err['choices'].items():
                choice_escaped = str(choice).replace('"', '\\"')
                error_checking_lines.append(f"        if (val_int == {val_code}) label = \"{choice_escaped}\";")
            
            error_checking_lines.extend([
                "        if (label) {",
                "            bool is_safe = false;",
                "            std::string l_lower = label;",
                "            for (auto &c : l_lower) c = std::tolower(c);",
                "            if (l_lower == \"none\" || l_lower == \"empty\" || l_lower == \"no fault\" ||",
                "                l_lower == \"ok\" || l_lower == \"normal\" || l_lower == \"off\" ||",
                "                l_lower == \"deactivated\" || l_lower == \"no open wire\" || l_lower == \"false\") {",
                "                is_safe = true;",
                "            }",
                "            if (!is_safe) {",
                f"                on_error(\"{sig_id}\", \"{msg_name}\", \"{sig_name}\", val, label);",
                "            }",
                "        }",
            ])
        else:
            error_checking_lines.extend([
                "        if (val > 0.5f) {",
                f"            on_error(\"{sig_id}\", \"{msg_name}\", \"{sig_name}\", val, \"ERROR\");",
                "        }"
            ])
        error_checking_lines.append(f"    }}")
        
    error_checking_lines.append("}")
    
    source_lines.extend(error_checking_lines)

    source_path = os.path.join(script_dir, "dbc_api.cpp")
    with open(source_path, "w") as f:
        f.write("\n".join(source_lines) + "\n")
    print(f"Generated: {source_path}")

if __name__ == "__main__":
    main()
