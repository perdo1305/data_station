import can
import time

try:
    bus = can.interface.Bus(channel='/dev/ttyACM1', interface='slcan', bitrate=1000000)
    print("Opened bus")
    msg = can.Message(arbitration_id=0x123, data=[1, 2, 3, 4], is_extended_id=False)
    for i in range(100):
        bus.send(msg)
        time.sleep(0.001)
    print("Sent 100 messages")
    bus.shutdown()
except Exception as e:
    print(f"Error: {e}")
