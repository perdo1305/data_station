import can
try:
    bus = can.interface.Bus(channel='/dev/ttyACM0', interface='slcan', bitrate=1000000)
    print("Success!")
    bus.shutdown()
except Exception as e:
    print(f"Error: {e}")
