#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Screens

enum ScreensEnum {
    _SCREEN_ID_FIRST = 1,
    SCREEN_ID_DRIVER_VIEW = 1,
    SCREEN_ID_START_SCENE = 2,
    SCREEN_ID_AUTONOMOUS = 3,
    SCREEN_ID_DEBUG_AUTONOMOUS = 4,
    SCREEN_ID_DEBUG = 5,
    _SCREEN_ID_LAST = 5
};

typedef struct _objects_t {
    lv_obj_t *driver_view;
    lv_obj_t *start_scene;
    lv_obj_t *autonomous;
    lv_obj_t *debug_autonomous;
    lv_obj_t *debug;
    lv_obj_t *ready_label_1;
    lv_obj_t *ready_label;
    lv_obj_t *middle_container;
    lv_obj_t *temp_motor_container;
    lv_obj_t *tempmotor_label;
    lv_obj_t *lap_times_container;
    lv_obj_t *laptime_label;
    lv_obj_t *lastlap_label;
    lv_obj_t *obj0;
    lv_obj_t *obj1;
    lv_obj_t *temp_inv_container;
    lv_obj_t *temp_inv_label;
    lv_obj_t *speed_container;
    lv_obj_t *speed_label;
    lv_obj_t *km_label;
    lv_obj_t *brake_acell_presure_container;
    lv_obj_t *brake_presure_bar;
    lv_obj_t *accellerator_presure_bar;
    lv_obj_t *hv_bar;
    lv_obj_t *lv_bar;
    lv_obj_t *hv_label;
    lv_obj_t *lv_label;
    lv_obj_t *obj2;
    lv_obj_t *logo;
    lv_obj_t *obj3;
    lv_obj_t *obj4;
    lv_obj_t *obj5;
    lv_obj_t *hv_bar_1;
    lv_obj_t *lv_bar_1;
    lv_obj_t *hv_label_1;
    lv_obj_t *lv_label_1;
    lv_obj_t *brake_presure_container;
    lv_obj_t *brake_presure_bar_1;
    lv_obj_t *brake_presure_bar_2;
    lv_obj_t *infos_container;
    lv_obj_t *obj6;
    lv_obj_t *pneumaticresure_container;
    lv_obj_t *brake_presure_bar_3;
    lv_obj_t *brake_presure_bar_4;
    lv_obj_t *lap_times_container_1;
    lv_obj_t *laptime_label_1;
    lv_obj_t *lastlap_label_1;
    lv_obj_t *obj7;
    lv_obj_t *obj8;
    lv_obj_t *max_speed_container;
    lv_obj_t *km_label_2;
    lv_obj_t *km_label_1;
    lv_obj_t *speed_label_1;
    lv_obj_t *km_label_3;
    lv_obj_t *obj9;
    lv_obj_t *obj10;
    lv_obj_t *bat_cells_container_1;
    lv_obj_t *brake_presure_bar_5;
    lv_obj_t *brake_presure_bar_6;
    lv_obj_t *brake_presure_bar_7;
    lv_obj_t *brake_presure_bar_8;
    lv_obj_t *brake_presure_bar_9;
    lv_obj_t *brake_presure_bar_10;
} objects_t;

extern objects_t objects;

void create_screen_driver_view();
void tick_screen_driver_view();

void create_screen_start_scene();
void tick_screen_start_scene();

void create_screen_autonomous();
void tick_screen_autonomous();

void create_screen_debug_autonomous();
void tick_screen_debug_autonomous();

void create_screen_debug();
void tick_screen_debug();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/