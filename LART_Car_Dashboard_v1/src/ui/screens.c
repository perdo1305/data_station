#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#include <string.h>

objects_t objects;

static const char *screen_names[] = { "Driver View", "Start Scene", "Autonomous", "Debug Autonomous", "Debug" };
static const char *object_names[] = { "driver_view", "start_scene", "autonomous", "debug_autonomous", "debug", "ready_label_1", "ready_label", "middle_container", "temp_motor_container", "tempmotor_label", "lap_times_container", "laptime_label", "lastlap_label", "obj0", "obj1", "temp_inv_container", "temp_inv_label", "speed_container", "speed_label", "km_label", "brake_acell_presure_container", "brake_presure_bar", "accellerator_presure_bar", "obj2", "hv_bar", "lv_bar", "hv_label", "lv_label", "logo", "obj3", "obj4", "obj5", "hv_bar_1", "lv_bar_1", "hv_label_1", "lv_label_1", "brake_presure_container", "brake_presure_bar_1", "brake_presure_bar_2", "infos_container", "obj6", "pneumaticresure_container", "brake_presure_bar_3", "brake_presure_bar_4", "lap_times_container_1", "laptime_label_1", "lastlap_label_1", "obj7", "obj8", "max_speed_container", "km_label_2", "km_label_1", "speed_label_1", "km_label_3", "obj9", "obj10", "bat_cells_container_1", "brake_presure_bar_5", "brake_presure_bar_6", "brake_presure_bar_7", "brake_presure_bar_8", "brake_presure_bar_9", "brake_presure_bar_10" };

//
// Event handlers
//

lv_obj_t *tick_value_change_obj;

//
// Screens
//

void create_screen_driver_view() {
    void *flowState = getFlowState(0, 0);
    (void)flowState;
    lv_obj_t *obj = lv_obj_create(0);
    objects.driver_view = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 800, 480);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // readyLabel_1
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.ready_label_1 = obj;
            lv_obj_set_pos(obj, 0, 200);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_50, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0x02ff02), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "READY");
        }
        {
            // readyLabel
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.ready_label = obj;
            lv_obj_set_pos(obj, 0, 200);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_50, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xfe0000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "");
        }
        {
            // middleContainer
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.middle_container = obj;
            lv_obj_set_pos(obj, 84, 49);
            lv_obj_set_size(obj, 632, 354);
            add_style_info_containers(obj);
            lv_obj_set_style_radius(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // tempMotorContainer
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.temp_motor_container = obj;
                    lv_obj_set_pos(obj, 473, 0);
                    lv_obj_set_size(obj, 155, 88);
                    add_style_info_containers(obj);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // tempmotorLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.tempmotor_label = obj;
                            lv_obj_set_pos(obj, 14, 23);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            add_style_text(obj);
                            lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_30, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "");
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 10, -15);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            add_style_text(obj);
                            lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Temp Motor");
                        }
                    }
                }
                {
                    // lapTimesContainer
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.lap_times_container = obj;
                    lv_obj_set_pos(obj, 123, 235);
                    lv_obj_set_size(obj, 387, 103);
                    add_style_info_containers(obj);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // laptimeLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.laptime_label = obj;
                            lv_obj_set_pos(obj, 6, 9);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            add_style_text(obj);
                            lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_30, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "00:00.000");
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 6, -14);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            add_style_text(obj);
                            lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Lap Times");
                        }
                        {
                            // lastlapLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.lastlap_label = obj;
                            lv_obj_set_pos(obj, 5, 42);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            add_style_text(obj);
                            lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_30, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xfff500), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "00:00.000");
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.obj0 = obj;
                            lv_obj_set_pos(obj, 329, 11);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            add_style_text(obj);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xfefefe), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_50, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "1");
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 247, -14);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            add_style_text(obj);
                            lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Diff");
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.obj1 = obj;
                            lv_obj_set_pos(obj, 247, 14);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            add_style_text(obj);
                            lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xff0000), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "+0.8");
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 317, -15);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            add_style_text(obj);
                            lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Lap");
                        }
                    }
                }
                {
                    // tempInvContainer
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.temp_inv_container = obj;
                    lv_obj_set_pos(obj, 0, 5);
                    lv_obj_set_size(obj, 155, 88);
                    add_style_info_containers(obj);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // tempInvLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.temp_inv_label = obj;
                            lv_obj_set_pos(obj, 14, 23);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            add_style_text(obj);
                            lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_30, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "");
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 20, -15);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            add_style_text(obj);
                            lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Temp INV");
                        }
                    }
                }
                {
                    // speedContainer
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.speed_container = obj;
                    lv_obj_set_pos(obj, 141, 98);
                    lv_obj_set_size(obj, 351, 118);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // speedLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.speed_label = obj;
                            lv_obj_set_pos(obj, -47, 6);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &ui_font_orbiter_bold_100, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "");
                        }
                        {
                            // kmLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.km_label = obj;
                            lv_obj_set_pos(obj, 253, 64);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            add_style_text(obj);
                            lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Km/h");
                        }
                    }
                }
                {
                    // brakeAcellPresureContainer
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.brake_acell_presure_container = obj;
                    lv_obj_set_pos(obj, 530, 235);
                    lv_obj_set_size(obj, 102, 103);
                    add_style_info_containers(obj);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 11, -15);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            add_style_text(obj);
                            lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Pedals");
                        }
                        {
                            // brakePresureBar
                            lv_obj_t *obj = lv_bar_create(parent_obj);
                            objects.brake_presure_bar = obj;
                            lv_obj_set_pos(obj, 18, 8);
                            lv_obj_set_size(obj, 20, 67);
                            lv_bar_set_mode(obj, LV_BAR_MODE_RANGE);
                            lv_bar_set_value(obj, 0, LV_ANIM_OFF);
                            lv_bar_set_start_value(obj, 0, LV_ANIM_OFF);
                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb0000), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_radius(obj, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb0000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                        }
                        {
                            // accelleratorPresureBar
                            lv_obj_t *obj = lv_bar_create(parent_obj);
                            objects.accellerator_presure_bar = obj;
                            lv_obj_set_pos(obj, 62, 8);
                            lv_obj_set_size(obj, 20, 67);
                            lv_bar_set_mode(obj, LV_BAR_MODE_RANGE);
                            lv_bar_set_value(obj, 0, LV_ANIM_OFF);
                            lv_bar_set_start_value(obj, 0, LV_ANIM_OFF);
                            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x02ff02), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_radius(obj, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x02ff02), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                        }
                    }
                }
            }
        }
        {
            lv_obj_t *obj = lv_led_create(parent_obj);
            objects.obj2 = obj;
            lv_obj_set_pos(obj, 120, 8);
            lv_obj_set_size(obj, 32, 32);
            lv_led_set_color(obj, lv_color_hex(0x19ff00));
            lv_led_set_brightness(obj, 255);
        }
        {
            // hvBar
            lv_obj_t *obj = lv_bar_create(parent_obj);
            objects.hv_bar = obj;
            lv_obj_set_pos(obj, 734, 106);
            lv_obj_set_size(obj, 53, 314);
            lv_bar_set_mode(obj, LV_BAR_MODE_RANGE);
            lv_bar_set_value(obj, 0, LV_ANIM_OFF);
            lv_bar_set_start_value(obj, 0, LV_ANIM_OFF);
            add_style_bar(obj);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb0000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_line_width(obj, 10, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 52, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xfb0000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb0000), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // lvBar
            lv_obj_t *obj = lv_bar_create(parent_obj);
            objects.lv_bar = obj;
            lv_obj_set_pos(obj, 11, 108);
            lv_obj_set_size(obj, 53, 314);
            lv_bar_set_range(obj, 18, 33);
            lv_bar_set_mode(obj, LV_BAR_MODE_RANGE);
            lv_bar_set_value(obj, 0, LV_ANIM_ON);
            lv_bar_set_start_value(obj, 0, LV_ANIM_ON);
            add_style_bar(obj);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x02ff02), LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(obj, lv_color_hex(0x02ff02), LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_border_opa(obj, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 52, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x02ff02), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(obj, lv_color_hex(0x02ff02), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // hvLabel
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.hv_label = obj;
            lv_obj_set_pos(obj, 733, 436);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            add_style_text(obj);
            lv_label_set_text(obj, "");
        }
        {
            // lvLabel
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.lv_label = obj;
            lv_obj_set_pos(obj, 9, 436);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            add_style_text(obj);
            lv_label_set_text(obj, "");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 736, 73);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            add_style_text(obj);
            lv_label_set_text_static(obj, "SOC");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 22, 75);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            add_style_text(obj);
            lv_label_set_text_static(obj, "LV");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 11, 17);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            add_style_text(obj);
            lv_label_set_text_static(obj, "STATUS");
        }
    }
    
    tick_screen_driver_view();
}

void tick_screen_driver_view() {
    void *flowState = getFlowState(0, 0);
    (void)flowState;
    {
        const char *new_val = evalTextProperty(flowState, 2, 3, "Failed to evaluate Text in Label widget");
        const char *cur_val = lv_label_get_text(objects.ready_label);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.ready_label;
            lv_label_set_text(objects.ready_label, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = evalTextProperty(flowState, 5, 3, "Failed to evaluate Text in Label widget");
        const char *cur_val = lv_label_get_text(objects.tempmotor_label);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.tempmotor_label;
            lv_label_set_text(objects.tempmotor_label, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = evalTextProperty(flowState, 16, 3, "Failed to evaluate Text in Label widget");
        const char *cur_val = lv_label_get_text(objects.temp_inv_label);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.temp_inv_label;
            lv_label_set_text(objects.temp_inv_label, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = evalTextProperty(flowState, 19, 3, "Failed to evaluate Text in Label widget");
        const char *cur_val = lv_label_get_text(objects.speed_label);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.speed_label;
            lv_label_set_text(objects.speed_label, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = evalIntegerProperty(flowState, 23, 3, "Failed to evaluate Value in Bar widget");
        int32_t cur_val = lv_bar_get_value(objects.brake_presure_bar);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.brake_presure_bar;
            lv_bar_set_value(objects.brake_presure_bar, new_val, LV_ANIM_OFF);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = evalIntegerProperty(flowState, 24, 3, "Failed to evaluate Value in Bar widget");
        int32_t cur_val = lv_bar_get_value(objects.accellerator_presure_bar);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.accellerator_presure_bar;
            lv_bar_set_value(objects.accellerator_presure_bar, new_val, LV_ANIM_OFF);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = evalIntegerProperty(flowState, 26, 3, "Failed to evaluate Value in Bar widget");
        int32_t cur_val = lv_bar_get_value(objects.hv_bar);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.hv_bar;
            lv_bar_set_value(objects.hv_bar, new_val, LV_ANIM_OFF);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = evalIntegerProperty(flowState, 27, 3, "Failed to evaluate Value in Bar widget");
        int32_t cur_val = lv_bar_get_value(objects.lv_bar);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.lv_bar;
            lv_bar_set_value(objects.lv_bar, new_val, LV_ANIM_ON);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = evalTextProperty(flowState, 28, 3, "Failed to evaluate Text in Label widget");
        const char *cur_val = lv_label_get_text(objects.hv_label);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.hv_label;
            lv_label_set_text(objects.hv_label, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = evalTextProperty(flowState, 29, 3, "Failed to evaluate Text in Label widget");
        const char *cur_val = lv_label_get_text(objects.lv_label);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.lv_label;
            lv_label_set_text(objects.lv_label, new_val);
            tick_value_change_obj = NULL;
        }
    }
}

void create_screen_start_scene() {
    void *flowState = getFlowState(0, 1);
    (void)flowState;
    lv_obj_t *obj = lv_obj_create(0);
    objects.start_scene = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 800, 480);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 313, 339);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            add_style_text(obj);
            lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_30, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "LOADING...");
        }
        {
            // logo
            lv_obj_t *obj = lv_image_create(parent_obj);
            objects.logo = obj;
            lv_obj_set_pos(obj, 50, 128);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_image_set_src(obj, &img_lart_logo);
            lv_image_set_scale(obj, 160);
            lv_obj_set_style_width(obj, 700, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_height(obj, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 185, 51);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            add_style_text(obj);
            lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_50, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "DATA STATION");
        }
        {
            lv_obj_t *obj = lv_bar_create(parent_obj);
            objects.obj3 = obj;
            lv_obj_set_pos(obj, 75, 372);
            lv_obj_set_size(obj, 694, 50);
            add_style_bar(obj);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_HOR, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0xffffff), LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_grad_stop(obj, 200, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj4 = obj;
            lv_obj_set_pos(obj, 374, 439);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            add_style_text(obj);
            lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_30, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "");
        }
    }
    
    tick_screen_start_scene();
}

void tick_screen_start_scene() {
    void *flowState = getFlowState(0, 1);
    (void)flowState;
    {
        int32_t new_val = evalIntegerProperty(flowState, 4, 3, "Failed to evaluate Value in Bar widget");
        int32_t cur_val = lv_bar_get_value(objects.obj3);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.obj3;
            lv_bar_set_value(objects.obj3, new_val, LV_ANIM_OFF);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = evalTextProperty(flowState, 5, 3, "Failed to evaluate Text in Label widget");
        const char *cur_val = lv_label_get_text(objects.obj4);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.obj4;
            lv_label_set_text(objects.obj4, new_val);
            tick_value_change_obj = NULL;
        }
    }
}

void create_screen_autonomous() {
    void *flowState = getFlowState(0, 2);
    (void)flowState;
    lv_obj_t *obj = lv_obj_create(0);
    objects.autonomous = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 800, 480);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(obj, lv_color_hex(0xff9400), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 66, 12);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            add_style_text(obj);
            lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_50, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "AUTONOMOUS DRIVING");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj5 = obj;
            lv_obj_set_pos(obj, 125, 417);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            add_style_text(obj);
            lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_40, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0x13ff00), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "MISSION: ACCELERATION");
        }
        {
            // hvBar_1
            lv_obj_t *obj = lv_bar_create(parent_obj);
            objects.hv_bar_1 = obj;
            lv_obj_set_pos(obj, 736, 96);
            lv_obj_set_size(obj, 53, 314);
            lv_bar_set_mode(obj, LV_BAR_MODE_RANGE);
            lv_bar_set_value(obj, 50, LV_ANIM_OFF);
            lv_bar_set_start_value(obj, 0, LV_ANIM_OFF);
            add_style_bar(obj);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb0000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_line_width(obj, 10, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 52, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xfb0000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb0000), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // lvBar_1
            lv_obj_t *obj = lv_bar_create(parent_obj);
            objects.lv_bar_1 = obj;
            lv_obj_set_pos(obj, 13, 98);
            lv_obj_set_size(obj, 53, 314);
            lv_bar_set_mode(obj, LV_BAR_MODE_RANGE);
            lv_bar_set_value(obj, 50, LV_ANIM_ON);
            lv_bar_set_start_value(obj, 0, LV_ANIM_ON);
            add_style_bar(obj);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x02ff02), LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(obj, lv_color_hex(0x02ff02), LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_border_opa(obj, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 52, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x02ff02), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(obj, lv_color_hex(0x02ff02), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // hvLabel_1
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.hv_label_1 = obj;
            lv_obj_set_pos(obj, 738, 424);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            add_style_text(obj);
            lv_label_set_text(obj, "");
        }
        {
            // lvLabel_1
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.lv_label_1 = obj;
            lv_obj_set_pos(obj, 11, 426);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            add_style_text(obj);
            lv_label_set_text(obj, "");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 738, 63);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            add_style_text(obj);
            lv_label_set_text_static(obj, "SOC");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 24, 65);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            add_style_text(obj);
            lv_label_set_text_static(obj, "LV");
        }
        {
            // brakePresureContainer
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.brake_presure_container = obj;
            lv_obj_set_pos(obj, 97, 90);
            lv_obj_set_size(obj, 93, 151);
            add_style_info_containers(obj);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 16, -16);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_text(obj);
                    lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "BRK\nPRESS");
                }
                {
                    // brakePresureBar_1
                    lv_obj_t *obj = lv_bar_create(parent_obj);
                    objects.brake_presure_bar_1 = obj;
                    lv_obj_set_pos(obj, 16, 25);
                    lv_obj_set_size(obj, 20, 81);
                    lv_bar_set_mode(obj, LV_BAR_MODE_RANGE);
                    lv_bar_set_value(obj, 67, LV_ANIM_OFF);
                    lv_bar_set_start_value(obj, 0, LV_ANIM_OFF);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb0000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb0000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                }
                {
                    // brakePresureBar_2
                    lv_obj_t *obj = lv_bar_create(parent_obj);
                    objects.brake_presure_bar_2 = obj;
                    lv_obj_set_pos(obj, 57, 25);
                    lv_obj_set_size(obj, 20, 81);
                    lv_bar_set_mode(obj, LV_BAR_MODE_RANGE);
                    lv_bar_set_value(obj, 67, LV_ANIM_OFF);
                    lv_bar_set_start_value(obj, 0, LV_ANIM_OFF);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb0000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb0000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 17, 108);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_text(obj);
                    lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "10");
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 57, 108);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_text(obj);
                    lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "10");
                }
            }
        }
        {
            // infosContainer
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.infos_container = obj;
            lv_obj_set_pos(obj, 106, 327);
            lv_obj_set_size(obj, 609, 59);
            add_style_info_containers(obj);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.obj6 = obj;
                    lv_obj_set_pos(obj, 17, 6);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_text(obj);
                    lv_obj_set_style_text_font(obj, &ui_font_orbitron_15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xff9400), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "BUG BUG BUG INFO INFO INFOOOOOOOOOOOOOOOO");
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 17, -17);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_text(obj);
                    lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Infos");
                }
            }
        }
        {
            // pneumaticresureContainer
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.pneumaticresure_container = obj;
            lv_obj_set_pos(obj, 219, 90);
            lv_obj_set_size(obj, 93, 151);
            add_style_info_containers(obj);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 16, -16);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_text(obj);
                    lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "PNEUM\nPRESS");
                }
                {
                    // brakePresureBar_3
                    lv_obj_t *obj = lv_bar_create(parent_obj);
                    objects.brake_presure_bar_3 = obj;
                    lv_obj_set_pos(obj, 16, 25);
                    lv_obj_set_size(obj, 20, 81);
                    lv_bar_set_mode(obj, LV_BAR_MODE_RANGE);
                    lv_bar_set_value(obj, 67, LV_ANIM_OFF);
                    lv_bar_set_start_value(obj, 0, LV_ANIM_OFF);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb0000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb0000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                }
                {
                    // brakePresureBar_4
                    lv_obj_t *obj = lv_bar_create(parent_obj);
                    objects.brake_presure_bar_4 = obj;
                    lv_obj_set_pos(obj, 57, 25);
                    lv_obj_set_size(obj, 20, 81);
                    lv_bar_set_mode(obj, LV_BAR_MODE_RANGE);
                    lv_bar_set_value(obj, 67, LV_ANIM_OFF);
                    lv_bar_set_start_value(obj, 0, LV_ANIM_OFF);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb0000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb0000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 17, 108);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_text(obj);
                    lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "10");
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 57, 108);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_text(obj);
                    lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "10");
                }
            }
        }
        {
            // lapTimesContainer_1
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.lap_times_container_1 = obj;
            lv_obj_set_pos(obj, 410, 208);
            lv_obj_set_size(obj, 314, 103);
            add_style_info_containers(obj);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // laptimeLabel_1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.laptime_label_1 = obj;
                    lv_obj_set_pos(obj, 7, 11);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_text(obj);
                    lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_25, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "00:00.000");
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 6, -14);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_text(obj);
                    lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Lap Times");
                }
                {
                    // lastlapLabel_1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.lastlap_label_1 = obj;
                    lv_obj_set_pos(obj, 5, 42);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_text(obj);
                    lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_25, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfff500), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "00:00.000");
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.obj7 = obj;
                    lv_obj_set_pos(obj, 265, 18);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_text(obj);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfefefe), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_50, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "1");
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 197, -14);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_text(obj);
                    lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Diff");
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.obj8 = obj;
                    lv_obj_set_pos(obj, 197, 14);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_text(obj);
                    lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xff0000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "+0.8");
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 253, -14);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_text(obj);
                    lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Lap");
                }
            }
        }
        {
            // maxSpeedContainer
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.max_speed_container = obj;
            lv_obj_set_pos(obj, 404, 75);
            lv_obj_set_size(obj, 320, 121);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // kmLabel_2
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.km_label_2 = obj;
                    lv_obj_set_pos(obj, 36, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_text(obj);
                    lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Max speed");
                }
                {
                    // kmLabel_1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.km_label_1 = obj;
                    lv_obj_set_pos(obj, 245, 82);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_text(obj);
                    lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Km/h");
                }
                {
                    // speedLabel_1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.speed_label_1 = obj;
                    lv_obj_set_pos(obj, -23, 8);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &ui_font_orbiter_bold_100, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "");
                }
            }
        }
        {
            // kmLabel_3
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.km_label_3 = obj;
            lv_obj_set_pos(obj, 106, 268);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            add_style_text(obj);
            lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_30, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xf7ff00), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "STATE: OFF");
        }
    }
    
    tick_screen_autonomous();
}

void tick_screen_autonomous() {
    void *flowState = getFlowState(0, 2);
    (void)flowState;
    {
        const char *new_val = evalTextProperty(flowState, 5, 3, "Failed to evaluate Text in Label widget");
        const char *cur_val = lv_label_get_text(objects.hv_label_1);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.hv_label_1;
            lv_label_set_text(objects.hv_label_1, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = evalTextProperty(flowState, 6, 3, "Failed to evaluate Text in Label widget");
        const char *cur_val = lv_label_get_text(objects.lv_label_1);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.lv_label_1;
            lv_label_set_text(objects.lv_label_1, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = evalTextProperty(flowState, 35, 3, "Failed to evaluate Text in Label widget");
        const char *cur_val = lv_label_get_text(objects.speed_label_1);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.speed_label_1;
            lv_label_set_text(objects.speed_label_1, new_val);
            tick_value_change_obj = NULL;
        }
    }
}

void create_screen_debug_autonomous() {
    void *flowState = getFlowState(0, 3);
    (void)flowState;
    lv_obj_t *obj = lv_obj_create(0);
    objects.debug_autonomous = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 800, 480);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj9 = obj;
            lv_obj_set_pos(obj, 18, 13);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff9400), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "DEBUG");
        }
    }
    
    tick_screen_debug_autonomous();
}

void tick_screen_debug_autonomous() {
    void *flowState = getFlowState(0, 3);
    (void)flowState;
}

void create_screen_debug() {
    void *flowState = getFlowState(0, 4);
    (void)flowState;
    lv_obj_t *obj = lv_obj_create(0);
    objects.debug = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 800, 480);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj10 = obj;
            lv_obj_set_pos(obj, 18, 13);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff9400), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "DEBUG");
        }
        {
            // batCellsContainer_1
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.bat_cells_container_1 = obj;
            lv_obj_set_pos(obj, 35, 71);
            lv_obj_set_size(obj, 225, 151);
            add_style_info_containers(obj);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 23, -12);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_text(obj);
                    lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "BATTERY MODULES");
                }
                {
                    // brakePresureBar_5
                    lv_obj_t *obj = lv_bar_create(parent_obj);
                    objects.brake_presure_bar_5 = obj;
                    lv_obj_set_pos(obj, 16, 26);
                    lv_obj_set_size(obj, 20, 81);
                    lv_bar_set_mode(obj, LV_BAR_MODE_RANGE);
                    lv_bar_set_value(obj, 67, LV_ANIM_OFF);
                    lv_bar_set_start_value(obj, 0, LV_ANIM_OFF);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb0000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x19ff00), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                }
                {
                    // brakePresureBar_6
                    lv_obj_t *obj = lv_bar_create(parent_obj);
                    objects.brake_presure_bar_6 = obj;
                    lv_obj_set_pos(obj, 50, 26);
                    lv_obj_set_size(obj, 20, 81);
                    lv_bar_set_mode(obj, LV_BAR_MODE_RANGE);
                    lv_bar_set_value(obj, 67, LV_ANIM_OFF);
                    lv_bar_set_start_value(obj, 0, LV_ANIM_OFF);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb0000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x19ff00), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 17, 108);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_text(obj);
                    lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "10");
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 50, 108);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_text(obj);
                    lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "10");
                }
                {
                    // brakePresureBar_7
                    lv_obj_t *obj = lv_bar_create(parent_obj);
                    objects.brake_presure_bar_7 = obj;
                    lv_obj_set_pos(obj, 83, 26);
                    lv_obj_set_size(obj, 20, 81);
                    lv_bar_set_mode(obj, LV_BAR_MODE_RANGE);
                    lv_bar_set_value(obj, 67, LV_ANIM_OFF);
                    lv_bar_set_start_value(obj, 0, LV_ANIM_OFF);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb0000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x19ff00), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                }
                {
                    // brakePresureBar_8
                    lv_obj_t *obj = lv_bar_create(parent_obj);
                    objects.brake_presure_bar_8 = obj;
                    lv_obj_set_pos(obj, 117, 26);
                    lv_obj_set_size(obj, 20, 81);
                    lv_bar_set_mode(obj, LV_BAR_MODE_RANGE);
                    lv_bar_set_value(obj, 67, LV_ANIM_OFF);
                    lv_bar_set_start_value(obj, 0, LV_ANIM_OFF);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb0000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x19ff00), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                }
                {
                    // brakePresureBar_9
                    lv_obj_t *obj = lv_bar_create(parent_obj);
                    objects.brake_presure_bar_9 = obj;
                    lv_obj_set_pos(obj, 150, 26);
                    lv_obj_set_size(obj, 20, 81);
                    lv_bar_set_mode(obj, LV_BAR_MODE_RANGE);
                    lv_bar_set_value(obj, 67, LV_ANIM_OFF);
                    lv_bar_set_start_value(obj, 0, LV_ANIM_OFF);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb0000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x19ff00), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                }
                {
                    // brakePresureBar_10
                    lv_obj_t *obj = lv_bar_create(parent_obj);
                    objects.brake_presure_bar_10 = obj;
                    lv_obj_set_pos(obj, 184, 26);
                    lv_obj_set_size(obj, 20, 81);
                    lv_bar_set_mode(obj, LV_BAR_MODE_RANGE);
                    lv_bar_set_value(obj, 67, LV_ANIM_OFF);
                    lv_bar_set_start_value(obj, 0, LV_ANIM_OFF);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xfb0000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x19ff00), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 84, 108);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_text(obj);
                    lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "10");
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 117, 108);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_text(obj);
                    lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "10");
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 151, 108);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_text(obj);
                    lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "10");
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 184, 108);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_text(obj);
                    lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "10");
                }
            }
        }
    }
    
    tick_screen_debug();
}

void tick_screen_debug() {
    void *flowState = getFlowState(0, 4);
    (void)flowState;
}

typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_driver_view,
    tick_screen_start_scene,
    tick_screen_autonomous,
    tick_screen_debug_autonomous,
    tick_screen_debug,
};
void tick_screen(int screen_index) {
    if (screen_index >= 0 && screen_index < 5) {
        tick_screen_funcs[screen_index]();
    }
}
void tick_screen_by_id(enum ScreensEnum screenId) {
    tick_screen(screenId - 1);
}

//
// Styles
//

static const char *style_names[] = { "info_containers", "text", "bar" };

extern void add_style(lv_obj_t *obj, int32_t styleIndex);
extern void remove_style(lv_obj_t *obj, int32_t styleIndex);

//
// Fonts
//

ext_font_desc_t fonts[] = {
    { "orbitron_bold_20", &ui_font_orbitron_bold_20 },
    { "ORBITER_BOLD_100", &ui_font_orbiter_bold_100 },
    { "orbitron_bold_30", &ui_font_orbitron_bold_30 },
    { "orbitron_bold_50", &ui_font_orbitron_bold_50 },
    { "orbitron_bold_15", &ui_font_orbitron_bold_15 },
    { "orbitron_15", &ui_font_orbitron_15 },
    { "orbitron_bold_25", &ui_font_orbitron_bold_25 },
    { "orbitron_bold_40", &ui_font_orbitron_bold_40 },
#if LV_FONT_MONTSERRAT_8
    { "MONTSERRAT_8", &lv_font_montserrat_8 },
#endif
#if LV_FONT_MONTSERRAT_10
    { "MONTSERRAT_10", &lv_font_montserrat_10 },
#endif
#if LV_FONT_MONTSERRAT_12
    { "MONTSERRAT_12", &lv_font_montserrat_12 },
#endif
#if LV_FONT_MONTSERRAT_14
    { "MONTSERRAT_14", &lv_font_montserrat_14 },
#endif
#if LV_FONT_MONTSERRAT_16
    { "MONTSERRAT_16", &lv_font_montserrat_16 },
#endif
#if LV_FONT_MONTSERRAT_18
    { "MONTSERRAT_18", &lv_font_montserrat_18 },
#endif
#if LV_FONT_MONTSERRAT_20
    { "MONTSERRAT_20", &lv_font_montserrat_20 },
#endif
#if LV_FONT_MONTSERRAT_22
    { "MONTSERRAT_22", &lv_font_montserrat_22 },
#endif
#if LV_FONT_MONTSERRAT_24
    { "MONTSERRAT_24", &lv_font_montserrat_24 },
#endif
#if LV_FONT_MONTSERRAT_26
    { "MONTSERRAT_26", &lv_font_montserrat_26 },
#endif
#if LV_FONT_MONTSERRAT_28
    { "MONTSERRAT_28", &lv_font_montserrat_28 },
#endif
#if LV_FONT_MONTSERRAT_30
    { "MONTSERRAT_30", &lv_font_montserrat_30 },
#endif
#if LV_FONT_MONTSERRAT_32
    { "MONTSERRAT_32", &lv_font_montserrat_32 },
#endif
#if LV_FONT_MONTSERRAT_34
    { "MONTSERRAT_34", &lv_font_montserrat_34 },
#endif
#if LV_FONT_MONTSERRAT_36
    { "MONTSERRAT_36", &lv_font_montserrat_36 },
#endif
#if LV_FONT_MONTSERRAT_38
    { "MONTSERRAT_38", &lv_font_montserrat_38 },
#endif
#if LV_FONT_MONTSERRAT_40
    { "MONTSERRAT_40", &lv_font_montserrat_40 },
#endif
#if LV_FONT_MONTSERRAT_42
    { "MONTSERRAT_42", &lv_font_montserrat_42 },
#endif
#if LV_FONT_MONTSERRAT_44
    { "MONTSERRAT_44", &lv_font_montserrat_44 },
#endif
#if LV_FONT_MONTSERRAT_46
    { "MONTSERRAT_46", &lv_font_montserrat_46 },
#endif
#if LV_FONT_MONTSERRAT_48
    { "MONTSERRAT_48", &lv_font_montserrat_48 },
#endif
};

//
//
//

void create_screens() {
    // Initialize styles
    eez_flow_init_styles(add_style, remove_style);
    eez_flow_init_style_names(style_names, sizeof(style_names) / sizeof(const char *));

eez_flow_init_fonts(fonts, sizeof(fonts) / sizeof(ext_font_desc_t));

// Set default LVGL theme
    lv_display_t *dispp = lv_display_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
    lv_display_set_theme(dispp, theme);
    
    // Initialize screens
    eez_flow_init_screen_names(screen_names, sizeof(screen_names) / sizeof(const char *));
    eez_flow_init_object_names(object_names, sizeof(object_names) / sizeof(const char *));
    
    // Create screens
    create_screen_driver_view();
    create_screen_start_scene();
    create_screen_autonomous();
    create_screen_debug_autonomous();
    create_screen_debug();
}