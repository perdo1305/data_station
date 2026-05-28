#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>

#include "ros2subscriber.h"
#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

// Custom recolor text parser for lv_spangroup
void my_spangroup_set_recolor_text_font(lv_obj_t *spangroup, const char *text, const lv_font_t *font) {
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
                lv_style_set_text_font(&span->style, font);
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
                lv_style_set_text_font(&span->style, font);
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
        lv_style_set_text_font(&span->style, font);
    }
    
    lv_spangroup_refr_mode(spangroup);
}

void my_spangroup_set_recolor_text(lv_obj_t *spangroup, const char *text) {
    my_spangroup_set_recolor_text_font(spangroup, text, &ui_font_orbitron_bold_15);
}

objects_t objects;

static const char *screen_names[] = { 
    "Driver View", 
    "Autonomous", 
    "Debug 1", 
    "Debug_Inverter 2", 
    "Debug_3", 
    "Debug_Wheels 4", 
    "Debug 5", 
    "Debug Autonomous 1", 
    "Debug Autonomous 2", 
    "Debug Autonomous 3", 
    "Debug Autonomous 4", 
    "Debug Autonomous 5" 
};
static const char *object_names[] = {
    "driver_view",
    "autonomous",
    "debug_1",
    "debug_inverter_2",
    "debug_3",
    "debug_wheels_4",
    "debug_5",
    "debug_autonomous_1",
    "debug_autonomous_2",
    "debug_autonomous_3",
    "debug_autonomous_4",
    "debug_autonomous_5",
    "ready_label",
    "middle_container",
    "temp_motor_container",
    "tempmotor_label",
    "lap_times_container",
    "laptime_label",
    "lastlap_label",
    "obj0",
    "obj1",
    "temp_inv_container",
    "temp_inv_label",
    "speed_container",
    "speed_label",
    "km_label",
    "brake_acell_presure_container",
    "brake_presure_bar",
    "accellerator_presure_bar",
    "obj2",
    "hv_bar",
    "lv_bar",
    "hv_label",
    "lv_label",
    "obj3",
    "hv_bar_1",
    "lv_bar_1",
    "hv_label_1",
    "lv_label_1",
    "brake_presure_container",
    "brake_presure_bar_1",
    "brake_presure_bar_2",
    "pneumaticresure_container",
    "brake_presure_bar_3",
    "brake_presure_bar_4",
    "lap_times_container_1",
    "laptime_label_1",
    "lastlap_label_1",
    "obj4",
    "obj5",
    "max_speed_container",
    "km_label_2",
    "km_label_1",
    "speed_label_1",
    "km_label_3",
    "general_debug_text_1",
    "general_debug_text_2",
    "general_debug_text_3",
    "general_debug_text_4",
    "general_debug_text_5",
    "autonomous_debug_text_1",
    "autonomous_debug_text_2",
    "autonomous_debug_text_3",
    "autonomous_debug_text_4",
    "autonomous_debug_text_5"
};

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
            lv_bar_set_range(obj, 0, 100);
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
            lv_bar_set_range(obj, 20, 28);
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
    TelemetryData t;
    if (ros2subscriber_get_telemetry(&t)) {
        ui_update_telemetry_vars(&t);
    }
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
        char formatted_val[128];
        snprintf(formatted_val, sizeof(formatted_val), "%s ºC", new_val);
        if (strcmp(formatted_val, cur_val) != 0) {
            tick_value_change_obj = objects.tempmotor_label;
            lv_label_set_text(objects.tempmotor_label, formatted_val);
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
        char formatted_val[128];
        snprintf(formatted_val, sizeof(formatted_val), "%s ºC", new_val);
        if (strcmp(formatted_val, cur_val) != 0) {
            tick_value_change_obj = objects.temp_inv_label;
            lv_label_set_text(objects.temp_inv_label, formatted_val);
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
            lv_bar_set_range(obj, 0, 100);
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
            lv_bar_set_range(obj, 20, 28);
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

static void table_draw_event_cb(lv_event_t * e) {
    lv_draw_task_t * draw_task = lv_event_get_draw_task(e);
    if (!draw_task) return;
    lv_draw_dsc_base_t * base_dsc = draw_task->draw_dsc;
    if (!base_dsc) return;

    if(base_dsc->part == LV_PART_ITEMS) {
        lv_obj_t * obj = lv_event_get_current_target(e);
        if(!obj) return;
        if(lv_obj_get_class(obj) != &lv_table_class) return;

        uint16_t row = base_dsc->id1;
        uint16_t col = base_dsc->id2;
        
        uint32_t row_cnt = lv_table_get_row_count(obj);
        uint32_t col_cnt = lv_table_get_column_count(obj);
        if(row >= row_cnt || col >= col_cnt) return;

        uint32_t *color_ptr = (uint32_t *)lv_table_get_cell_user_data(obj, row, col);
        if(color_ptr != NULL) {
            uint32_t color_val = *color_ptr;
            lv_draw_label_dsc_t * label_draw_dsc = lv_draw_task_get_label_dsc(draw_task);
            if(label_draw_dsc) {
                label_draw_dsc->color = lv_color_hex(color_val);
            }
        }

        lv_draw_fill_dsc_t * fill_draw_dsc = lv_draw_task_get_fill_dsc(draw_task);
        if(fill_draw_dsc) {
            if(row % 2 == 0) {
                fill_draw_dsc->color = lv_color_hex(0x000000);
            }
            else {
                fill_draw_dsc->color = lv_color_hex(0x1a1a1a);
            }
        }
    }
}

static void set_cell(lv_obj_t *table, uint16_t row, uint16_t col, const char *txt, uint32_t color) {
    lv_table_set_cell_value(table, row, col, txt);
    uint32_t *color_ptr = (uint32_t *)lv_table_get_cell_user_data(table, row, col);
    if (color_ptr) {
        *color_ptr = color;
    } else {
        color_ptr = lv_malloc(sizeof(uint32_t));
        if (color_ptr) {
            *color_ptr = color;
            lv_table_set_cell_user_data(table, row, col, color_ptr);
        }
    }
}

static void set_cell_fmt(lv_obj_t *table, uint16_t row, uint16_t col, uint32_t color, const char *fmt, ...) {
    char buf[256];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    set_cell(table, row, col, buf, color);
}

static lv_obj_t *create_debug_table(lv_obj_t *parent_obj, uint32_t row_cnt) {
    lv_obj_t *obj = lv_table_create(parent_obj);
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 800, 480);
    lv_table_set_column_count(obj, 4);
    lv_table_set_row_count(obj, row_cnt);
    lv_table_set_column_width(obj, 0, 190);
    lv_table_set_column_width(obj, 1, 210);
    lv_table_set_column_width(obj, 2, 190);
    lv_table_set_column_width(obj, 3, 210);
    
    // Set styles for dark dashboard table
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_PART_ITEMS);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x2a2a2a), LV_PART_ITEMS);
    lv_obj_set_style_border_color(obj, lv_color_hex(0x444444), LV_PART_ITEMS);
    lv_obj_set_style_border_width(obj, 1, LV_PART_ITEMS);
    
    // Decrease row height by adjusting padding
    lv_obj_set_style_pad_top(obj, 2, LV_PART_ITEMS);
    lv_obj_set_style_pad_bottom(obj, 2, LV_PART_ITEMS);
    lv_obj_set_style_pad_left(obj, 8, LV_PART_ITEMS);
    lv_obj_set_style_pad_right(obj, 8, LV_PART_ITEMS);
    
    // Set font
    lv_obj_set_style_text_font(obj, &ui_font_orbitron_25, LV_PART_ITEMS);

    // Add custom color and alignment draw event callback
    lv_obj_add_event_cb(obj, table_draw_event_cb, LV_EVENT_DRAW_TASK_ADDED, NULL);
    lv_obj_add_flag(obj, LV_OBJ_FLAG_SEND_DRAW_TASK_EVENTS);
    return obj;
}

void create_screen_debug_1() {
    void *flowState = getFlowState(0, 2);
    (void)flowState;
    lv_obj_t *obj = lv_obj_create(0);
    objects.debug_1 = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 800, 480);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        objects.general_debug_text_1 = create_debug_table(obj, 13);
    }
    tick_screen_debug_1();
}

void tick_screen_debug_1() {
    TelemetryData t;
    if (!ros2subscriber_get_telemetry(&t)) {
        return;
    }
    lv_obj_t *table = objects.general_debug_text_1;
    if (!table) return;

    // Header
    set_cell(table, 0, 0, "AMS / PRECHARGE / PACK (1/5)", 0x00BFFF);
    lv_table_add_cell_ctrl(table, 0, 0, LV_TABLE_CELL_CTRL_MERGE_RIGHT);
    lv_table_add_cell_ctrl(table, 0, 1, LV_TABLE_CELL_CTRL_MERGE_RIGHT);
    lv_table_add_cell_ctrl(table, 0, 2, LV_TABLE_CELL_CTRL_MERGE_RIGHT);

    // Row 1
    const char *ams_state_str = "FAULT";
    uint32_t ams_state_color = 0xFF0000;
    if (t.ams_state == 0) { ams_state_str = "IDLE"; ams_state_color = 0x808080; }
    else if (t.ams_state == 1) { ams_state_str = "RUNNING"; ams_state_color = 0x00FF00; }
    set_cell(table, 1, 0, "AMS State:", 0xFFFFFF);
    set_cell(table, 1, 1, ams_state_str, ams_state_color);

    uint32_t soc_color = (t.ams_soc > 30) ? 0x00FF00 : ((t.ams_soc > 15) ? 0xFFA500 : 0xFF0000);
    set_cell(table, 1, 2, "SOC:", 0xFFFFFF);
    set_cell_fmt(table, 1, 3, soc_color, "%5.1f%%", t.ams_soc);

    // Row 2
    set_cell(table, 2, 0, "Runtime:", 0xFFFFFF);
    set_cell_fmt(table, 2, 1, 0xFFFFFF, "%5.0f s", t.ams_runtime);
    set_cell(table, 2, 2, "Fans:", 0xFFFFFF);
    set_cell_fmt(table, 2, 3, 0xFFFFFF, "%3.0f%%", t.ams_fans);

    // Row 3
    set_cell(table, 3, 0, "MCU Vref:", 0xFFFFFF);
    set_cell_fmt(table, 3, 1, 0xFFFFFF, "%4.2f V", t.ams_mcu_vref);
    set_cell(table, 3, 2, "MCU T:", 0xFFFFFF);
    set_cell_fmt(table, 3, 3, 0xFFFFFF, "%3.0f C", t.ams_mcu_temp);

    // Row 4
    uint32_t pec_color = (t.ams_pec_err == 0) ? 0x00FF00 : 0xFF0000;
    set_cell(table, 4, 0, "PEC Err:", 0xFFFFFF);
    set_cell_fmt(table, 4, 1, pec_color, "%3.0f", t.ams_pec_err);

    uint32_t fault_color = (t.ams_fault_cnt == 0) ? 0x00FF00 : 0xFF0000;
    set_cell(table, 4, 2, "Flt Cnt:", 0xFFFFFF);
    set_cell_fmt(table, 4, 3, fault_color, "%3.0f", t.ams_fault_cnt);

    // Row 5
    set_cell(table, 5, 0, "Slaves:", 0xFFFFFF);
    set_cell_fmt(table, 5, 1, 0xFFFFFF, "%2.0f/12", t.ams_slaves);
    set_cell(table, 5, 2, "FW:", 0xFFFFFF);
    set_cell_fmt(table, 5, 3, 0xFFFFFF, "%-8.0f", t.ams_fw);

    // Row 6: Header Precharge
    set_cell(table, 6, 0, "PRECHARGE", 0x00BFFF);
    lv_table_add_cell_ctrl(table, 6, 0, LV_TABLE_CELL_CTRL_MERGE_RIGHT);
    lv_table_add_cell_ctrl(table, 6, 1, LV_TABLE_CELL_CTRL_MERGE_RIGHT);
    lv_table_add_cell_ctrl(table, 6, 2, LV_TABLE_CELL_CTRL_MERGE_RIGHT);

    // Row 7
    const char *precharge_state_str = "FAULT";
    uint32_t precharge_state_color = 0xFF0000;
    if (t.precharge_state == 0) { precharge_state_str = "OFF"; precharge_state_color = 0x808080; }
    else if (t.precharge_state == 1) { precharge_state_str = "CHG"; precharge_state_color = 0xFFA500; }
    else if (t.precharge_state == 2) { precharge_state_str = "DONE"; precharge_state_color = 0x00FF00; }
    set_cell(table, 7, 0, "Precharge:", 0xFFFFFF);
    set_cell(table, 7, 1, precharge_state_str, precharge_state_color);
    set_cell(table, 7, 2, "", 0xFFFFFF);
    set_cell(table, 7, 3, "", 0xFFFFFF);

    // Row 8
    set_cell(table, 8, 0, "AIR+:", 0xFFFFFF);
    set_cell(table, 8, 1, (t.air_pos == 1) ? "ON" : "OFF", (t.air_pos == 1) ? 0x00FF00 : 0x808080);
    set_cell(table, 8, 2, "AIR-:", 0xFFFFFF);
    set_cell(table, 8, 3, (t.air_min == 1) ? "ON" : "OFF", (t.air_min == 1) ? 0x00FF00 : 0x808080);

    // Row 9
    set_cell(table, 9, 0, "CHG:", 0xFFFFFF);
    set_cell(table, 9, 1, (t.ctc_charge == 1) ? "ON" : "OFF", (t.ctc_charge == 1) ? 0x00FF00 : 0x808080);
    set_cell(table, 9, 2, "DIS:", 0xFFFFFF);
    set_cell(table, 9, 3, (t.ctc_discharge == 1) ? "ON" : "OFF", (t.ctc_discharge == 1) ? 0x00FF00 : 0x808080);

    // Row 10: Header Pack
    set_cell(table, 10, 0, "PACK VOLTAGE & TEMP", 0x00BFFF);
    lv_table_add_cell_ctrl(table, 10, 0, LV_TABLE_CELL_CTRL_MERGE_RIGHT);
    lv_table_add_cell_ctrl(table, 10, 1, LV_TABLE_CELL_CTRL_MERGE_RIGHT);
    lv_table_add_cell_ctrl(table, 10, 2, LV_TABLE_CELL_CTRL_MERGE_RIGHT);

    // Row 11
    set_cell(table, 11, 0, "Vmax:", 0xFFFFFF);
    set_cell_fmt(table, 11, 1, 0xFFFFFF, "%5.3f V", t.v_max);
    set_cell(table, 11, 2, "Vmin:", 0xFFFFFF);
    set_cell_fmt(table, 11, 3, (t.v_min > 3.0f) ? 0x00FF00 : 0xFF0000, "%5.3f V", t.v_min);

    // Row 12
    set_cell(table, 12, 0, "Tmax:", 0xFFFFFF);
    set_cell_fmt(table, 12, 1, (t.t_max < 50.0f) ? 0x00FF00 : 0xFF0000, "%5.1f C", t.t_max);
    set_cell(table, 12, 2, "Tmin:", 0xFFFFFF);
    set_cell_fmt(table, 12, 3, 0xFFFFFF, "%5.1f C", t.t_min);
}

void create_screen_debug_inverter_2() {
    void *flowState = getFlowState(0, 3);
    (void)flowState;
    lv_obj_t *obj = lv_obj_create(0);
    objects.debug_inverter_2 = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 800, 480);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        objects.general_debug_text_2 = create_debug_table(obj, 8);
    }
    tick_screen_debug_inverter_2();
}

void tick_screen_debug_inverter_2() {
    TelemetryData t;
    if (!ros2subscriber_get_telemetry(&t)) {
        return;
    }
    lv_obj_t *table = objects.general_debug_text_2;
    if (!table) return;

    // Header
    set_cell(table, 0, 0, "INVERTER DTI (2/5)", 0x00BFFF);
    lv_table_add_cell_ctrl(table, 0, 0, LV_TABLE_CELL_CTRL_MERGE_RIGHT);
    lv_table_add_cell_ctrl(table, 0, 1, LV_TABLE_CELL_CTRL_MERGE_RIGHT);
    lv_table_add_cell_ctrl(table, 0, 2, LV_TABLE_CELL_CTRL_MERGE_RIGHT);

    // Row 1
    set_cell(table, 1, 0, "ERPM:", 0xFFFFFF);
    set_cell_fmt(table, 1, 1, 0xFFFFFF, "%-8.0f", t.inv_erpm);
    set_cell(table, 1, 2, "Duty:", 0xFFFFFF);
    set_cell_fmt(table, 1, 3, 0xFFFFFF, "%5.1f%%", t.inv_duty);

    // Row 2
    set_cell(table, 2, 0, "Vin:", 0xFFFFFF);
    set_cell_fmt(table, 2, 1, 0xFFFFFF, "%-6.0f V", t.inv_vin);
    set_cell(table, 2, 2, "Fault:", 0xFFFFFF);
    set_cell_fmt(table, 2, 3, (t.inv_fault == 0) ? 0x00FF00 : 0xFF0000, "%-3.0f", t.inv_fault);

    // Row 3
    set_cell(table, 3, 0, "AC Curr:", 0xFFFFFF);
    set_cell_fmt(table, 3, 1, 0xFFFFFF, "%5.1f Apk", t.inv_ac_curr);
    set_cell(table, 3, 2, "DC Curr:", 0xFFFFFF);
    set_cell_fmt(table, 3, 3, 0xFFFFFF, "%5.1f A", t.inv_dc_curr);

    // Row 4
    set_cell(table, 4, 0, "Ctrl T:", 0xFFFFFF);
    set_cell_fmt(table, 4, 1, 0xFFFFFF, "%5.1f C", t.inv_temp_ctrl);
    set_cell(table, 4, 2, "Mot T:", 0xFFFFFF);
    set_cell_fmt(table, 4, 3, 0xFFFFFF, "%5.1f C", t.inv_temp_mot);

    // Row 5
    set_cell(table, 5, 0, "Throttle:", 0xFFFFFF);
    set_cell_fmt(table, 5, 1, 0xFFFFFF, "%3.0f%%", t.inv_throttle);
    set_cell(table, 5, 2, "Brake:", 0xFFFFFF);
    set_cell_fmt(table, 5, 3, 0xFFFFFF, "%3.0f%%", t.inv_brake);

    // Row 6
    set_cell(table, 6, 0, "FOC Id:", 0xFFFFFF);
    set_cell_fmt(table, 6, 1, 0xFFFFFF, "%5.1f Apk", t.inv_foc_id);
    set_cell(table, 6, 2, "FOC Iq:", 0xFFFFFF);
    set_cell_fmt(table, 6, 3, 0xFFFFFF, "%5.1f Apk", t.inv_foc_iq);

    // Row 7
    set_cell(table, 7, 0, "DriveEn:", 0xFFFFFF);
    set_cell(table, 7, 1, (t.inv_drive_en == 1) ? "ENABLED" : "DISABLED", (t.inv_drive_en == 1) ? 0x00FF00 : 0x808080);
    set_cell(table, 7, 2, "", 0xFFFFFF);
    set_cell(table, 7, 3, "", 0xFFFFFF);
}

void create_screen_debug_3() {
    void *flowState = getFlowState(0, 4);
    (void)flowState;
    lv_obj_t *obj = lv_obj_create(0);
    objects.debug_3 = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 800, 480);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        objects.general_debug_text_3 = create_debug_table(obj, 11);
    }
    tick_screen_debug_3();
}

void tick_screen_debug_3() {
    TelemetryData t;
    if (!ros2subscriber_get_telemetry(&t)) {
        return;
    }
    lv_obj_t *table = objects.general_debug_text_3;
    if (!table) return;

    // Header
    set_cell(table, 0, 0, "IVT / PEDALS / REAR SENSORS (3/5)", 0x00BFFF);
    lv_table_add_cell_ctrl(table, 0, 0, LV_TABLE_CELL_CTRL_MERGE_RIGHT);
    lv_table_add_cell_ctrl(table, 0, 1, LV_TABLE_CELL_CTRL_MERGE_RIGHT);
    lv_table_add_cell_ctrl(table, 0, 2, LV_TABLE_CELL_CTRL_MERGE_RIGHT);

    // Row 1
    set_cell(table, 1, 0, "Current:", 0xFFFFFF);
    set_cell_fmt(table, 1, 1, 0xFFFFFF, "%8.0f mA", t.ivt_current);
    set_cell(table, 1, 2, "Temp:", 0xFFFFFF);
    set_cell_fmt(table, 1, 3, 0xFFFFFF, "%5.1f C", t.ivt_temp);

    // Row 2
    set_cell(table, 2, 0, "U1:", 0xFFFFFF);
    set_cell_fmt(table, 2, 1, 0xFFFFFF, "%7.0f mV", t.ivt_u1);
    set_cell(table, 2, 2, "U2:", 0xFFFFFF);
    set_cell_fmt(table, 2, 3, 0xFFFFFF, "%7.0f mV", t.ivt_u2);

    // Row 3
    set_cell(table, 3, 0, "U3:", 0xFFFFFF);
    set_cell_fmt(table, 3, 1, 0xFFFFFF, "%7.0f mV", t.ivt_u3);
    set_cell(table, 3, 2, "Power:", 0xFFFFFF);
    set_cell_fmt(table, 3, 3, 0xFFFFFF, "%7.0f W", t.ivt_power);

    // Row 4
    set_cell(table, 4, 0, "Energy:", 0xFFFFFF);
    set_cell_fmt(table, 4, 1, 0xFFFFFF, "%7.0f Wh", t.ivt_energy);
    set_cell(table, 4, 2, "", 0xFFFFFF);
    set_cell(table, 4, 3, "", 0xFFFFFF);

    // Row 5: Header Pedals & Rear Sensors
    set_cell(table, 5, 0, "PEDALS & REAR SENSORS", 0x00BFFF);
    lv_table_add_cell_ctrl(table, 5, 0, LV_TABLE_CELL_CTRL_MERGE_RIGHT);
    lv_table_add_cell_ctrl(table, 5, 1, LV_TABLE_CELL_CTRL_MERGE_RIGHT);
    lv_table_add_cell_ctrl(table, 5, 2, LV_TABLE_CELL_CTRL_MERGE_RIGHT);

    // Row 6
    set_cell(table, 6, 0, "APPS1:", 0xFFFFFF);
    set_cell_fmt(table, 6, 1, 0xFFFFFF, "%6.1f", t.apps1);
    set_cell(table, 6, 2, "APPS2:", 0xFFFFFF);
    set_cell_fmt(table, 6, 3, 0xFFFFFF, "%6.1f", t.apps2);

    // Row 7
    set_cell(table, 7, 0, "NTC1:", 0xFFFFFF);
    set_cell_fmt(table, 7, 1, 0xFFFFFF, "%4.1f C", t.ntc1);
    set_cell(table, 7, 2, "NTC2:", 0xFFFFFF);
    set_cell_fmt(table, 7, 3, 0xFFFFFF, "%4.1f C", t.ntc2);

    // Row 8
    set_cell(table, 8, 0, "NTC3:", 0xFFFFFF);
    set_cell_fmt(table, 8, 1, 0xFFFFFF, "%4.1f C", t.ntc3);
    set_cell(table, 8, 2, "", 0xFFFFFF);
    set_cell(table, 8, 3, "", 0xFFFFFF);

    // Row 9
    set_cell(table, 9, 0, "Susp R:", 0xFFFFFF);
    set_cell_fmt(table, 9, 1, 0xFFFFFF, "%5.1f mm", t.susp_r);
    set_cell(table, 9, 2, "Susp L:", 0xFFFFFF);
    set_cell_fmt(table, 9, 3, 0xFFFFFF, "%5.1f mm", t.susp_l);

    // Row 10
    set_cell(table, 10, 0, "IGN:", 0xFFFFFF);
    set_cell(table, 10, 1, (t.rear_ign == 1) ? "ON" : "OFF", (t.rear_ign == 1) ? 0x00FF00 : 0x808080);
    set_cell(table, 10, 2, "R2D:", 0xFFFFFF);
    set_cell(table, 10, 3, (t.rear_r2d == 1) ? "ON" : "OFF", (t.rear_r2d == 1) ? 0x00FF00 : 0x808080);
}

void create_screen_debug_wheels_4() {
    void *flowState = getFlowState(0, 5);
    (void)flowState;
    lv_obj_t *obj = lv_obj_create(0);
    objects.debug_wheels_4 = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 800, 480);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        objects.general_debug_text_4 = create_debug_table(obj, 9);
    }
    tick_screen_debug_wheels_4();
}

void tick_screen_debug_wheels_4() {
    TelemetryData t;
    if (!ros2subscriber_get_telemetry(&t)) {
        return;
    }
    lv_obj_t *table = objects.general_debug_text_4;
    if (!table) return;

    // Header
    set_cell(table, 0, 0, "WHEELS STATUS (4/5)", 0x00BFFF);
    lv_table_add_cell_ctrl(table, 0, 0, LV_TABLE_CELL_CTRL_MERGE_RIGHT);
    lv_table_add_cell_ctrl(table, 0, 1, LV_TABLE_CELL_CTRL_MERGE_RIGHT);
    lv_table_add_cell_ctrl(table, 0, 2, LV_TABLE_CELL_CTRL_MERGE_RIGHT);

    // Row 1: FL Wheel
    set_cell(table, 1, 0, "FL Speed:", 0xFFFFFF);
    set_cell_fmt(table, 1, 1, 0xFFFFFF, "%3.0f km/h", t.spd_fl);
    set_cell(table, 1, 2, "FL Temp:", 0xFFFFFF);
    set_cell_fmt(table, 1, 3, 0xFFFFFF, "%4.1f C", t.temp_fl);

    // Row 2: FL Brake T
    set_cell(table, 2, 0, "FL BrkT:", 0xFFFFFF);
    set_cell_fmt(table, 2, 1, 0xFFFFFF, "%4.1f C", t.brk_fl);
    set_cell(table, 2, 2, "", 0xFFFFFF);
    set_cell(table, 2, 3, "", 0xFFFFFF);

    // Row 3: FR Wheel
    set_cell(table, 3, 0, "FR Speed:", 0xFFFFFF);
    set_cell_fmt(table, 3, 1, 0xFFFFFF, "%3.0f km/h", t.spd_fr);
    set_cell(table, 3, 2, "FR Temp:", 0xFFFFFF);
    set_cell_fmt(table, 3, 3, 0xFFFFFF, "%4.1f C", t.temp_fr);

    // Row 4: FR Brake T
    set_cell(table, 4, 0, "FR BrkT:", 0xFFFFFF);
    set_cell_fmt(table, 4, 1, 0xFFFFFF, "%4.1f C", t.brk_fr);
    set_cell(table, 4, 2, "", 0xFFFFFF);
    set_cell(table, 4, 3, "", 0xFFFFFF);

    // Row 5: RL Wheel
    set_cell(table, 5, 0, "RL Speed:", 0xFFFFFF);
    set_cell_fmt(table, 5, 1, 0xFFFFFF, "%3.0f km/h", t.spd_rl);
    set_cell(table, 5, 2, "RL Temp:", 0xFFFFFF);
    set_cell_fmt(table, 5, 3, 0xFFFFFF, "%4.1f C", t.temp_rl);

    // Row 6: RL Brake T
    set_cell(table, 6, 0, "RL BrkT:", 0xFFFFFF);
    set_cell_fmt(table, 6, 1, 0xFFFFFF, "%4.1f C", t.brk_rl);
    set_cell(table, 6, 2, "", 0xFFFFFF);
    set_cell(table, 6, 3, "", 0xFFFFFF);

    // Row 7: RR Wheel
    set_cell(table, 7, 0, "RR Speed:", 0xFFFFFF);
    set_cell_fmt(table, 7, 1, 0xFFFFFF, "%3.0f km/h", t.spd_rr);
    set_cell(table, 7, 2, "RR Temp:", 0xFFFFFF);
    set_cell_fmt(table, 7, 3, 0xFFFFFF, "%4.1f C", t.temp_rr);

    // Row 8: RR Brake T
    set_cell(table, 8, 0, "RR BrkT:", 0xFFFFFF);
    set_cell_fmt(table, 8, 1, 0xFFFFFF, "%4.1f C", t.brk_rr);
    set_cell(table, 8, 2, "", 0xFFFFFF);
    set_cell(table, 8, 3, "", 0xFFFFFF);
}

void create_screen_debug_5() {
    void *flowState = getFlowState(0, 6);
    (void)flowState;
    lv_obj_t *obj = lv_obj_create(0);
    objects.debug_5 = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 800, 480);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        objects.general_debug_text_5 = create_debug_table(obj, 4);
    }
    tick_screen_debug_5();
}

void tick_screen_debug_5() {
    TelemetryData t;
    if (!ros2subscriber_get_telemetry(&t)) {
        return;
    }
    lv_obj_t *table = objects.general_debug_text_5;
    if (!table) return;

    // Header
    set_cell(table, 0, 0, "GENERAL STATUS (5/5)", 0x00BFFF);
    lv_table_add_cell_ctrl(table, 0, 0, LV_TABLE_CELL_CTRL_MERGE_RIGHT);
    lv_table_add_cell_ctrl(table, 0, 1, LV_TABLE_CELL_CTRL_MERGE_RIGHT);
    lv_table_add_cell_ctrl(table, 0, 2, LV_TABLE_CELL_CTRL_MERGE_RIGHT);

    // Row 1
    set_cell(table, 1, 0, "System Status:", 0xFFFFFF);
    set_cell(table, 1, 1, "OK", 0x00FF00);
    set_cell(table, 1, 2, "Domain ID:", 0xFFFFFF);
    set_cell(table, 1, 3, "0", 0xFFFFFF);

    // Row 2
    set_cell(table, 2, 0, "ROS2 Node:", 0xFFFFFF);
    set_cell(table, 2, 1, "ACTIVE", 0x00FF00);
    set_cell(table, 2, 2, "AMS Flt Count:", 0xFFFFFF);
    set_cell_fmt(table, 2, 3, (t.ams_fault_cnt == 0) ? 0x00FF00 : 0xFF0000, "%.0f", t.ams_fault_cnt);

    // Row 3
    set_cell(table, 3, 0, "Inverter Flt:", 0xFFFFFF);
    set_cell_fmt(table, 3, 1, (t.inv_fault == 0) ? 0x00FF00 : 0xFF0000, "%.0f", t.inv_fault);
    set_cell(table, 3, 2, "", 0xFFFFFF);
    set_cell(table, 3, 3, "", 0xFFFFFF);
}

void create_screen_debug_autonomous_1() {
    void *flowState = getFlowState(0, 7);
    (void)flowState;
    lv_obj_t *obj = lv_obj_create(0);
    objects.debug_autonomous_1 = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 800, 480);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        objects.autonomous_debug_text_1 = create_debug_table(obj, 9);
    }
    tick_screen_debug_autonomous_1();
}

void tick_screen_debug_autonomous_1() {
    TelemetryData t;
    if (!ros2subscriber_get_telemetry(&t)) {
        return;
    }
    lv_obj_t *table = objects.autonomous_debug_text_1;
    if (!table) return;

    // Header
    set_cell(table, 0, 0, "ACU & JETSON STATUS (1/5)", 0x00BFFF);
    lv_table_add_cell_ctrl(table, 0, 0, LV_TABLE_CELL_CTRL_MERGE_RIGHT);
    lv_table_add_cell_ctrl(table, 0, 1, LV_TABLE_CELL_CTRL_MERGE_RIGHT);
    lv_table_add_cell_ctrl(table, 0, 2, LV_TABLE_CELL_CTRL_MERGE_RIGHT);

    // Row 1
    const char *acu_state_str = "FAULT";
    uint32_t acu_state_color = 0xFF0000;
    if (t.acu_state == 0) { acu_state_str = "INIT"; acu_state_color = 0x808080; }
    else if (t.acu_state == 1) { acu_state_str = "MISS_SEL"; acu_state_color = 0xFFA500; }
    else if (t.acu_state == 2) { acu_state_str = "JET_WAIT"; acu_state_color = 0x00BFFF; }
    else if (t.acu_state == 3) { acu_state_str = "SEQ"; acu_state_color = 0xFFA500; }
    else if (t.acu_state == 4) { acu_state_str = "READY"; acu_state_color = 0x00FF00; }
    else if (t.acu_state == 5) { acu_state_str = "DRIVING"; acu_state_color = 0x00FF00; }
    else if (t.acu_state == 7) { acu_state_str = "EMERG"; acu_state_color = 0xFF0000; }
    set_cell(table, 1, 0, "ACU State:", 0xFFFFFF);
    set_cell(table, 1, 1, acu_state_str, acu_state_color);

    const char *assi_state_str = "OFF";
    uint32_t assi_state_color = 0x808080;
    if (t.acu_assi_state == 2) { assi_state_str = "READY"; assi_state_color = 0x00FF00; }
    else if (t.acu_assi_state == 3) { assi_state_str = "DRIVING"; assi_state_color = 0x00FF00; }
    else if (t.acu_assi_state == 4) { assi_state_str = "EMERG"; assi_state_color = 0xFF0000; }
    else if (t.acu_assi_state == 5) { assi_state_str = "FINISH"; assi_state_color = 0x00FF00; }
    set_cell(table, 1, 2, "ASSI:", 0xFFFFFF);
    set_cell(table, 1, 3, assi_state_str, assi_state_color);

    // Row 2
    set_cell(table, 2, 0, "ASMS:", 0xFFFFFF);
    set_cell(table, 2, 1, (t.acu_asms == 1) ? "ON" : "OFF", (t.acu_asms == 1) ? 0x00FF00 : 0x808080);
    set_cell(table, 2, 2, "IGN:", 0xFFFFFF);
    set_cell(table, 2, 3, (t.acu_ign == 1) ? "ON" : "OFF", (t.acu_ign == 1) ? 0x00FF00 : 0x808080);

    // Row 3
    set_cell(table, 3, 0, "EMERGENCY:", 0xFFFFFF);
    set_cell(table, 3, 1, (t.acu_emergency == 1) ? "ACTIVE" : "OK", (t.acu_emergency == 1) ? 0xFF0000 : 0x00FF00);

    const char *acu_emerg_str = "NONE";
    uint32_t acu_emerg_color = 0x00FF00;
    if (t.acu_emerg_cause == 1) { acu_emerg_str = "SDC_OPEN"; acu_emerg_color = 0xFF0000; }
    else if (t.acu_emerg_cause == 2) { acu_emerg_str = "RES"; acu_emerg_color = 0xFF0000; }
    else if (t.acu_emerg_cause == 3) { acu_emerg_str = "PRESS_CHK"; acu_emerg_color = 0xFF0000; }
    else if (t.acu_emerg_cause == 4) { acu_emerg_str = "VCU_TO"; acu_emerg_color = 0xFF0000; }
    else if (t.acu_emerg_cause == 5) { acu_emerg_str = "JETSON_TO"; acu_emerg_color = 0xFF0000; }
    else if (t.acu_emerg_cause == 6) { acu_emerg_str = "WDT"; acu_emerg_color = 0xFF0000; }
    set_cell(table, 3, 2, "Emer Cause:", 0xFFFFFF);
    set_cell(table, 3, 3, acu_emerg_str, acu_emerg_color);

    // Row 4
    set_cell(table, 4, 0, "CPU Temp:", 0xFFFFFF);
    set_cell_fmt(table, 4, 1, 0xFFFFFF, "%3.0f C", t.acu_cpu_temp);
    set_cell(table, 4, 2, "", 0xFFFFFF);
    set_cell(table, 4, 3, "", 0xFFFFFF);

    // Row 5: Header Jetson
    set_cell(table, 5, 0, "JETSON STATUS", 0x00BFFF);
    lv_table_add_cell_ctrl(table, 5, 0, LV_TABLE_CELL_CTRL_MERGE_RIGHT);
    lv_table_add_cell_ctrl(table, 5, 1, LV_TABLE_CELL_CTRL_MERGE_RIGHT);
    lv_table_add_cell_ctrl(table, 5, 2, LV_TABLE_CELL_CTRL_MERGE_RIGHT);

    // Row 6
    const char *jet_as_state_str = "OFF";
    uint32_t jet_as_state_color = 0x808080;
    if (t.jetson_as_state == 2) { jet_as_state_str = "READY"; jet_as_state_color = 0x00FF00; }
    else if (t.jetson_as_state == 3) { jet_as_state_str = "DRIVING"; jet_as_state_color = 0x00FF00; }
    else if (t.jetson_as_state == 4) { jet_as_state_str = "EMERG"; jet_as_state_color = 0xFF0000; }
    else if (t.jetson_as_state == 5) { jet_as_state_str = "FINISH"; jet_as_state_color = 0x00FF00; }
    set_cell(table, 6, 0, "AS State:", 0xFFFFFF);
    set_cell(table, 6, 1, jet_as_state_str, jet_as_state_color);

    set_cell(table, 6, 2, "Mission:", 0xFFFFFF);
    set_cell_fmt(table, 6, 3, 0xFFFFFF, "%-25.0f", t.jetson_mission);

    // Row 7
    set_cell(table, 7, 0, "Temp:", 0xFFFFFF);
    set_cell_fmt(table, 7, 1, 0xFFFFFF, "%3.0f C", t.jetson_temp);
    set_cell(table, 7, 2, "CPU:", 0xFFFFFF);
    set_cell_fmt(table, 7, 3, 0xFFFFFF, "%3.0f%%", t.jetson_cpu);

    // Row 8
    set_cell(table, 8, 0, "GPU:", 0xFFFFFF);
    set_cell_fmt(table, 8, 1, 0xFFFFFF, "%3.0f%%", t.jetson_gpu);

    const char *jet_emerg_str = "NONE";
    uint32_t jet_emerg_color = 0x00FF00;
    if (t.jetson_emerg_cause == 1) { jet_emerg_str = "SDC_OPEN"; jet_emerg_color = 0xFF0000; }
    else if (t.jetson_emerg_cause == 2) { jet_emerg_str = "RES"; jet_emerg_color = 0xFF0000; }
    else if (t.jetson_emerg_cause == 3) { jet_emerg_str = "STEER_ERR"; jet_emerg_color = 0xFF0000; }
    set_cell(table, 8, 2, "Emer Cause:", 0xFFFFFF);
    set_cell(table, 8, 3, jet_emerg_str, jet_emerg_color);
}

void create_screen_debug_autonomous_2() {
    void *flowState = getFlowState(0, 8);
    (void)flowState;
    lv_obj_t *obj = lv_obj_create(0);
    objects.debug_autonomous_2 = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 800, 480);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        objects.autonomous_debug_text_2 = create_debug_table(obj, 6);
    }
    tick_screen_debug_autonomous_2();
}

void tick_screen_debug_autonomous_2() {
    TelemetryData t;
    if (!ros2subscriber_get_telemetry(&t)) {
        return;
    }
    lv_obj_t *table = objects.autonomous_debug_text_2;
    if (!table) return;

    // Header
    set_cell(table, 0, 0, "VCU CONTROL STATUS (2/5)", 0x00BFFF);
    lv_table_add_cell_ctrl(table, 0, 0, LV_TABLE_CELL_CTRL_MERGE_RIGHT);
    lv_table_add_cell_ctrl(table, 0, 1, LV_TABLE_CELL_CTRL_MERGE_RIGHT);
    lv_table_add_cell_ctrl(table, 0, 2, LV_TABLE_CELL_CTRL_MERGE_RIGHT);

    // Row 1
    set_cell(table, 1, 0, "IGN Man:", 0xFFFFFF);
    set_cell(table, 1, 1, (t.vcu_ign_man == 1) ? "ON" : "OFF", (t.vcu_ign_man == 1) ? 0x00FF00 : 0x808080);
    set_cell(table, 1, 2, "IGN Auto:", 0xFFFFFF);
    set_cell(table, 1, 3, (t.vcu_ign_auto == 1) ? "ON" : "OFF", (t.vcu_ign_auto == 1) ? 0x00FF00 : 0x808080);

    // Row 2
    set_cell(table, 2, 0, "R2D Man:", 0xFFFFFF);
    set_cell(table, 2, 1, (t.vcu_r2d_man == 1) ? "ON" : "OFF", (t.vcu_r2d_man == 1) ? 0x00FF00 : 0x808080);
    set_cell(table, 2, 2, "R2D Auto:", 0xFFFFFF);
    set_cell(table, 2, 3, (t.vcu_r2d_auto == 1) ? "ON" : "OFF", (t.vcu_r2d_auto == 1) ? 0x00FF00 : 0x808080);

    // Row 3
    set_cell(table, 3, 0, "Shutdown:", 0xFFFFFF);
    set_cell(table, 3, 1, (t.vcu_shutdown == 1) ? "CLOSED" : "OPEN", (t.vcu_shutdown == 1) ? 0x00FF00 : 0xFF0000);
    set_cell(table, 3, 2, "State:", 0xFFFFFF);
    set_cell_fmt(table, 3, 3, 0xFFFFFF, "%-25.0f", t.vcu_state);

    // Row 4
    set_cell(table, 4, 0, "HV:", 0xFFFFFF);
    set_cell(table, 4, 1, (t.vcu_hv == 1) ? "ACTIVE" : "OFF", (t.vcu_hv == 1) ? 0xFF0000 : 0x808080);
    set_cell(table, 4, 2, "", 0xFFFFFF);
    set_cell(table, 4, 3, "", 0xFFFFFF);

    // Row 5
    set_cell(table, 5, 0, "RPM Act:", 0xFFFFFF);
    set_cell_fmt(table, 5, 1, 0xFFFFFF, "%-5.0f", t.vcu_rpm_act);
    set_cell(table, 5, 2, "RPM Tgt:", 0xFFFFFF);
    set_cell_fmt(table, 5, 3, 0xFFFFFF, "%-5.0f", t.vcu_rpm_tgt);
}

void create_screen_debug_autonomous_3() {
    void *flowState = getFlowState(0, 9);
    (void)flowState;
    lv_obj_t *obj = lv_obj_create(0);
    objects.debug_autonomous_3 = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 800, 480);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        objects.autonomous_debug_text_3 = create_debug_table(obj, 11);
    }
    tick_screen_debug_autonomous_3();
}

void tick_screen_debug_autonomous_3() {
    TelemetryData t;
    if (!ros2subscriber_get_telemetry(&t)) {
        return;
    }
    lv_obj_t *table = objects.autonomous_debug_text_3;
    if (!table) return;

    // Header
    set_cell(table, 0, 0, "DV DYNAMICS & STATUS (3/5)", 0x00BFFF);
    lv_table_add_cell_ctrl(table, 0, 0, LV_TABLE_CELL_CTRL_MERGE_RIGHT);
    lv_table_add_cell_ctrl(table, 0, 1, LV_TABLE_CELL_CTRL_MERGE_RIGHT);
    lv_table_add_cell_ctrl(table, 0, 2, LV_TABLE_CELL_CTRL_MERGE_RIGHT);

    // Row 1
    set_cell(table, 1, 0, "Spd Act:", 0xFFFFFF);
    set_cell_fmt(table, 1, 1, 0xFFFFFF, "%3.1f km/h", t.dv_spd_act);
    set_cell(table, 1, 2, "Spd Tgt:", 0xFFFFFF);
    set_cell_fmt(table, 1, 3, 0xFFFFFF, "%3.1f km/h", t.dv_spd_tgt);

    // Row 2
    set_cell(table, 2, 0, "Str Act:", 0xFFFFFF);
    set_cell_fmt(table, 2, 1, 0xFFFFFF, "%5.1f", t.dv_str_act);
    set_cell(table, 2, 2, "Str Tgt:", 0xFFFFFF);
    set_cell_fmt(table, 2, 3, 0xFFFFFF, "%5.1f", t.dv_str_tgt);

    // Row 3
    set_cell(table, 3, 0, "Brk Act:", 0xFFFFFF);
    set_cell_fmt(table, 3, 1, 0xFFFFFF, "%3.0f%%", t.dv_brk_act);
    set_cell(table, 3, 2, "Brk Tgt:", 0xFFFFFF);
    set_cell_fmt(table, 3, 3, 0xFFFFFF, "%3.0f%%", t.dv_brk_tgt);

    // Row 4
    set_cell(table, 4, 0, "Mot Act:", 0xFFFFFF);
    set_cell_fmt(table, 4, 1, 0xFFFFFF, "%3.0f%%", t.dv_mot_act);
    set_cell(table, 4, 2, "Mot Tgt:", 0xFFFFFF);
    set_cell_fmt(table, 4, 3, 0xFFFFFF, "%3.0f%%", t.dv_mot_tgt);

    // Row 5
    set_cell(table, 5, 0, "Acc Lon:", 0xFFFFFF);
    set_cell_fmt(table, 5, 1, 0xFFFFFF, "%6.3f m/s2", t.dv_acc_lon);
    set_cell(table, 5, 2, "Acc Lat:", 0xFFFFFF);
    set_cell_fmt(table, 5, 3, 0xFFFFFF, "%6.3f m/s2", t.dv_acc_lat);

    // Row 6
    set_cell(table, 6, 0, "Yaw:", 0xFFFFFF);
    set_cell_fmt(table, 6, 1, 0xFFFFFF, "%6.3f /s", t.dv_yaw);
    set_cell(table, 6, 2, "", 0xFFFFFF);
    set_cell(table, 6, 3, "", 0xFFFFFF);

    // Row 7: Header Statuses
    set_cell(table, 7, 0, "STATUSES", 0x00BFFF);
    lv_table_add_cell_ctrl(table, 7, 0, LV_TABLE_CELL_CTRL_MERGE_RIGHT);
    lv_table_add_cell_ctrl(table, 7, 1, LV_TABLE_CELL_CTRL_MERGE_RIGHT);
    lv_table_add_cell_ctrl(table, 7, 2, LV_TABLE_CELL_CTRL_MERGE_RIGHT);

    // Row 8
    set_cell(table, 8, 0, "AS:", 0xFFFFFF);
    set_cell_fmt(table, 8, 1, 0xFFFFFF, "%3.0f", t.dv_as_status);
    set_cell(table, 8, 2, "EBS:", 0xFFFFFF);
    set_cell_fmt(table, 8, 3, 0xFFFFFF, "%3.0f", t.dv_ebs_state);

    // Row 9
    set_cell(table, 9, 0, "AMI:", 0xFFFFFF);
    set_cell_fmt(table, 9, 1, 0xFFFFFF, "%3.0f", t.dv_ami_state);
    set_cell(table, 9, 2, "Steer:", 0xFFFFFF);
    set_cell_fmt(table, 9, 3, 0xFFFFFF, "%3.0f", t.dv_steer_state);

    // Row 10
    set_cell(table, 10, 0, "EBS Red:", 0xFFFFFF);
    set_cell_fmt(table, 10, 1, 0xFFFFFF, "%3.0f", t.dv_ebs_red_state);
    set_cell(table, 10, 2, "", 0xFFFFFF);
    set_cell(table, 10, 3, "", 0xFFFFFF);
}

void create_screen_debug_autonomous_4() {
    void *flowState = getFlowState(0, 10);
    (void)flowState;
    lv_obj_t *obj = lv_obj_create(0);
    objects.debug_autonomous_4 = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 800, 480);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        objects.autonomous_debug_text_4 = create_debug_table(obj, 8);
    }
    tick_screen_debug_autonomous_4();
}

void tick_screen_debug_autonomous_4() {
    TelemetryData t;
    if (!ros2subscriber_get_telemetry(&t)) {
        return;
    }
    lv_obj_t *table = objects.autonomous_debug_text_4;
    if (!table) return;

    // Header
    set_cell(table, 0, 0, "PRESSURES & STEERING (4/5)", 0x00BFFF);
    lv_table_add_cell_ctrl(table, 0, 0, LV_TABLE_CELL_CTRL_MERGE_RIGHT);
    lv_table_add_cell_ctrl(table, 0, 1, LV_TABLE_CELL_CTRL_MERGE_RIGHT);
    lv_table_add_cell_ctrl(table, 0, 2, LV_TABLE_CELL_CTRL_MERGE_RIGHT);

    // Row 1
    uint32_t ebs_press_color = (t.ebs_tank_f > 10.0f) ? 0x00FF00 : 0xFF0000;
    set_cell(table, 1, 0, "EBS Tank F:", 0xFFFFFF);
    set_cell_fmt(table, 1, 1, ebs_press_color, "%5.1f bar", t.ebs_tank_f);
    set_cell(table, 1, 2, "EBS Tank R:", 0xFFFFFF);
    set_cell_fmt(table, 1, 3, ebs_press_color, "%5.1f bar", t.ebs_tank_r);

    // Row 2
    set_cell(table, 2, 0, "Brk Press F:", 0xFFFFFF);
    set_cell_fmt(table, 2, 1, 0xFFFFFF, "%5.1f bar", t.brk_press_f);
    set_cell(table, 2, 2, "Brk Press R:", 0xFFFFFF);
    set_cell_fmt(table, 2, 3, 0xFFFFFF, "%5.1f bar", t.brk_press_r);

    // Row 3: Header Steering Motor
    set_cell(table, 3, 0, "STEERING MOTOR", 0x00BFFF);
    lv_table_add_cell_ctrl(table, 3, 0, LV_TABLE_CELL_CTRL_MERGE_RIGHT);
    lv_table_add_cell_ctrl(table, 3, 1, LV_TABLE_CELL_CTRL_MERGE_RIGHT);
    lv_table_add_cell_ctrl(table, 3, 2, LV_TABLE_CELL_CTRL_MERGE_RIGHT);

    // Row 4
    set_cell(table, 4, 0, "Pos:", 0xFFFFFF);
    set_cell_fmt(table, 4, 1, 0xFFFFFF, "%5.1f", t.steer_pos);
    set_cell(table, 4, 2, "Spd:", 0xFFFFFF);
    set_cell_fmt(table, 4, 3, 0xFFFFFF, "%6.0f ERPM", t.steer_spd);

    // Row 5
    set_cell(table, 5, 0, "Curr:", 0xFFFFFF);
    set_cell_fmt(table, 5, 1, 0xFFFFFF, "%5.1f A", t.steer_curr);
    set_cell(table, 5, 2, "Drv T:", 0xFFFFFF);
    set_cell_fmt(table, 5, 3, 0xFFFFFF, "%3.0f C", t.steer_temp);

    // Row 6
    set_cell(table, 6, 0, "Err:", 0xFFFFFF);
    set_cell(table, 6, 1, (t.steer_err == 0) ? "OK" : "FAULT", (t.steer_err == 0) ? 0x00FF00 : 0xFF0000);
    set_cell(table, 6, 2, "Pos Target:", 0xFFFFFF);
    set_cell_fmt(table, 6, 3, 0xFFFFFF, "%5.1f", t.steer_pos_tgt);

    // Row 7
    set_cell(table, 7, 0, "Torque Tgt:", 0xFFFFFF);
    set_cell_fmt(table, 7, 1, 0xFFFFFF, "%5.2f Nm", t.vcu_torque_tgt);
    set_cell(table, 7, 2, "", 0xFFFFFF);
    set_cell(table, 7, 3, "", 0xFFFFFF);
}

void create_screen_debug_autonomous_5() {
    void *flowState = getFlowState(0, 11);
    (void)flowState;
    lv_obj_t *obj = lv_obj_create(0);
    objects.debug_autonomous_5 = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 800, 480);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        objects.autonomous_debug_text_5 = create_debug_table(obj, 10);
    }
    tick_screen_debug_autonomous_5();
}

void tick_screen_debug_autonomous_5() {
    TelemetryData t;
    if (!ros2subscriber_get_telemetry(&t)) {
        return;
    }
    lv_obj_t *table = objects.autonomous_debug_text_5;
    if (!table) return;

    // Header
    set_cell(table, 0, 0, "SLAM & ACQUISITION STATUS (5/5)", 0x00BFFF);
    lv_table_add_cell_ctrl(table, 0, 0, LV_TABLE_CELL_CTRL_MERGE_RIGHT);
    lv_table_add_cell_ctrl(table, 0, 1, LV_TABLE_CELL_CTRL_MERGE_RIGHT);
    lv_table_add_cell_ctrl(table, 0, 2, LV_TABLE_CELL_CTRL_MERGE_RIGHT);

    // Row 1
    set_cell(table, 1, 0, "Laps:", 0xFFFFFF);
    set_cell_fmt(table, 1, 1, 0xFFFFFF, "%2.0f", t.slam_laps);
    set_cell(table, 1, 2, "Cones:", 0xFFFFFF);
    set_cell_fmt(table, 1, 3, 0xFFFFFF, "%3.0f", t.slam_cones);

    // Row 2
    set_cell(table, 2, 0, "All Cones:", 0xFFFFFF);
    set_cell_fmt(table, 2, 1, 0xFFFFFF, "%4.0f", t.slam_all);
    set_cell(table, 2, 2, "RES Signal:", 0xFFFFFF);
    set_cell_fmt(table, 2, 3, 0xFFFFFF, "%5.1f", t.res_signal);

    // Row 3
    set_cell(table, 3, 0, "RES Status:", 0xFFFFFF);
    set_cell(table, 3, 1, (t.res_signal > 0.0f) ? "OK" : "ESTOP", (t.res_signal > 0.0f) ? 0x00FF00 : 0xFF0000);
    set_cell(table, 3, 2, "", 0xFFFFFF);
    set_cell(table, 3, 3, "", 0xFFFFFF);

    // Row 4: Header Acquisition Boards
    set_cell(table, 4, 0, "ACQUISITION BOARDS", 0x00BFFF);
    lv_table_add_cell_ctrl(table, 4, 0, LV_TABLE_CELL_CTRL_MERGE_RIGHT);
    lv_table_add_cell_ctrl(table, 4, 1, LV_TABLE_CELL_CTRL_MERGE_RIGHT);
    lv_table_add_cell_ctrl(table, 4, 2, LV_TABLE_CELL_CTRL_MERGE_RIGHT);

    // Row 5
    set_cell(table, 5, 0, "AQT1 BrkP:", 0xFFFFFF);
    set_cell_fmt(table, 5, 1, 0xFFFFFF, "%5.1f bar", t.aqt1_brkp);
    set_cell(table, 5, 2, "AQT1 RES:", 0xFFFFFF);
    set_cell(table, 5, 3, (t.aqt1_res == 1) ? "ACTIVE" : "OK", (t.aqt1_res == 1) ? 0xFF0000 : 0x00FF00);

    // Row 6
    set_cell(table, 6, 0, "AQT1 BOTS:", 0xFFFFFF);
    set_cell(table, 6, 1, (t.aqt1_bots == 1) ? "ACTIVE" : "OK", (t.aqt1_bots == 1) ? 0xFF0000 : 0x00FF00);
    set_cell(table, 6, 2, "AQT4 Inertia:", 0xFFFFFF);
    set_cell(table, 6, 3, (t.aqt4_inertia == 1) ? "ACTIVE" : "OK", (t.aqt4_inertia == 1) ? 0xFF0000 : 0x00FF00);

    // Row 7
    set_cell(table, 7, 0, "AQT4 Emer:", 0xFFFFFF);
    set_cell(table, 7, 1, (t.aqt4_emer == 1) ? "ACTIVE" : "OK", (t.aqt4_emer == 1) ? 0xFF0000 : 0x00FF00);
    set_cell(table, 7, 2, "AQT7 BrkP:", 0xFFFFFF);
    set_cell_fmt(table, 7, 3, 0xFFFFFF, "%5.1f bar", t.aqt7_brkp);

    // Row 8
    set_cell(table, 8, 0, "AQT2 WhlAng:", 0xFFFFFF);
    set_cell_fmt(table, 8, 1, 0xFFFFFF, "%5.1f", t.aqt2_whl_ang);
    set_cell(table, 8, 2, "AQT3 WhlAng:", 0xFFFFFF);
    set_cell_fmt(table, 8, 3, 0xFFFFFF, "%5.1f", t.aqt3_whl_ang);

    // Row 9
    set_cell(table, 9, 0, "AQT4 StAng:", 0xFFFFFF);
    set_cell_fmt(table, 9, 1, 0xFFFFFF, "%5.1f", t.aqt4_st_ang);
    set_cell(table, 9, 2, "", 0xFFFFFF);
    set_cell(table, 9, 3, "", 0xFFFFFF);
}

typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_driver_view,
    tick_screen_autonomous,
    tick_screen_debug_1,
    tick_screen_debug_inverter_2,
    tick_screen_debug_3,
    tick_screen_debug_wheels_4,
    tick_screen_debug_5,
    tick_screen_debug_autonomous_1,
    tick_screen_debug_autonomous_2,
    tick_screen_debug_autonomous_3,
    tick_screen_debug_autonomous_4,
    tick_screen_debug_autonomous_5,
};
void tick_screen(int screen_index) {
    if (screen_index >= 0 && screen_index < 12) {
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
    { "orbitron_25", &ui_font_orbitron_25 },
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
    create_screen_debug_1();
    create_screen_debug_inverter_2();
    create_screen_debug_3();
    create_screen_debug_wheels_4();
    create_screen_debug_5();
    create_screen_debug_autonomous_1();
    create_screen_debug_autonomous_2();
    create_screen_debug_autonomous_3();
    create_screen_debug_autonomous_4();
    create_screen_debug_autonomous_5();
}