import importlib
import sys
import types


class _Parameter:
    def __init__(self, value):
        self.value = value


class _Logger:
    def warn(self, _message):
        pass

    def info(self, _message):
        pass


class _Node:
    def __init__(self, _name):
        self.parameters = {}

    def declare_parameter(self, name, default):
        self.parameters[name] = default

    def get_parameter(self, name):
        return _Parameter(self.parameters[name])

    def create_subscription(self, *_args):
        pass

    def get_logger(self):
        return _Logger()


class _NeoPixel:
    def __init__(self, _pin, count, **_kwargs):
        self.values = [None] * count
        self.show_count = 0

    def fill(self, color):
        self.values[:] = [color] * len(self.values)

    def show(self):
        self.show_count += 1


def test_controller_turns_all_eight_leds_green(monkeypatch):
    rclpy = types.ModuleType("rclpy")
    rclpy_node = types.ModuleType("rclpy.node")
    rclpy_node.Node = _Node
    std_msgs = types.ModuleType("std_msgs")
    std_msgs_msg = types.ModuleType("std_msgs.msg")
    std_msgs_msg.Float32 = type("Float32", (), {})
    board = types.ModuleType("board")
    board.D18 = object()
    neopixel = types.ModuleType("neopixel")
    neopixel.NeoPixel = _NeoPixel

    monkeypatch.setitem(sys.modules, "rclpy", rclpy)
    monkeypatch.setitem(sys.modules, "rclpy.node", rclpy_node)
    monkeypatch.setitem(sys.modules, "std_msgs", std_msgs)
    monkeypatch.setitem(sys.modules, "std_msgs.msg", std_msgs_msg)
    monkeypatch.setitem(sys.modules, "board", board)
    monkeypatch.setitem(sys.modules, "neopixel", neopixel)

    sys.modules.pop("led_controller.led_node", None)
    led_node = importlib.import_module("led_controller.led_node")
    node = led_node.LedControllerNode()

    assert len(node._pixels.values) == 8
    assert node._pixels.values == [(0, 200, 0)] * 8
    assert node._pixels.show_count == 1
