"""Decode fixed CAN payloads with the committed C implementation, without ROS."""
import ctypes
from pathlib import Path
import subprocess

import pytest


class Aqt7(ctypes.Structure):
    _fields_ = [("susp_l", ctypes.c_int16), ("susp_r", ctypes.c_int16),
                ("ntc_1", ctypes.c_uint8)]


class MasterMscId3(ctypes.Structure):
    _fields_ = [(name, ctypes.c_uint16) for name in (
        "overall_maximum_voltage", "overall_maximum_temperature",
        "overall_minimum_voltage", "overall_minimum_temperature",
    )]


class Aqt4(ctypes.Structure):
    _fields_ = [("st_angle", ctypes.c_int16), ("susp_l", ctypes.c_int16),
                ("susp_r", ctypes.c_int16), ("inertia", ctypes.c_uint8),
                ("emergency", ctypes.c_uint8)]


class VcuStates(ctypes.Structure):
    _fields_ = [(name, ctypes.c_uint8) for name in (
        "vcu_state", "throttle_status", "tc_state", "tv_state",
        "front_wheel_speed_ok", "steering_angle_ok", "front_left_speed",
        "front_right_speed", "tc_factor_rear_left", "tc_factor_rear_right",
    )] + [("tv_shift", ctypes.c_int8), ("road_wheel_angle", ctypes.c_int8),
         ("vcu_states_counter", ctypes.c_uint8)]


@pytest.fixture(scope="module")
def decoders(tmp_path_factory):
    generated = (Path(__file__).resolve().parents[1]
                 / "LART_Car_Dashboard_v1/src/ui/generated")
    library = tmp_path_factory.mktemp("dbc-decoding") / "decoders.so"
    subprocess.run([
        "cc", "-std=c99", "-shared", "-fPIC", "-Wall", "-Wextra", "-Werror",
        str(generated / "data_t26.c"), str(generated / "autonomous_t26.c"),
        str(generated / "powertrain_t26.c"),
        "-o", str(library),
    ], check=True)
    return ctypes.CDLL(str(library))


def unpack(decoders, name, message_type, payload):
    function = getattr(decoders, f"{name}_unpack")
    function.argtypes = [ctypes.POINTER(message_type),
                         ctypes.POINTER(ctypes.c_uint8), ctypes.c_size_t]
    function.restype = ctypes.c_int
    raw = (ctypes.c_uint8 * len(payload)).from_buffer_copy(payload)
    message = message_type()
    result = function(ctypes.byref(message), raw, len(payload))
    return result, message


def physical(decoders, name, message):
    values = []
    for field, raw_type in message._fields_:
        function = getattr(decoders, f"{name}_{field}_decode")
        function.argtypes = [raw_type]
        function.restype = ctypes.c_double
        values.append(function(getattr(message, field)))
    return values


@pytest.mark.parametrize(("payload", "expected"), [
    # Little endian raw: -1234, +2345, 77; trailing padding is unrelated.
    ("2e fb 29 09 4d a5 5a ff", [-123.4, 234.5, 77.0]),
    # Both signed suspension limits and an unsigned temperature above 127.
    ("ff 7f 00 80 ff 00 00 00", [3276.7, -3276.8, 255.0]),
])
def test_aqt7_decodes_suspension_and_separate_temperature(decoders, payload, expected):
    result, message = unpack(decoders, "data_t26_aqt7", Aqt7,
                             bytes.fromhex(payload))
    assert result == 0
    assert physical(decoders, "data_t26_aqt7", message) == pytest.approx(expected)


def test_aqt7_rejects_previous_four_byte_payload(decoders):
    result, _ = unpack(decoders, "data_t26_aqt7", Aqt7,
                       bytes.fromhex("2e fb 29 09"))
    assert result == -22  # EINVAL: the current wire frame requires eight bytes.


def test_data_master_msc_id_3_decodes_physical_voltage_and_temperature(decoders):
    # Raw 4200, 4512, 3150, 2735 => volts x0.001, temperatures x0.01.
    result, message = unpack(decoders, "data_t26_master_msc_id_3", MasterMscId3,
                             bytes.fromhex("68 10 a0 11 4e 0c af 0a"))
    assert result == 0
    assert physical(decoders, "data_t26_master_msc_id_3", message) == pytest.approx(
        [4.2, 45.12, 3.15, 27.35])


def test_autonomous_aqt4_decodes_signed_suspension(decoders):
    # Raw steering +123, suspension -1000 / -1, inertia on, emergency off.
    result, message = unpack(decoders, "autonomous_t26_aqt4", Aqt4,
                             bytes.fromhex("7b 00 18 fc ff ff 01"))
    assert result == 0
    assert physical(decoders, "autonomous_t26_aqt4", message) == pytest.approx(
        [12.3, -100.0, -0.1, 1.0, 0.0])


@pytest.mark.parametrize(("payload", "expected"), [
    # Packed status flags, byte-crossing speeds/factors, signed shift and angle.
    ("96 fa bd 64 15 f9 bd 9a",
     [6, 1, 5, 6, 1, 1, 61.5, 100.5, 42, 100, -17, -21.25, 9]),
    ("ff df 7f 00 00 19 08 f8",
     [15, 7, 7, 7, 0, 1, 127.5, 0, 0, 100, -64, -32, 15]),
    ("00 20 80 7f 32 e0 f7 07",
     [0, 0, 0, 0, 1, 0, 0, 127.5, 100, 0, 63, 31.75, 0]),
])
def test_powertrain_vcu_states_decodes_packed_debug_signals(decoders, payload, expected):
    result, message = unpack(decoders, "powertrain_t26_vcu_states", VcuStates,
                             bytes.fromhex(payload))
    assert result == 0
    assert physical(decoders, "powertrain_t26_vcu_states", message) == pytest.approx(expected)


def test_powertrain_vcu_states_rejects_truncated_frame(decoders):
    result, _ = unpack(decoders, "powertrain_t26_vcu_states", VcuStates,
                       bytes.fromhex("96 fa bd 64 15 f9 bd"))
    assert result == -22
