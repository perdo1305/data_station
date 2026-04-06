"""Mock CAN publisher for home / bench testing.

Simulates ECU CAN traffic without any real hardware.
Publishes synthetic frames that match what can_bridge would receive on the car,
so all downstream nodes (led_controller, dashboard_ui) work identically.

Published topics:
  /can/frames   (lart_msgs/CanFrame)
  /vehicle/rpm  (std_msgs/Float32)
  /vehicle/dashboard_state (lart_msgs/DashboardState)

Simulation cycle (configurable via parameters):
  0 – 2 s  : idle RPM
  2 – 7 s  : linear sweep idle → rpm_max
  7 – 8 s  : hold rpm_max
  repeat

RPM is encoded big-endian unsigned in bytes [0:2] of the CAN frame,
matching the default can_bridge.rpm_start_byte / rpm_length_bytes settings.
"""

import rclpy
from rclpy.node import Node
from std_msgs.msg import Float32
from lart_msgs.msg import CanFrame, DashboardState


class MockCanNode(Node):
    def __init__(self):
        super().__init__('mock_can')

        self.declare_parameter('rpm_can_id', 291)        # 0x123
        self.declare_parameter('rpm_idle', 1000.0)
        self.declare_parameter('rpm_max', 12500.0)
        self.declare_parameter('rpm_shift', 11500.0)
        self.declare_parameter('sweep_period_s', 5.0)
        self.declare_parameter('publish_hz', 100.0)

        self._rpm_can_id = self.get_parameter('rpm_can_id').value
        self._rpm_idle = self.get_parameter('rpm_idle').value
        self._rpm_max = self.get_parameter('rpm_max').value
        self._sweep_s = self.get_parameter('sweep_period_s').value

        hz = self.get_parameter('publish_hz').value
        self._dt = 1.0 / hz
        self._t = 0.0

        self._frame_pub = self.create_publisher(CanFrame, '/can/frames', 10)
        self._rpm_pub = self.create_publisher(Float32, '/vehicle/rpm', 10)
        self._state_pub = self.create_publisher(
            DashboardState, '/vehicle/dashboard_state', 10
        )

        self.create_timer(self._dt, self._tick)
        self.get_logger().info(
            f'Mock CAN publisher running at {hz:.0f} Hz '
            f'(RPM ID=0x{self._rpm_can_id:03X})'
        )

    # ------------------------------------------------------------------

    def _tick(self) -> None:
        self._t += self._dt
        rpm = self._compute_rpm(self._t)

        # Encode RPM as big-endian uint16 in payload bytes 0-1
        raw = min(int(rpm), 0xFFFF)
        payload = [(raw >> 8) & 0xFF, raw & 0xFF, 0, 0, 0, 0, 0, 0]

        frame = CanFrame()
        frame.stamp = self.get_clock().now().to_msg()
        frame.can_id = self._rpm_can_id
        frame.dlc = 8
        frame.data = payload
        self._frame_pub.publish(frame)

        rpm_msg = Float32()
        rpm_msg.data = float(rpm)
        self._rpm_pub.publish(rpm_msg)

        self._publish_dashboard_state(rpm)

    def _compute_rpm(self, t: float) -> float:
        """Cycle: 2 s idle | sweep_period_s sweep | 1 s hold max | repeat."""
        cycle = 2.0 + self._sweep_s + 1.0
        phase = t % cycle
        if phase < 2.0:
            return self._rpm_idle
        elif phase < 2.0 + self._sweep_s:
            frac = (phase - 2.0) / self._sweep_s
            return self._rpm_idle + (self._rpm_max - self._rpm_idle) * frac
        else:
            return self._rpm_max

    def _publish_dashboard_state(self, rpm: float) -> None:
        now = self.get_clock().now().to_msg()

        # Legacy-compatible speed conversion used in the old dashboard script.
        speed_kph = rpm * 0.02454

        # Light synthetic dynamics for MVP UI testing.
        t = self._t
        inv_temp = 42.0 + 18.0 * (rpm / max(self._rpm_max, 1.0))
        motor_temp = 39.0 + 22.0 * (rpm / max(self._rpm_max, 1.0))
        bps = min(100.0, max(0.0, 8.0 + (rpm / max(self._rpm_max, 1.0)) * 82.0))
        kw_inst = (rpm / max(self._rpm_max, 1.0)) * 62.0
        kw_limit = 75.0

        # SoC values evolve slowly, stay inside valid [0, 100].
        soc_hv = max(10.0, 82.0 - 0.15 * t)
        soc_lv = max(20.0, 78.0 - 0.06 * t)
        lv_voltage = 20.0 + (soc_lv / 100.0) * (28.8 - 20.0)

        # Keep R2D false during idle phase, true once RPM sweep starts.
        cycle = 2.0 + self._sweep_s + 1.0
        phase = t % cycle
        r2d_ready = phase >= 2.0

        # Simulate brief CAN inactivity dropouts every 9 seconds.
        can_active = (t % 9.0) < 8.2

        msg = DashboardState()
        msg.stamp = now
        msg.rpm = float(rpm)
        msg.speed_kph = float(speed_kph)
        msg.inv_temp_c = float(inv_temp)
        msg.motor_temp_c = float(motor_temp)
        msg.bps = float(bps)
        msg.kw_inst = float(kw_inst)
        msg.kw_limit = float(kw_limit)
        msg.soc_lv = float(soc_lv)
        msg.soc_hv = float(soc_hv)
        msg.lv_voltage = float(lv_voltage)
        msg.r2d_ready = bool(r2d_ready)
        msg.can_active = bool(can_active)
        self._state_pub.publish(msg)


def main(args=None):
    rclpy.init(args=args)
    node = MockCanNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.try_shutdown()


if __name__ == '__main__':
    main()
