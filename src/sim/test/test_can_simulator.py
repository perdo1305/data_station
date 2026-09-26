import unittest

from sim.can_simulator import _PrechargeSequence, _precharge_sequence_value


class PrechargeSequenceTest(unittest.TestCase):
    def test_advances_from_rx_can_to_hv_on_every_half_second(self):
        expected_sequence = [19.0] + [float(state) for state in range(17)]
        actual_sequence = [
            _precharge_sequence_value(step * 0.5)
            for step in range(len(expected_sequence))
        ]

        self.assertEqual(actual_sequence, expected_sequence)
        self.assertEqual(_precharge_sequence_value(0.499), 19.0)
        self.assertEqual(_precharge_sequence_value(20.0), 16.0)

    def test_parameter_19_triggers_once_and_can_be_rearmed(self):
        sequence = _PrechargeSequence()

        self.assertEqual(sequence.value(-1.0, now=5.0), -1.0)
        self.assertEqual(sequence.value(19.0, now=10.0), 19.0)
        self.assertEqual(sequence.value(19.0, now=10.5), 0.0)
        self.assertEqual(sequence.value(19.0, now=30.0), 16.0)

        self.assertEqual(sequence.value(-1.0, now=31.0), -1.0)
        self.assertEqual(sequence.value(19.0, now=32.0), 19.0)


if __name__ == '__main__':
    unittest.main()
