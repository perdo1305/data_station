import re
import unittest
from pathlib import Path


UI_DIR = Path(__file__).resolve().parent


class DriverBatteryLayoutTest(unittest.TestCase):
    def setUp(self):
        self.screens = (UI_DIR / "screens.c").read_text()
        self.screen_header = (UI_DIR / "screens.h").read_text()
        self.ui_header = (UI_DIR / "ui.h").read_text()
        self.dbc_api = (UI_DIR / "dbc_api.cpp").read_text()

    def _driver_view(self):
        return self.screens.split("void create_screen_driver_view()", 1)[1].split(
            "void tick_screen_driver_view()", 1
        )[0]

    def _position(self, object_name):
        block = self._driver_view().split(f"objects.{object_name} = obj;", 1)[1]
        match = re.search(r"lv_obj_set_pos\(obj,\s*(-?\d+),\s*(-?\d+)\);", block)
        self.assertIsNotNone(match, object_name)
        return tuple(map(int, match.groups()))

    def test_current_labels_exist_on_both_sides(self):
        self.assertIn("lv_obj_t *lv_current_label;", self.screen_header)
        self.assertIn("lv_obj_t *hv_current_label;", self.screen_header)
        self.assertIn("objects.lv_current_label = obj;", self._driver_view())
        self.assertIn("objects.hv_current_label = obj;", self._driver_view())

    def test_battery_stack_is_clear_of_status_and_screen_bottom(self):
        _, lv_bar_y = self._position("lv_bar")
        _, hv_bar_y = self._position("hv_bar")
        _, lv_voltage_y = self._position("lv_label")
        _, hv_voltage_y = self._position("hv_label")
        _, lv_current_y = self._position("lv_current_label")
        _, hv_current_y = self._position("hv_current_label")

        self.assertGreaterEqual(lv_bar_y, 70)
        self.assertGreaterEqual(hv_bar_y, 70)
        self.assertLess(lv_bar_y, 108)
        self.assertLess(hv_bar_y, 106)
        self.assertGreater(lv_voltage_y, lv_bar_y + 314)
        self.assertGreater(hv_voltage_y, hv_bar_y + 314)
        self.assertGreater(lv_current_y, lv_voltage_y)
        self.assertGreater(hv_current_y, hv_voltage_y)
        self.assertLessEqual(lv_current_y, 458)
        self.assertLessEqual(hv_current_y, 458)

    def test_lv_current_is_formatted_from_icd_current(self):
        self.assertIn("const char *ui_get_lv_current_str();", self.ui_header)
        formatter = self.dbc_api.split("ui_get_lv_current_str()", 1)[1].split(
            "extern \"C\"", 1
        )[0]
        self.assertIn("dbc_api.icd_result.icd_current", formatter)
        self.assertIn('"%.1f A"', formatter)
        self.assertIn("ui_get_lv_current_str()", self.screens)

    def test_hv_current_is_a_placeholder(self):
        hv_block = self._driver_view().split("objects.hv_current_label = obj;", 1)[1]
        self.assertIn('lv_label_set_text_static(obj, "--.- A");', hv_block)


if __name__ == "__main__":
    unittest.main()
