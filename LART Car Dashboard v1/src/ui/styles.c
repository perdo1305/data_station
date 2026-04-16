#include "styles.h"
#include "images.h"
#include "fonts.h"

#include "ui.h"
#include "screens.h"

//
// Style: info_containers
//

void init_style_info_containers_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0xcc2b2b));
    lv_style_set_bg_opa(style, 255);
};

lv_style_t *get_style_info_containers_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_info_containers_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_info_containers(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_info_containers_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_info_containers(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_info_containers_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: text
//

void init_style_text_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_text_font(style, &ui_font_orbitron_bold_20);
    lv_style_set_text_align(style, LV_TEXT_ALIGN_CENTER);
    lv_style_set_text_color(style, lv_color_hex(0xfefefe));
};

lv_style_t *get_style_text_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_text_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_text(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_text_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_text(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_text_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: bar
//

void init_style_bar_INDICATOR_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0x02ff02));
    lv_style_set_line_width(style, 10);
    lv_style_set_radius(style, 0);
    lv_style_set_bg_opa(style, 255);
};

lv_style_t *get_style_bar_INDICATOR_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_bar_INDICATOR_DEFAULT(style);
    }
    return style;
};

void init_style_bar_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0x000000));
    lv_style_set_text_color(style, lv_color_hex(0x2196f3));
    lv_style_set_bg_opa(style, 255);
    lv_style_set_translate_y(style, 0);
    lv_style_set_radius(style, 10);
};

lv_style_t *get_style_bar_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_bar_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_bar(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_bar_INDICATOR_DEFAULT(), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_bar_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_bar(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_bar_INDICATOR_DEFAULT(), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_bar_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
//
//

void add_style(lv_obj_t *obj, int32_t styleIndex) {
    typedef void (*AddStyleFunc)(lv_obj_t *obj);
    static const AddStyleFunc add_style_funcs[] = {
        add_style_info_containers,
        add_style_text,
        add_style_bar,
    };
    add_style_funcs[styleIndex](obj);
}

void remove_style(lv_obj_t *obj, int32_t styleIndex) {
    typedef void (*RemoveStyleFunc)(lv_obj_t *obj);
    static const RemoveStyleFunc remove_style_funcs[] = {
        remove_style_info_containers,
        remove_style_text,
        remove_style_bar,
    };
    remove_style_funcs[styleIndex](obj);
}