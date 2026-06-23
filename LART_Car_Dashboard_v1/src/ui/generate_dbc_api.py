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
        "#endif"
    ])
    
    source_path = os.path.join(script_dir, "dbc_api.cpp")
    with open(source_path, "w") as f:
        f.write("\n".join(source_lines) + "\n")
    print(f"Generated: {source_path}")

if __name__ == "__main__":
    main()
