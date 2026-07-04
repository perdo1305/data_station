#ifndef EEZ_LVGL_UI_GUI_H
#define EEZ_LVGL_UI_GUI_H

#include <lvgl.h>

#include "eez-flow.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const uint8_t assets[7920];

void ui_init();
void ui_tick();
void ui_set_speed(float speed_kph);
void ui_set_screen_var(int screen_id);
void ui_update_telemetry_vars(const void *t);
const char *ui_get_mission_str();
const char *ui_get_mission_name(int mission_id);
const char *ui_get_as_state_name(int as_state_id);

#ifdef __cplusplus
}
#endif

#endif // EEZ_LVGL_UI_GUI_H