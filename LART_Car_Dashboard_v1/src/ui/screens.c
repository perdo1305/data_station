#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "ros2subscriber.h"
#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

// Custom recolor text parser for lv_spangroup
void my_spangroup_set_recolor_text(lv_obj_t *spangroup, const char *text) {
    if (!spangroup) return;
    
    // Clear all existing spans
    while (lv_spangroup_get_span_count(spangroup) > 0) {
        lv_span_t *span = lv_spangroup_get_child(spangroup, 0);
        if (span) {
            lv_spangroup_delete_span(spangroup, span);
        }
    }
    
    uint32_t default_color = 0xff9400; // default amber
    uint32_t active_color = default_color;
    
    const char *ptr = text;
    const char *chunk_start = text;
    
    while (*ptr != '\0') {
        // Check if starting a color tag: #RRGGBB
        if (ptr[0] == '#' &&
            isxdigit((unsigned char)ptr[1]) && isxdigit((unsigned char)ptr[2]) &&
            isxdigit((unsigned char)ptr[3]) && isxdigit((unsigned char)ptr[4]) &&
            isxdigit((unsigned char)ptr[5]) && isxdigit((unsigned char)ptr[6])) {
            
            // First emit previous chunk if any
            int len = ptr - chunk_start;
            if (len > 0) {
                lv_span_t *span = lv_spangroup_new_span(spangroup);
                char *temp = malloc(len + 1);
                memcpy(temp, chunk_start, len);
                temp[len] = '\0';
                lv_span_set_text(span, temp);
                free(temp);
                lv_style_set_text_color(&span->style, lv_color_hex(active_color));
                lv_style_set_text_font(&span->style, &ui_font_orbitron_bold_15);
            }
            
            // Parse new color
            char hex[7];
            memcpy(hex, ptr + 1, 6);
            hex[6] = '\0';
            active_color = strtol(hex, NULL, 16);
            
            ptr += 7;
            chunk_start = ptr;
        }
        // Check if it's a closing tag: #
        else if (ptr[0] == '#' && ptr[1] != '\0') {
            // First emit previous chunk if any
            int len = ptr - chunk_start;
            if (len > 0) {
                lv_span_t *span = lv_spangroup_new_span(spangroup);
                char *temp = malloc(len + 1);
                memcpy(temp, chunk_start, len);
                temp[len] = '\0';
                lv_span_set_text(span, temp);
                free(temp);
                lv_style_set_text_color(&span->style, lv_color_hex(active_color));
                lv_style_set_text_font(&span->style, &ui_font_orbitron_bold_15);
            }
            
            active_color = default_color;
            ptr += 1;
            chunk_start = ptr;
        }
        else {
            ptr++;
        }
    }
    
    // Emit final chunk if any
    int len = ptr - chunk_start;
    if (len > 0) {
        lv_span_t *span = lv_spangroup_new_span(spangroup);
        char *temp = malloc(len + 1);
        memcpy(temp, chunk_start, len);
        temp[len] = '\0';
        lv_span_set_text(span, temp);
        free(temp);
        lv_style_set_text_color(&span->style, lv_color_hex(active_color));
        lv_style_set_text_font(&span->style, &ui_font_orbitron_bold_15);
    }
    
    lv_spangroup_refr_mode(spangroup);
}

objects_t objects;

static const char *screen_names[] = { "Driver View", "Autonomous", "Debug", "Debug Autonomous", "Debug Autonomous_1" };
static const char *object_names[] = { "driver_view", "autonomous", "debug", "debug_autonomous", "debug_autonomous_1", "ready_label", "middle_container", "temp_motor_container", "tempmotor_label", "lap_times_container", "laptime_label", "lastlap_label", "obj0", "obj1", "temp_inv_container", "temp_inv_label", "speed_container", "speed_label", "km_label", "brake_acell_presure_container", "brake_presure_bar", "accellerator_presure_bar", "obj2", "hv_bar", "lv_bar", "hv_label", "lv_label", "obj3", "hv_bar_1", "lv_bar_1", "hv_label_1", "lv_label_1", "brake_presure_container", "brake_presure_bar_1", "brake_presure_bar_2", "pneumaticresure_container", "brake_presure_bar_3", "brake_presure_bar_4", "lap_times_container_1", "laptime_label_1", "lastlap_label_1", "obj4", "obj5", "max_speed_container", "km_label_2", "km_label_1", "speed_label_1", "km_label_3", "obj6", "debug_text_1", "debug_text", "debug_text_2" };

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
            // readyLabel
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.ready_label = obj;
            lv_obj_set_pos(obj, 0, 200);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            add_style_text(obj);
            lv_obj_set_style_text_color(obj, lv_color_hex(0x3fff00), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_50, LV_PART_MAIN | LV_STATE_DEFAULT);
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
                            lv_obj_set_pos(obj, 10, 22);
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
                            lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_25, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "00:00.000");
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 6, -14);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            add_style_text(obj);
                            lv_label_set_text_static(obj, "Lap Times");
                        }
                        {
                            // lastlapLabel
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.lastlap_label = obj;
                            lv_obj_set_pos(obj, 5, 42);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            add_style_text(obj);
                            lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_25, LV_PART_MAIN | LV_STATE_DEFAULT);
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
                            lv_label_set_text(obj, "");
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 235, -14);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            add_style_text(obj);
                            lv_label_set_text_static(obj, "Diff");
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.obj1 = obj;
                            lv_obj_set_pos(obj, 220, 21);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            add_style_text(obj);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xff0000), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_30, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "");
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 317, -15);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            add_style_text(obj);
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
                            lv_obj_set_pos(obj, 11, 20);
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
                            lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_30, LV_PART_MAIN | LV_STATE_DEFAULT);
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
                            lv_label_set_text_static(obj, "Pedals");
                        }
                        {
                            // brakePresureBar
                            lv_obj_t *obj = lv_bar_create(parent_obj);
                            objects.brake_presure_bar = obj;
                            lv_obj_set_pos(obj, 18, 8);
                            lv_obj_set_size(obj, 20, 67);
                            lv_bar_set_mode(obj, LV_BAR_MODE_RANGE);
                            lv_bar_set_value(obj, 0, LV_ANIM_ON);
                            lv_bar_set_start_value(obj, 0, LV_ANIM_ON);
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
                            lv_bar_set_value(obj, 0, LV_ANIM_ON);
                            lv_bar_set_start_value(obj, 0, LV_ANIM_ON);
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
            lv_led_set_color(obj, lv_color_hex(0x29ff00));
            lv_led_set_brightness(obj, 255);
        }
        {
            // hvBar
            lv_obj_t *obj = lv_bar_create(parent_obj);
            objects.hv_bar = obj;
            lv_obj_set_pos(obj, 734, 106);
            lv_obj_set_size(obj, 53, 314);
            lv_bar_set_mode(obj, LV_BAR_MODE_RANGE);
            lv_bar_set_value(obj, 0, LV_ANIM_ON);
            lv_bar_set_start_value(obj, 0, LV_ANIM_ON);
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
            lv_bar_set_range(obj, 18, 30);
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
        const char *new_val = evalTextProperty(flowState, 0, 3, "Failed to evaluate Text in Label widget");
        const char *cur_val = lv_label_get_text(objects.ready_label);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.ready_label;
            lv_label_set_text(objects.ready_label, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = evalTextProperty(flowState, 4, 3, "Failed to evaluate Text in Label widget");
        const char *cur_val = lv_label_get_text(objects.tempmotor_label);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.tempmotor_label;
            lv_label_set_text(objects.tempmotor_label, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = evalTextProperty(flowState, 10, 3, "Failed to evaluate Text in Label widget");
        const char *cur_val = lv_label_get_text(objects.obj0);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.obj0;
            lv_label_set_text(objects.obj0, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = evalTextProperty(flowState, 12, 3, "Failed to evaluate Text in Label widget");
        const char *cur_val = lv_label_get_text(objects.obj1);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.obj1;
            lv_label_set_text(objects.obj1, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = evalTextProperty(flowState, 15, 3, "Failed to evaluate Text in Label widget");
        const char *cur_val = lv_label_get_text(objects.temp_inv_label);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.temp_inv_label;
            lv_label_set_text(objects.temp_inv_label, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = evalTextProperty(flowState, 18, 3, "Failed to evaluate Text in Label widget");
        const char *cur_val = lv_label_get_text(objects.speed_label);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.speed_label;
            lv_label_set_text(objects.speed_label, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = evalIntegerProperty(flowState, 22, 3, "Failed to evaluate Value in Bar widget");
        int32_t cur_val = lv_bar_get_value(objects.brake_presure_bar);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.brake_presure_bar;
            lv_bar_set_value(objects.brake_presure_bar, new_val, LV_ANIM_ON);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = evalIntegerProperty(flowState, 23, 3, "Failed to evaluate Value in Bar widget");
        int32_t cur_val = lv_bar_get_value(objects.accellerator_presure_bar);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.accellerator_presure_bar;
            lv_bar_set_value(objects.accellerator_presure_bar, new_val, LV_ANIM_ON);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = evalIntegerProperty(flowState, 25, 3, "Failed to evaluate Value in Bar widget");
        int32_t cur_val = lv_bar_get_value(objects.hv_bar);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.hv_bar;
            lv_bar_set_value(objects.hv_bar, new_val, LV_ANIM_ON);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = evalIntegerProperty(flowState, 26, 3, "Failed to evaluate Value in Bar widget");
        int32_t cur_val = lv_bar_get_value(objects.lv_bar);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.lv_bar;
            lv_bar_set_value(objects.lv_bar, new_val, LV_ANIM_ON);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = evalTextProperty(flowState, 27, 3, "Failed to evaluate Text in Label widget");
        const char *cur_val = lv_label_get_text(objects.hv_label);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.hv_label;
            lv_label_set_text(objects.hv_label, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = evalTextProperty(flowState, 28, 3, "Failed to evaluate Text in Label widget");
        const char *cur_val = lv_label_get_text(objects.lv_label);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.lv_label;
            lv_label_set_text(objects.lv_label, new_val);
            tick_value_change_obj = NULL;
        }
    }
}

void create_screen_autonomous() {
    void *flowState = getFlowState(0, 1);
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
            lv_obj_set_pos(obj, 70, 10);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            add_style_text(obj);
            lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_50, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "AUTONOMOUS DRIVING");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj3 = obj;
            lv_obj_set_pos(obj, 106, 407);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            add_style_text(obj);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0x13ff00), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_40, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "");
        }
        {
            // hvBar_1
            lv_obj_t *obj = lv_bar_create(parent_obj);
            objects.hv_bar_1 = obj;
            lv_obj_set_pos(obj, 736, 96);
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
            // lvBar_1
            lv_obj_t *obj = lv_bar_create(parent_obj);
            objects.lv_bar_1 = obj;
            lv_obj_set_pos(obj, 13, 98);
            lv_obj_set_size(obj, 53, 314);
            lv_bar_set_range(obj, 18, 30);
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
            lv_obj_set_size(obj, 93, 154);
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
                    lv_obj_set_pos(obj, 4, -16);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_text(obj);
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
                    lv_obj_set_pos(obj, 14, 106);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_text(obj);
                    lv_label_set_text_static(obj, "10");
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 54, 106);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_text(obj);
                    lv_label_set_text_static(obj, "10");
                }
            }
        }
        {
            // pneumaticresureContainer
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.pneumaticresure_container = obj;
            lv_obj_set_pos(obj, 219, 90);
            lv_obj_set_size(obj, 93, 154);
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
                    lv_obj_set_pos(obj, 3, -16);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_text(obj);
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
                    lv_obj_set_pos(obj, 14, 106);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_text(obj);
                    lv_label_set_text_static(obj, "10");
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 54, 106);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_text(obj);
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
                    lv_label_set_text_static(obj, "Lap Times");
                }
                {
                    // lastlapLabel_1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.lastlap_label_1 = obj;
                    lv_obj_set_pos(obj, 5, 42);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_text(obj);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfff500), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_25, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "00:00.000");
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.obj4 = obj;
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
                    lv_label_set_text_static(obj, "Diff");
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.obj5 = obj;
                    lv_obj_set_pos(obj, 197, 14);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_text(obj);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xff0000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "+0.8");
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 253, -14);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_text(obj);
                    lv_label_set_text_static(obj, "Lap");
                }
            }
        }
        {
            // maxSpeedContainer
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.max_speed_container = obj;
            lv_obj_set_pos(obj, 379, 65);
            lv_obj_set_size(obj, 345, 143);
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
                    lv_obj_set_pos(obj, 22, 6);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_text(obj);
                    lv_label_set_text_static(obj, "Max speed");
                }
                {
                    // kmLabel_1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.km_label_1 = obj;
                    lv_obj_set_pos(obj, 230, 72);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_text(obj);
                    lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_30, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Km/h");
                }
                {
                    // speedLabel_1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.speed_label_1 = obj;
                    lv_obj_set_pos(obj, -55, 15);
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
            lv_obj_set_style_text_color(obj, lv_color_hex(0xf7ff00), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_30, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "");
        }
    }
    
    tick_screen_autonomous();
}

void tick_screen_autonomous() {
    void *flowState = getFlowState(0, 1);
    (void)flowState;
    {
        const char *new_val = evalTextProperty(flowState, 2, 3, "Failed to evaluate Text in Label widget");
        const char *cur_val = lv_label_get_text(objects.obj3);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.obj3;
            lv_label_set_text(objects.obj3, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = evalIntegerProperty(flowState, 3, 3, "Failed to evaluate Value in Bar widget");
        int32_t cur_val = lv_bar_get_value(objects.hv_bar_1);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.hv_bar_1;
            lv_bar_set_value(objects.hv_bar_1, new_val, LV_ANIM_OFF);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = evalIntegerProperty(flowState, 4, 3, "Failed to evaluate Value in Bar widget");
        int32_t cur_val = lv_bar_get_value(objects.lv_bar_1);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.lv_bar_1;
            lv_bar_set_value(objects.lv_bar_1, new_val, LV_ANIM_ON);
            tick_value_change_obj = NULL;
        }
    }
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
        const char *new_val = evalTextProperty(flowState, 32, 3, "Failed to evaluate Text in Label widget");
        const char *cur_val = lv_label_get_text(objects.speed_label_1);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.speed_label_1;
            lv_label_set_text(objects.speed_label_1, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = evalTextProperty(flowState, 33, 3, "Failed to evaluate Text in Label widget");
        const char *cur_val = lv_label_get_text(objects.km_label_3);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.km_label_3;
            lv_label_set_text(objects.km_label_3, new_val);
            tick_value_change_obj = NULL;
        }
    }
}

void create_screen_debug() {
    void *flowState = getFlowState(0, 2);
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
            objects.obj6 = obj;
            lv_obj_set_pos(obj, 19, 10);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff9400), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &ui_font_orbitron_bold_25, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "DEBUG");
        }
        {
            // debugText_1
            lv_obj_t *obj = lv_spangroup_create(parent_obj);
            objects.debug_text_1 = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 800, 480);
            lv_spangroup_set_mode(obj, LV_SPAN_MODE_FIXED);
            lv_spangroup_set_align(obj, LV_TEXT_ALIGN_LEFT);
            my_spangroup_set_recolor_text(obj, " GENERAL DEBUG                                                          \n------------------------------------------+---------------------------------------------\n AMS                                     | INVERTER (HV500)\n State:    --------   SOC: ---.--%       | ERPM:     ------    Duty: ---.--%\n Runtime:  -----s     Fans: ---%         | Vin:      ---- V    Fault: ---\n MCU Vref: -.--V      MCU T: ---°C      | AC Curr:  ---.- Apk DC: ---.- A\n PEC Err:  ---        Flt Cnt: ---       | Ctrl T:   ---.-°C   Mot T: ---.-°C\n Slaves:   --/12      FW: --------       | Throttle: ---%      Brake: ---%\n Fault1:   ----------------              | FOC Id:   ---.- Apk Iq: ---.- Apk\n Fault2:   ----------------              | DriveEn:  ---        CtrlMode: ---\n------------------------------------------+---------------------------------------------\n PRECHARGE                               | IVT (ISABELLE)\n State:    ----------------              | Current:  -------- mA\n AIR+: --- AIR-: --- CHG: --- DIS: ---   | U1: -------- mV  U2: -------- mV\n                                         | U3: -------- mV  Temp: -----.-°C\n PACK OVERVIEW                           | Power: -------- W  Energy: -------- Wh\n Vmax: -.---V   Vmin: -.---V             |\n Tmax: ---.--°C Tmin: ---.--°C           | PEDALS / DASHBOARD\n                                         | APPS1: ----.- APPS2: ----.-\n------------------------------------------+---------------------------------------------\n WHEELS                                  | REAR SENSORS (AQT7)\n FL: --- km/h  T:---.-°C  BrkT:---.-°C  | NTC1: ---.-°C  NTC2: ---.-°C\n FR: --- km/h  T:---.-°C  BrkT:---.-°C  | NTC3: ---.-°C\n RL: --- km/h  T:---.-°C  BrkT:---.-°C  | Susp R: ---.- mm  Susp L: ---.- mm\n RR: --- km/h  T:---.-°C  BrkT:---.-°C  | IGN: ---  R2D: ---\n------------------------------------------+---------------------------------------------");
        }
    }
    
    tick_screen_debug();
}

void tick_screen_debug() {
    TelemetryData t;
    if (!ros2subscriber_get_telemetry(&t)) {
        return;
    }

    char buf[4096];
    
    // Status strings
    const char *ams_state_str = (t.ams_state == 1) ? "#00FF00 RUNNING#" : ((t.ams_state == 0) ? "#808080 IDLE#" : "#FF0000 FAULT#");
    const char *precharge_state_str = (t.precharge_state == 2) ? "#00FF00 DONE#" : 
                                      ((t.precharge_state == 1) ? "#FFA500 CHG#" : 
                                      ((t.precharge_state == 0) ? "#808080 OFF#" : "#FF0000 FAULT#"));

    const char *air_pos_str = (t.air_pos == 1) ? "#00FF00 ON#" : "#808080 OFF#";
    const char *air_min_str = (t.air_min == 1) ? "#00FF00 ON#" : "#808080 OFF#";
    const char *ctc_chg_str = (t.ctc_charge == 1) ? "#00FF00 ON#" : "#808080 OFF#";
    const char *ctc_dis_str = (t.ctc_discharge == 1) ? "#00FF00 ON#" : "#808080 OFF#";

    const char *inv_drive_str = (t.inv_drive_en == 1) ? "#00FF00 ENABLED#" : "#808080 DISABLED#";
    const char *rear_ign_str = (t.rear_ign == 1) ? "#00FF00 ON#" : "#808080 OFF#";
    const char *rear_r2d_str = (t.rear_r2d == 1) ? "#00FF00 ON#" : "#808080 OFF#";

    // Value color mappings
    const char *soc_color = (t.ams_soc > 30) ? "#00FF00" : ((t.ams_soc > 15) ? "#FFA500" : "#FF0000");
    const char *pec_color = (t.ams_pec_err == 0) ? "#00FF00" : "#FF0000";
    const char *fault_color = (t.ams_fault_cnt == 0) ? "#00FF00" : "#FF0000";
    const char *inv_fault_color = (t.inv_fault == 0) ? "#00FF00" : "#FF0000";
    const char *tmax_color = (t.t_max < 50.0f) ? "#00FF00" : "#FF0000";
    const char *vmin_color = (t.v_min > 3.0f) ? "#00FF00" : "#FF0000";

    snprintf(buf, sizeof(buf),
        " #00BFFF GENERAL DEBUG#\n"
        "------------------------------------------+---------------------------------------------\n"
        " #00BFFF AMS#                                     | #00BFFF INVERTER (HV500)#\n"
        " State:    %-25s      | ERPM:     #FFFFFF %-8.0f# Duty: #FFFFFF %5.1f%%#\n"
        " SOC:      %s%5.1f%%#                         | Vin:      #FFFFFF %-6.0f# V  Fault: %s%-3.0f#\n"
        " Runtime:  #FFFFFF %5.0f s#                       | AC Curr:  #FFFFFF %5.1f Apk# DC: #FFFFFF %5.1f A#\n"
        " MCU Vref: #FFFFFF %4.2f V#  MCU T: #FFFFFF %3.0f°C#      | Ctrl T:   #FFFFFF %5.1f°C#  Mot T:#FFFFFF %5.1f°C#\n"
        " PEC Err:  %s%3.0f#      Flt Cnt: %s%3.0f#       | Throttle: #FFFFFF %3.0f%%#    Brake:#FFFFFF %3.0f%%#\n"
        " Slaves:   #FFFFFF %2.0f/12#     FW: #FFFFFF %-8.0f#      | FOC Id:   #FFFFFF %5.1f Apk# Iq:  #FFFFFF %5.1f Apk#\n"
        " Fault:    %s%-25.0f#      | DriveEn:  %-25s\n"
        "------------------------------------------+---------------------------------------------\n"
        " #00BFFF PRECHARGE#                               | #00BFFF IVT (ISABELLE)#\n"
        " State:    %-25s      | Current:  #FFFFFF %8.0f mA#\n"
        " AIRpos:   %-15s AIRmin: %-15s | U1: #FFFFFF %7.0f mV#  U2: #FFFFFF %7.0f mV#\n"
        " CTCchg:   %-15s CTCdis: %-15s | U3: #FFFFFF %7.0f mV#  Temp:#FFFFFF %5.1f°C#\n"
        "                                          | Power:   #FFFFFF %7.0f W#  Energy:#FFFFFF %7.0f Wh#\n"
        " #00BFFF PACK OVERVIEW#                           |\n"
        " Vmax: #FFFFFF %5.3f V#   Vmin: %s%5.3f V#           | #00BFFF PEDALS / DASHBOARD#\n"
        " Tmax: %s%5.1f°C#  Tmin: #FFFFFF %5.1f°C#           | APPS1:   #FFFFFF %6.1f#  APPS2: #FFFFFF %6.1f#\n"
        "------------------------------------------+---------------------------------------------\n"
        " #00BFFF WHEELS#                                  | #00BFFF REAR SENSORS (AQT7)#\n"
        " FL: #FFFFFF %3.0f km/h# T:#FFFFFF %4.1f°C# BrkT:#FFFFFF %4.1f°C# | NTC1: #FFFFFF %4.1f°C# NTC2: #FFFFFF %4.1f°C#\n"
        " FR: #FFFFFF %3.0f km/h# T:#FFFFFF %4.1f°C# BrkT:#FFFFFF %4.1f°C# | NTC3: #FFFFFF %4.1f°C#\n"
        " RL: #FFFFFF %3.0f km/h# T:#FFFFFF %4.1f°C# BrkT:#FFFFFF %4.1f°C# | Susp R:#FFFFFF %5.1f mm# Susp L:#FFFFFF %5.1f mm#\n"
        " RR: #FFFFFF %3.0f km/h# T:#FFFFFF %4.1f°C# BrkT:#FFFFFF %4.1f°C# | IGN:  %-15s R2D:  %-15s\n"
        "------------------------------------------+---------------------------------------------\n",
        ams_state_str, t.inv_erpm, t.inv_duty,
        soc_color, t.ams_soc, t.inv_vin, inv_fault_color, t.inv_fault,
        t.ams_runtime, t.inv_ac_curr, t.inv_dc_curr,
        t.ams_mcu_vref, t.ams_mcu_temp, t.inv_temp_ctrl, t.inv_temp_mot,
        pec_color, t.ams_pec_err, fault_color, t.ams_fault_cnt, t.inv_throttle, t.inv_brake,
        t.ams_slaves, t.ams_fw, t.inv_foc_id, t.inv_foc_iq,
        fault_color, t.ams_fault_cnt, inv_drive_str,
        precharge_state_str, t.ivt_current,
        air_pos_str, air_min_str, t.ivt_u1, t.ivt_u2,
        ctc_chg_str, ctc_dis_str, t.ivt_u3, t.ivt_temp,
        t.ivt_power, t.ivt_energy,
        t.v_max, vmin_color, t.v_min,
        tmax_color, t.t_max, t.t_min, t.apps1, t.apps2,
        t.spd_fl, t.temp_fl, t.brk_fl, t.ntc1, t.ntc2,
        t.spd_fr, t.temp_fr, t.brk_fr, t.ntc3,
        t.spd_rl, t.temp_rl, t.brk_rl, t.susp_r, t.susp_l,
        t.spd_rr, t.temp_rr, t.brk_rr, rear_ign_str, rear_r2d_str
    );

    if (objects.debug_text_1) {
        my_spangroup_set_recolor_text(objects.debug_text_1, buf);
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
            // debugText
            lv_obj_t *obj = lv_spangroup_create(parent_obj);
            objects.debug_text = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 800, 480);
            lv_spangroup_set_mode(obj, LV_SPAN_MODE_FIXED);
            lv_spangroup_set_align(obj, LV_TEXT_ALIGN_LEFT);
            my_spangroup_set_recolor_text(obj, " AUTONOMOUS DEBUG  1/2                                                     \n ACU                                     | DV DYNAMICS\n ACU State:  ---------  AS: --------     | Spd Act: --- km/h   Spd Tgt: --- km/h\n ASSI:       ---------  Mission: ---     | Str Act: ---.-°     Str Tgt: ---.-°\n ASMS: ---  IGN: ---  EMERGENCY: ---     | Brk Act: ---%       Brk Tgt: ---%\n Emerg Cause: ------------------         | Mot Act: ---%       Mot Tgt: ---%\n CPU Temp: ---°C                         | Acc Lon: ---.--- m/s2\n                                         | Acc Lat: ---.--- m/s2\n JETSON                                  | Yaw:    ---.--- °/s\n AS State: ---  Mission: ---             |\n Temp: ---°C  CPU: ---% GPU: ---%       | DV STATUS\n Emerg Cause: ----------------           | AS: ---  EBS: ---  AMI: ---\n                                         | Steer: ---  EBS Red: ---\n------------------------------------------+---------------------------------------------\n VCU                                     | EBS / ASF PRESSURES\n IGN Man: ---  IGN Auto: ---             | EBS Tank F: ---- bar  R: ---- bar\n R2D Man: ---  R2D Auto: ---             | Brk Press F: ---- bar  R: ---- bar\n Shutdown: ---  State: ---               |\n R2D Raw: ---  IGN Raw: ---              | CUBEMARS STEERING\n HV: ---  BrkF: ---  BrkR: ---          | Pos: -----.-°  Spd: ------ ERPM\n                                         | Curr: ---.- A  Drv T: ---°C  Err: ---\n VCU RPM                                 | Pos Target: ------ °\n RPM Act: -----  RPM Tgt: -----         |    Torque Tgt: -----                                      | SLAM / CONES | Laps: --  Cones: ---  All: ------");
        }
    }
    
    tick_screen_debug_autonomous();
}

void tick_screen_debug_autonomous() {
    TelemetryData t;
    if (!ros2subscriber_get_telemetry(&t)) {
        return;
    }

    char buf[4096];

    // Status strings
    const char *acu_state_str = "#FF0000 FAULT#";
    if (t.acu_state == 0) acu_state_str = "#808080 INIT#";
    else if (t.acu_state == 1) acu_state_str = "#FFA500 MISS_SEL#";
    else if (t.acu_state == 2) acu_state_str = "#00BFFF JET_WAIT#";
    else if (t.acu_state == 3) acu_state_str = "#FFA500 SEQ#";
    else if (t.acu_state == 4) acu_state_str = "#00FF00 READY#";
    else if (t.acu_state == 5) acu_state_str = "#00FF00 DRIVING#";
    else if (t.acu_state == 7) acu_state_str = "#FF0000 EMERG#";

    const char *as_state_str = "#808080 OFF#";
    if (t.acu_as_state == 2) as_state_str = "#00FF00 READY#";
    else if (t.acu_as_state == 3) as_state_str = "#00FF00 DRIVING#";
    else if (t.acu_as_state == 4) as_state_str = "#FF0000 EMERG#";
    else if (t.acu_as_state == 5) as_state_str = "#00FF00 FINISH#";

    const char *assi_state_str = "#808080 OFF#";
    if (t.acu_assi_state == 2) assi_state_str = "#00FF00 READY#";
    else if (t.acu_assi_state == 3) assi_state_str = "#00FF00 DRIVING#";
    else if (t.acu_assi_state == 4) assi_state_str = "#FF0000 EMERG#";
    else if (t.acu_assi_state == 5) assi_state_str = "#00FF00 FINISH#";

    const char *acu_emerg_str = "#00FF00 NONE#";
    if (t.acu_emerg_cause == 1) acu_emerg_str = "#FF0000 SDC_OPEN#";
    else if (t.acu_emerg_cause == 2) acu_emerg_str = "#FF0000 RES#";
    else if (t.acu_emerg_cause == 3) acu_emerg_str = "#FF0000 PRESS_CHK#";
    else if (t.acu_emerg_cause == 4) acu_emerg_str = "#FF0000 VCU_TO#";
    else if (t.acu_emerg_cause == 5) acu_emerg_str = "#FF0000 JETSON_TO#";
    else if (t.acu_emerg_cause == 6) acu_emerg_str = "#FF0000 WDT#";

    const char *jet_as_state_str = "#808080 OFF#";
    if (t.jetson_as_state == 2) jet_as_state_str = "#00FF00 READY#";
    else if (t.jetson_as_state == 3) jet_as_state_str = "#00FF00 DRIVING#";
    else if (t.jetson_as_state == 4) jet_as_state_str = "#FF0000 EMERG#";
    else if (t.jetson_as_state == 5) jet_as_state_str = "#00FF00 FINISH#";

    const char *jet_emerg_str = "#00FF00 NONE#";
    if (t.jetson_emerg_cause == 1) jet_emerg_str = "#FF0000 SDC_OPEN#";
    else if (t.jetson_emerg_cause == 2) jet_emerg_str = "#FF0000 RES#";
    else if (t.jetson_emerg_cause == 3) jet_emerg_str = "#FF0000 STEER_ERR#";

    const char *acu_asms_str = (t.acu_asms == 1) ? "#00FF00 ON#" : "#808080 OFF#";
    const char *acu_ign_str = (t.acu_ign == 1) ? "#00FF00 ON#" : "#808080 OFF#";
    const char *acu_emerg_act_str = (t.acu_emergency == 1) ? "#FF0000 ACTIVE#" : "#00FF00 OK#";

    const char *vcu_ign_man_str = (t.vcu_ign_man == 1) ? "#00FF00 ON#" : "#808080 OFF#";
    const char *vcu_ign_auto_str = (t.vcu_ign_auto == 1) ? "#00FF00 ON#" : "#808080 OFF#";
    const char *vcu_r2d_man_str = (t.vcu_r2d_man == 1) ? "#00FF00 ON#" : "#808080 OFF#";
    const char *vcu_r2d_auto_str = (t.vcu_r2d_auto == 1) ? "#00FF00 ON#" : "#808080 OFF#";
    const char *vcu_shutdown_str = (t.vcu_shutdown == 1) ? "#00FF00 CLOSED#" : "#FF0000 OPEN#";
    const char *vcu_hv_str = (t.vcu_hv == 1) ? "#FF0000 ACTIVE#" : "#808080 OFF#";

    const char *steer_err_str = (t.steer_err == 0) ? "#00FF00 OK#" : "#FF0000 FAULT#";

    // Pedals/Pressures color mappings
    const char *ebs_press_color = (t.ebs_tank_f > 10.0f) ? "#00FF00" : "#FF0000";

    snprintf(buf, sizeof(buf),
        " #00BFFF AUTONOMOUS DEBUG  1/2#\n"
        "------------------------------------------+---------------------------------------------\n"
        " #00BFFF ACU#                                     | #00BFFF DV DYNAMICS#\n"
        " ACU State:  %-25s      | Spd Act:  #FFFFFF %3.1f# km/h  Spd Tgt: #FFFFFF %3.1f# km/h\n"
        " ASSI:       %-25s      | Str Act:  #FFFFFF %5.1f# °    Str Tgt: #FFFFFF %5.1f# °\n"
        " ASMS: %-10s IGN: %-10s         | Brk Act:  #FFFFFF %3.0f%%#        Brk Tgt: #FFFFFF %3.0f%%#\n"
        " EMERGENCY:  %-25s      | Mot Act:  #FFFFFF %3.0f%%#        Mot Tgt: #FFFFFF %3.0f%%#\n"
        " Emerg Cause:%-25s      | Acc Lon:  #FFFFFF %6.3f# m/s2 Acc Lat:#FFFFFF %6.3f# m/s2\n"
        " CPU Temp:   #FFFFFF %3.0f°C#                     | Yaw:      #FFFFFF %6.3f# °/s\n"
        "                                          |\n"
        " #00BFFF JETSON#                                  | #00BFFF DV STATUS#\n"
        " AS State:   %-25s      | AS: #FFFFFF %3.0f#  EBS: #FFFFFF %3.0f#  AMI: #FFFFFF %3.0f#\n"
        " Mission:    #FFFFFF %-25.0f#      | Steer: #FFFFFF %3.0f#  EBS Red: #FFFFFF %3.0f#\n"
        " Temp: #FFFFFF %3.0f°C#  CPU: #FFFFFF %3.0f%%#  GPU: #FFFFFF %3.0f%%#      |\n"
        " Emerg Cause:%-25s      | #00BFFF EBS / ASF PRESSURES#\n"
        "                                          | EBS Tank F: %s%5.1f# bar R: %s%5.1f# bar\n"
        "------------------------------------------+ Brk Press F: #FFFFFF %5.1f# bar R: #FFFFFF %5.1f# bar\n"
        " #00BFFF VCU#                                     |\n"
        " IGN Man: %-10s IGN Auto: %-10s   | #00BFFF CUBEMARS STEERING#\n"
        " R2D Man: %-10s R2D Auto: %-10s   | Pos: #FFFFFF %5.1f# °   Spd: #FFFFFF %6.0f# ERPM\n"
        " Shutdown:%-25s      | Curr: #FFFFFF %5.1f# A   Drv T: #FFFFFF %3.0f°C# Err: %s\n"
        " State:   #FFFFFF %-25.0f#      | Pos Target: #FFFFFF %5.1f# °\n"
        " HV:      %-25s      | Torque Tgt: #FFFFFF %5.2f# Nm\n"
        "                                          |\n"
        " #00BFFF VCU RPM#                                 | #00BFFF SLAM / CONES#\n"
        " RPM Act: #FFFFFF %-5.0f#       RPM Tgt: #FFFFFF %-5.0f#       | Laps: #FFFFFF %2.0f#  Cones: #FFFFFF %3.0f#  All: #FFFFFF %4.0f#\n"
        "------------------------------------------+---------------------------------------------\n",
        acu_state_str, t.dv_spd_act, t.dv_spd_tgt,
        assi_state_str, t.dv_str_act, t.dv_str_tgt,
        acu_asms_str, acu_ign_str, t.dv_brk_act, t.dv_brk_tgt,
        acu_emerg_act_str, t.dv_mot_act, t.dv_mot_tgt,
        acu_emerg_str, t.dv_acc_lon, t.dv_acc_lat,
        t.acu_cpu_temp, t.dv_yaw,
        jet_as_state_str, t.dv_as_status, t.dv_ebs_state, t.dv_ami_state,
        t.jetson_mission, t.dv_steer_state, t.dv_ebs_red_state,
        t.jetson_temp, t.jetson_cpu, t.jetson_gpu,
        jet_emerg_str, ebs_press_color, t.ebs_tank_f, ebs_press_color, t.ebs_tank_r,
        t.brk_press_f, t.brk_press_r,
        vcu_ign_man_str, vcu_ign_auto_str,
        vcu_r2d_man_str, vcu_r2d_auto_str, t.steer_pos, t.steer_spd,
        vcu_shutdown_str, t.steer_curr, t.steer_temp, steer_err_str,
        t.vcu_state, t.steer_pos_tgt,
        vcu_hv_str, t.vcu_torque_tgt,
        t.vcu_rpm_act, t.vcu_rpm_tgt, t.slam_laps, t.slam_cones, t.slam_all
    );

    if (objects.debug_text) {
        my_spangroup_set_recolor_text(objects.debug_text, buf);
    }
}

void create_screen_debug_autonomous_1() {
    void *flowState = getFlowState(0, 4);
    (void)flowState;
    lv_obj_t *obj = lv_obj_create(0);
    objects.debug_autonomous_1 = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 800, 480);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // debugText_2
            lv_obj_t *obj = lv_spangroup_create(parent_obj);
            objects.debug_text_2 = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 800, 480);
            lv_spangroup_set_mode(obj, LV_SPAN_MODE_FIXED);
            lv_spangroup_set_align(obj, LV_TEXT_ALIGN_LEFT);
            my_spangroup_set_recolor_text(obj, " AUTONOMOUS DEBUG 2/2   \n------------------------------------------+---------------------------------------------\n ACQUISITION BOARDS                      | RES (Remote Emergency)\n AQT1 BrkP:---.- bar RES:--- BOTS:---   | Signal: ---\n AQT2 WhlAng:-----.-° AQT3:-----.-°    |\n AQT4 StAng:-----.-° Inertia:--- Emer:---| AQT7 BrkP: ---.- bar\n------------------------------------------+---------------------------------------------\n                                                    \n \n");
        }
    }
    
    tick_screen_debug_autonomous_1();
}

void tick_screen_debug_autonomous_1() {
    TelemetryData t;
    if (!ros2subscriber_get_telemetry(&t)) {
        return;
    }

    char buf[2048];

    // Status strings
    const char *aqt1_res_str = (t.aqt1_res == 1) ? "#FF0000 ACTIVE#" : "#00FF00 OK#";
    const char *aqt1_bots_str = (t.aqt1_bots == 1) ? "#FF0000 ACTIVE#" : "#00FF00 OK#";
    const char *aqt4_inertia_str = (t.aqt4_inertia == 1) ? "#FF0000 ACTIVE#" : "#00FF00 OK#";
    const char *aqt4_emerg_str = (t.aqt4_emer == 1) ? "#FF0000 ACTIVE#" : "#00FF00 OK#";

    snprintf(buf, sizeof(buf),
        " #00BFFF AUTONOMOUS DEBUG 2/2#   \n"
        "------------------------------------------+---------------------------------------------\n"
        " #00BFFF ACQUISITION BOARDS#                      | #00BFFF RES (Remote Emergency)#\n"
        " AQT1 BrkP:  #FFFFFF %5.1f# bar                   | Signal: #FFFFFF %5.1f#\n"
        " AQT1 RES:   %-25s      |\n"
        " AQT1 BOTS:  %-25s      | #00BFFF REAR SENSORS (AQT7)#\n"
        " AQT2 WhlAng:#FFFFFF %5.1f# °                     | AQT7 BrkP:#FFFFFF %5.1f# bar\n"
        " AQT3 WhlAng:#FFFFFF %5.1f# °                     |\n"
        " AQT4 StAng: #FFFFFF %5.1f# °                     |\n"
        " AQT4 Inertia:%-25s      |\n"
        " AQT4 Emer:  %-25s      |\n"
        "------------------------------------------+---------------------------------------------\n",
        t.aqt1_brkp, t.res_signal,
        aqt1_res_str,
        aqt1_bots_str,
        t.aqt2_whl_ang, t.aqt7_brkp,
        t.aqt3_whl_ang,
        t.aqt4_st_ang,
        aqt4_inertia_str,
        aqt4_emerg_str
    );

    if (objects.debug_text_2) {
        my_spangroup_set_recolor_text(objects.debug_text_2, buf);
    }
}

typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_driver_view,
    tick_screen_autonomous,
    tick_screen_debug,
    tick_screen_debug_autonomous,
    tick_screen_debug_autonomous_1,
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
    create_screen_autonomous();
    create_screen_debug();
    create_screen_debug_autonomous();
    create_screen_debug_autonomous_1();
}