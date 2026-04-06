import os
import re
import cantools

# Path to your DBC file
dbc_path = '/home/andre-lopes/Desktop/ros2_ws/src/can_bridge/include/CAN_DBC/Autonomous_temporary.dbc'

# Get directory of this script
script_dir = os.path.dirname(os.path.abspath(__file__))

# Output folder = ./msg relative to this script
output_dir = os.path.join(script_dir, "msg")
os.makedirs(output_dir, exist_ok=True)

# Load DBC
db = cantools.database.load_file(dbc_path)

def to_camel_case(name):
    """Convert string with spaces or hyphens to CamelCase."""
    parts = re.split(r'[-_\s]+', name)
    return ''.join(word.capitalize() for word in parts if word)

def dbc_to_rosmsg(message):
    lines = ["std_msgs/Header header\n"]  # Always include header

    for signal in message.signals:
        # Determine ROS data type
        if signal.scale != 1 or signal.offset != 0:
            dtype = "float32"
        elif signal.length <= 8:
            dtype = "int8"
        elif signal.length <= 16:
            dtype = "int16"
        elif signal.length <= 32:
            dtype = "int32"
        else:
            dtype = "int64"

        # Clean up signal name for ROS compatibility (lowercase)
        name = signal.name.replace(" ", "_").replace("-", "_").lower()

        lines.append(f"{dtype} {name}")

    return "\n".join(lines) + "\n"

# Generate one .msg per CAN message
for message in db.messages:
    # Use CamelCase for filename
    file_name = f"{to_camel_case(message.name)}.msg"
    file_path = os.path.join(output_dir, file_name)

    with open(file_path, "w") as f:
        f.write(dbc_to_rosmsg(message))
    
    print(f"✅ Generated {file_path}")

print(f"\nAll .msg files are in: {output_dir}")

