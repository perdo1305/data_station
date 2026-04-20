#ifndef EEZ_LVGL_UI_FONTS_H
#define EEZ_LVGL_UI_FONTS_H

#include <lvgl/lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern const lv_font_t ui_font_orbitron_bold_20;
extern const lv_font_t ui_font_orbiter_bold_100;
extern const lv_font_t ui_font_orbitron_bold_30;
extern const lv_font_t ui_font_orbitron_bold_50;
extern const lv_font_t ui_font_orbitron_bold_15;
extern const lv_font_t ui_font_orbitron_15;
extern const lv_font_t ui_font_orbitron_bold_25;
extern const lv_font_t ui_font_orbitron_bold_40;

#ifndef EXT_FONT_DESC_T
#define EXT_FONT_DESC_T
typedef struct _ext_font_desc_t {
    const char *name;
    const void *font_ptr;
} ext_font_desc_t;
#endif

extern ext_font_desc_t fonts[];

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_FONTS_H*/