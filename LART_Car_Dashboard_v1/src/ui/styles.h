#ifndef EEZ_LVGL_UI_STYLES_H
#define EEZ_LVGL_UI_STYLES_H

#include <lvgl/lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Style: info_containers
lv_style_t *get_style_info_containers_MAIN_DEFAULT();
void add_style_info_containers(lv_obj_t *obj);
void remove_style_info_containers(lv_obj_t *obj);

// Style: text
lv_style_t *get_style_text_MAIN_DEFAULT();
void add_style_text(lv_obj_t *obj);
void remove_style_text(lv_obj_t *obj);

// Style: bar
lv_style_t *get_style_bar_INDICATOR_DEFAULT();
lv_style_t *get_style_bar_MAIN_DEFAULT();
void add_style_bar(lv_obj_t *obj);
void remove_style_bar(lv_obj_t *obj);

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_STYLES_H*/