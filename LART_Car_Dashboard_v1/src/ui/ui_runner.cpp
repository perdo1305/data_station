#include "ui.h"
#include "screens.h"
#include "vars.h"
#include "ros2subscriber.h"
#include "dbc_api.h"
#include <cmath>
#include <cstring>
#include <cassert>

#include <SDL2/SDL.h>

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <map>
#include <string>

extern "C" void ui_fini(void);

LV_FONT_DECLARE(ui_font_orbitron_15);
LV_FONT_DECLARE(ui_font_orbitron_bold_15);
LV_FONT_DECLARE(ui_font_orbitron_bold_20);
LV_FONT_DECLARE(ui_font_orbitron_25);

namespace {
constexpr int kUiWidth = 800;
// ... (rest of the code remains unchanged up to line 225)

constexpr int kUiHeight = 480;
constexpr int kBufferLines = 40;
constexpr int kFrameDelayMs = 5;

SDL_Window *g_window = nullptr;
SDL_Renderer *g_renderer = nullptr;
SDL_Texture *g_texture = nullptr;

std::vector<uint32_t> g_framebuffer(kUiWidth * kUiHeight, 0xFF000000u);
#if LVGL_VERSION_MAJOR >= 9
std::vector<uint32_t> g_draw_buffer_a(kUiWidth * kBufferLines);
std::vector<uint32_t> g_draw_buffer_b(kUiWidth * kBufferLines);
#else
std::vector<lv_color_t> g_draw_buffer_a(kUiWidth * kBufferLines);
std::vector<lv_color_t> g_draw_buffer_b(kUiWidth * kBufferLines);
#endif

bool g_framebuffer_dirty = false;
bool g_running = true;

#if LVGL_VERSION_MAJOR >= 9
lv_display_t *g_display = nullptr;
#endif

void present_frame() {
    if (!g_framebuffer_dirty) {
        return;
    }

    if (SDL_UpdateTexture(g_texture, nullptr, g_framebuffer.data(), kUiWidth * static_cast<int>(sizeof(uint32_t))) != 0) {
        std::fprintf(stderr, "SDL_UpdateTexture failed: %s\n", SDL_GetError());
        g_running = false;
        return;
    }

    SDL_RenderClear(g_renderer);
    SDL_RenderCopy(g_renderer, g_texture, nullptr, nullptr);
    SDL_RenderPresent(g_renderer);
    g_framebuffer_dirty = false;
}

#if LVGL_VERSION_MAJOR >= 9
void flush_cb(lv_display_t *display, const lv_area_t *area, uint8_t *px_map) {
    const uint32_t *color_p = reinterpret_cast<const uint32_t *>(px_map);
#else
void flush_cb(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p) {
#endif
    const int area_width = area->x2 - area->x1 + 1;
    const int area_height = area->y2 - area->y1 + 1;

    for (int y = 0; y < area_height; ++y) {
        const int framebuffer_y = area->y1 + y;
        if (framebuffer_y < 0 || framebuffer_y >= kUiHeight) {
            continue;
        }

        const int source_x_offset = std::max(0, -area->x1);
        const int framebuffer_x = std::max(area->x1, 0);
        const int copy_width = std::min(area_width - source_x_offset, kUiWidth - framebuffer_x);
        if (copy_width <= 0) {
            continue;
        }

        uint32_t *destination = &g_framebuffer[framebuffer_y * kUiWidth + framebuffer_x];
        const auto *source = color_p + (y * area_width) + source_x_offset;

        for (int x = 0; x < copy_width; ++x) {
            /* LVGL v9 uses LV_COLOR_FORMAT_NATIVE (XRGB8888 when LV_COLOR_DEPTH=32) by default.
             * Ensure alpha is fully opaque for the SDL ARGB8888 texture.
             */
            destination[x] = 0xFF000000u | (source[x] & 0x00FFFFFFu);
        }
    }

    g_framebuffer_dirty = true;

#if LVGL_VERSION_MAJOR >= 9
    lv_display_flush_ready(display);
#else
    lv_disp_flush_ready(disp_drv);
#endif
}

#if LVGL_VERSION_MAJOR >= 9
void pointer_read_cb(lv_indev_t *, lv_indev_data_t *data) {
#else
void pointer_read_cb(lv_indev_drv_t *, lv_indev_data_t *data) {
#endif
    int mouse_x = 0;
    int mouse_y = 0;
    const uint32_t mouse_buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

    data->state = (mouse_buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
    data->point.x = mouse_x;
    data->point.y = mouse_y;
}

bool create_window() {
    const uint32_t window_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP;

    g_window = SDL_CreateWindow(
        "LART UI Runner",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        kUiWidth,
        kUiHeight,
        window_flags
    );

    if (g_window == nullptr) {
        std::fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        return false;
    }

    g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (g_renderer == nullptr) {
        std::fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
        return false;
    }

    g_texture = SDL_CreateTexture(
        g_renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        kUiWidth,
        kUiHeight
    );

    if (g_texture == nullptr) {
        std::fprintf(stderr, "SDL_CreateTexture failed: %s\n", SDL_GetError());
        return false;
    }

    SDL_ShowCursor(0);

    return true;
}

void destroy_window() {
    if (g_texture != nullptr) {
        SDL_DestroyTexture(g_texture);
        g_texture = nullptr;
    }

    if (g_renderer != nullptr) {
        SDL_DestroyRenderer(g_renderer);
        g_renderer = nullptr;
    }

    if (g_window != nullptr) {
        SDL_DestroyWindow(g_window);
        g_window = nullptr;
    }
}

void init_lvgl() {
    lv_init();

#if LVGL_VERSION_MAJOR >= 9
    g_display = lv_display_create(kUiWidth, kUiHeight);
    lv_display_set_color_format(g_display, LV_COLOR_FORMAT_XRGB8888);
    lv_display_set_flush_cb(g_display, flush_cb);
    lv_display_set_buffers(
        g_display,
        g_draw_buffer_a.data(),
        g_draw_buffer_b.data(),
        static_cast<uint32_t>(g_draw_buffer_a.size() * sizeof(g_draw_buffer_a[0])),
        LV_DISPLAY_RENDER_MODE_PARTIAL
    );
    lv_display_set_default(g_display);

    lv_indev_t *indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, pointer_read_cb);
#else
    static lv_disp_draw_buf_t draw_buf;
    static lv_disp_drv_t disp_drv;

    lv_disp_draw_buf_init(
        &draw_buf,
        g_draw_buffer_a.data(),
        g_draw_buffer_b.data(),
        static_cast<uint32_t>(g_draw_buffer_a.size())
    );

    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = kUiWidth;
    disp_drv.ver_res = kUiHeight;
    disp_drv.flush_cb = flush_cb;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = pointer_read_cb;
    lv_indev_drv_register(&indev_drv);
#endif
}

struct LvNotif {
    std::string id;
    std::string title;
    std::string message;
    lv_obj_t *container;
    lv_obj_t *title_label;
    lv_obj_t *msg_label;
    uint32_t created_ms;
};

static std::vector<LvNotif> g_lv_notifications;
static std::map<std::string, bool> g_last_error_state;

constexpr int kNotifWidth = 650;
constexpr int kNotifHeight = 80;
constexpr int kNotifSpacing = 8;
// Start the stack below the temperature containers (driver view temp row ends at y≈142)
// so notifications never cover the INV / Max / Motor temp readouts.
constexpr int kNotifStartY = 145;
// 3 × (80 + 8) starting at y=145 keeps the whole stack on the 480px screen.
constexpr int kNotifMaxVisible = 3;

void reposition_notifications() {
    for (size_t i = 0; i < g_lv_notifications.size(); ++i) {
        if (i < kNotifMaxVisible) {
            lv_obj_clear_flag(g_lv_notifications[i].container, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_pos(g_lv_notifications[i].container, (kUiWidth - kNotifWidth) / 2, kNotifStartY + i * (kNotifHeight + kNotifSpacing));
        } else {
            lv_obj_add_flag(g_lv_notifications[i].container, LV_OBJ_FLAG_HIDDEN);
        }
    }
}

static void notif_click_cb(lv_event_t *e) {
    lv_obj_t *obj = static_cast<lv_obj_t*>(lv_event_get_current_target(e));
    for (auto it = g_lv_notifications.begin(); it != g_lv_notifications.end(); ++it) {
        if (it->container == obj) {
            ros2subscriber_publish_ack(it->id.c_str());
            lv_obj_del(it->container);
            g_lv_notifications.erase(it);
            reposition_notifications();
            break;
        }
    }
}

extern "C" void ui_add_notification(const char *id, const char *title, const char *message) {
    for (auto &n : g_lv_notifications) {
        if (n.id == id) {
            n.title = title;
            n.message = message;
            n.created_ms = lv_tick_get();
            lv_label_set_text(n.title_label, title);
            lv_label_set_text(n.msg_label, message);
            return;
        }
    }
    
    lv_obj_t *sys_layer = lv_layer_sys();
    if (!sys_layer) return;
    
    lv_obj_t *container = lv_obj_create(sys_layer);
    lv_obj_set_size(container, kNotifWidth, kNotifHeight);
    
    // Premium theme styling matching Formula Student
    lv_obj_set_style_bg_color(container, lv_color_hex(0x24080c), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(container, 230, LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_set_style_border_color(container, lv_color_hex(0xff3b4d), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(container, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(container, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(container, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_set_style_pad_all(container, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(container, LV_OBJ_FLAG_SCROLLABLE);
    
    // Warning accent line
    lv_obj_t *accent = lv_obj_create(container);
    lv_obj_set_size(accent, 6, kNotifHeight - 20);
    lv_obj_align(accent, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_style_bg_color(accent, lv_color_hex(0xff3b4d), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(accent, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(accent, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(accent, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    
    // Title Label
    lv_obj_t *title_lbl = lv_label_create(container);
    lv_label_set_text(title_lbl, title);
    lv_obj_align(title_lbl, LV_ALIGN_TOP_LEFT, 20, 6);
    lv_obj_set_style_text_color(title_lbl, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(title_lbl, &ui_font_orbitron_bold_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    
    // Message Label
    lv_obj_t *msg_lbl = lv_label_create(container);
    lv_label_set_text(msg_lbl, message);
    lv_obj_align(msg_lbl, LV_ALIGN_BOTTOM_LEFT, 20, -6);
    lv_obj_set_style_text_color(msg_lbl, lv_color_hex(0xffb5bd), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(msg_lbl, &ui_font_orbitron_25, LV_PART_MAIN | LV_STATE_DEFAULT);
    
    // "TAP TO ACK" helper text
    lv_obj_t *ack_lbl = lv_label_create(container);
    lv_label_set_text(ack_lbl, "TAP TO ACK");
    lv_obj_align(ack_lbl, LV_ALIGN_RIGHT_MID, -20, 0);
    lv_obj_set_style_text_color(ack_lbl, lv_color_hex(0x8f585d), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ack_lbl, &ui_font_orbitron_bold_15, LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_add_event_cb(container, notif_click_cb, LV_EVENT_CLICKED, nullptr);
    
    g_lv_notifications.push_back({id, title, message, container, title_lbl, msg_lbl, lv_tick_get()});
    reposition_notifications();
}

extern "C" void ui_clear_notification(const char *id) {
    std::string id_str = id;
    for (auto it = g_lv_notifications.begin(); it != g_lv_notifications.end(); ) {
        if (it->id == id_str || it->title == id_str || it->message.find(id_str) != std::string::npos) {
            lv_obj_del(it->container);
            it = g_lv_notifications.erase(it);
        } else {
            ++it;
        }
    }
    reposition_notifications();
}

extern "C" void ui_clear_all_notifications() {
    for (const auto &n : g_lv_notifications) {
        lv_obj_del(n.container);
    }
    g_lv_notifications.clear();
}

void check_expired_notifications() {
    uint32_t now = lv_tick_get();
    bool changed = false;
    for (auto it = g_lv_notifications.begin(); it != g_lv_notifications.end(); ) {
        if (now - it->created_ms >= 10000) {
            lv_obj_del(it->container);
            it = g_lv_notifications.erase(it);
            changed = true;
        } else {
            ++it;
        }
    }
    if (changed) {
        reposition_notifications();
    }
}

}  // namespace

int main(int argc, char **argv) {
    if (std::getenv("LART_TEST_MAPPINGS") != nullptr) {
        init_lvgl();

        // Override EEZ-Flow hooks to prevent crashing on flow completion/errors,
        // and provide logging for any flow errors.
        eez::flow::stopScriptHook = []() {
            // No-op
        };
        eez::flow::onFlowErrorHook = [](eez::flow::FlowState *, int componentIndex, const char *errorMessage) {
            std::fprintf(stderr, "[EEZ-Flow Error] Component %d: %s\n", componentIndex, errorMessage);
        };

        ui_init();
        
        std::printf("[TEST] Running telemetry mapping unit tests...\n");

        // Test 1: Setting dbc_api directly and calling with NULL
        dbc_api.asf_signals.brake_pressure_front = 45.2f;
        dbc_api.asf_signals.brake_pressure_rear = 10.0f;
        dbc_api.inv1_misc.inv1_actual_throttle = 80.0f;
        dbc_api.master_soc_accumulator.soc_float = 92.5f;
        dbc_api.ivt_msg_result_u3.ivt_result_u3 = 24300.0f;
        dbc_api.vcu_ign_r2d.r2d_manual = 1.0f;
        dbc_api.dv_dynamics_1.speed_actual = 55.4f;
        dbc_api.inv1_temperatures.inv1_actual_tempcontroller = 38.2f;
        dbc_api.inv1_temperatures.inv1_actual_tempmotor = 62.1f;
        dbc_api.slam_stats_can.lap_counter = 3.0f;
        dbc_api.acu.mission_select = 4.0f;

        ui_update_telemetry_vars(nullptr);

        // Assertions
        auto val_brake = eez::flow::getGlobalVariable(FLOW_GLOBAL_VARIABLE_BRAKE_PEDAL_PRESSURE);
        assert(val_brake.getInt() == 45);

        auto val_acc = eez::flow::getGlobalVariable(FLOW_GLOBAL_VARIABLE_ACCELL_PEDAL_PRESSURE);
        assert(val_acc.getInt() == 80);

        auto val_soc = eez::flow::getGlobalVariable(FLOW_GLOBAL_VARIABLE_SOC);
        assert(val_soc.getInt() == 92);

        auto val_lv = eez::flow::getGlobalVariable(FLOW_GLOBAL_VARIABLE_LV);
        assert(std::abs(val_lv.getFloat() - 24.3f) < 0.01f);

        auto val_ready = eez::flow::getGlobalVariable(FLOW_GLOBAL_VARIABLE_READY);
        assert(std::strcmp(val_ready.getString(), "READY") == 0);

        auto val_speed = eez::flow::getGlobalVariable(FLOW_GLOBAL_VARIABLE_SPEED);
        assert(std::abs(val_speed.getFloat() - 55.4f) < 0.01f);

        auto val_temp_inv = eez::flow::getGlobalVariable(FLOW_GLOBAL_VARIABLE_TEMP_INV);
        assert(std::abs(val_temp_inv.getFloat() - 38.2f) < 0.01f);

        auto val_temp_mot = eez::flow::getGlobalVariable(FLOW_GLOBAL_VARIABLE_TEMP_MOTOR);
        assert(std::abs(val_temp_mot.getFloat() - 62.1f) < 0.01f);

        auto val_laps = eez::flow::getGlobalVariable(FLOW_GLOBAL_VARIABLE_LAP_COUNT);
        assert(val_laps.getInt() == 3);

        auto val_mission = eez::flow::getGlobalVariable(FLOW_GLOBAL_VARIABLE_MISSION);
        assert(std::strcmp(val_mission.getString(), "BRAKE TEST") == 0);

        // Test 2: Backwards compatibility (passing TelemetryData)
        TelemetryData t = {};
        t.brk_press_f = 20.0f;
        t.apps1 = 50.0f;
        t.ams_soc = 80.0f;
        t.ivt_u3 = 25000.0f;
        t.vcu_r2d_man = 0.0f;
        t.rear_r2d = 1.0f; // Ready
        t.dv_spd_act = 10.0f;
        t.inv_temp_ctrl = 30.0f;
        t.inv_temp_mot = 40.0f;
        t.slam_laps = 5.0f;
        t.acu_mission = 1.0f; // ACCEL

        ui_update_telemetry_vars(&t);

        assert(eez::flow::getGlobalVariable(FLOW_GLOBAL_VARIABLE_BRAKE_PEDAL_PRESSURE).getInt() == 20);
        assert(eez::flow::getGlobalVariable(FLOW_GLOBAL_VARIABLE_ACCELL_PEDAL_PRESSURE).getInt() == 50);
        assert(eez::flow::getGlobalVariable(FLOW_GLOBAL_VARIABLE_SOC).getInt() == 80);
        assert(std::abs(eez::flow::getGlobalVariable(FLOW_GLOBAL_VARIABLE_LV).getFloat() - 25.0f) < 0.01f);
        assert(std::strcmp(eez::flow::getGlobalVariable(FLOW_GLOBAL_VARIABLE_READY).getString(), "READY") == 0);
        assert(std::abs(eez::flow::getGlobalVariable(FLOW_GLOBAL_VARIABLE_SPEED).getFloat() - 10.0f) < 0.01f);
        assert(std::abs(eez::flow::getGlobalVariable(FLOW_GLOBAL_VARIABLE_TEMP_INV).getFloat() - 30.0f) < 0.01f);
        assert(std::abs(eez::flow::getGlobalVariable(FLOW_GLOBAL_VARIABLE_TEMP_MOTOR).getFloat() - 40.0f) < 0.01f);
        assert(eez::flow::getGlobalVariable(FLOW_GLOBAL_VARIABLE_LAP_COUNT).getInt() == 5);
        assert(std::strcmp(eez::flow::getGlobalVariable(FLOW_GLOBAL_VARIABLE_MISSION).getString(), "ACCEL") == 0);

        // Test 3: Test emergency screen transition on as_state = 4
        // The default screen after ui_init is SCREEN_ID_DRIVER_VIEW (1)
        assert(eez_flow_get_current_screen() == SCREEN_ID_DRIVER_VIEW);

        dbc_api.acu.as_state = 4.0f;
        ui_tick(); // Tick once to detect the state change and queue the screen change
        ui_tick(); // Tick twice to apply the screen change

        assert(eez_flow_get_current_screen() == SCREEN_ID_DEBUG_AUTONOMOUS_1);

        std::printf("[TEST] ✓ All telemetry mapping unit tests passed successfully!\n");
        std::_Exit(0);
    }

    float initial_speed = 0.0f;
    if (argc > 1) {
        initial_speed = static_cast<float>(std::strtod(argv[1], nullptr));
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    if (!create_window()) {
        destroy_window();
        SDL_Quit();
        return 1;
    }

    init_lvgl();

    // Override EEZ-Flow hooks to prevent crashing on flow completion/errors,
    // and provide logging for any flow errors.
    eez::flow::stopScriptHook = []() {
        // No-op
    };
    eez::flow::onFlowErrorHook = [](eez::flow::FlowState *, int componentIndex, const char *errorMessage) {
        std::fprintf(stderr, "[EEZ-Flow Error] Component %d: %s\n", componentIndex, errorMessage);
    };

    ui_init();
    ui_set_speed(initial_speed);
    ros2subscriber_init();

    uint32_t last_tick_ms = SDL_GetTicks();

    while (g_running) {
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                g_running = false;
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                g_running = false;
            }
        }

        const uint32_t now_ms = SDL_GetTicks();
        const uint32_t elapsed_ms = now_ms - last_tick_ms;
        last_tick_ms = now_ms;

        ros2subscriber_spin_some();
        lv_tick_inc(elapsed_ms);
        ui_tick();

        check_expired_notifications();

        // Check for DBC errors
        static std::map<std::string, bool> current_errors;
        current_errors.clear();
        
        check_dbc_errors([](const char *id, const char *msg_name, const char *sig_name, float value, const char *choice_label) {
            (void)value;
            std::string id_str = id;
            current_errors[id_str] = true;
            if (!g_last_error_state[id_str]) {
                g_last_error_state[id_str] = true;
                char title_buf[128];
                std::snprintf(title_buf, sizeof(title_buf), "%s Fault", msg_name);
                char msg_buf[256];
                if (std::strcmp(choice_label, "ERROR") == 0) {
                    std::snprintf(msg_buf, sizeof(msg_buf), "%s: ACTIVE", sig_name);
                } else {
                    std::snprintf(msg_buf, sizeof(msg_buf), "%s", choice_label);
                }
                ui_add_notification(id, title_buf, msg_buf);
            }
        });
        
        for (auto &pair : g_last_error_state) {
            if (pair.second && !current_errors[pair.first]) {
                pair.second = false;
            }
        }

        lv_timer_handler();
        present_frame();

        SDL_Delay(kFrameDelayMs);
    }

    ros2subscriber_fini();
    ui_fini();
    destroy_window();
    SDL_Quit();
    return 0;
}