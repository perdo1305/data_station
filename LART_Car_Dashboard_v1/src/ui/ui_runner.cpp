#include "ui.h"
#include "screens.h"
#include "vars.h"
#include "ros2subscriber.h"
#include <cmath>
#include <cstring>
#include <cassert>

#include <SDL2/SDL.h>

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <vector>

extern "C" void ui_fini(void);

namespace {
constexpr int kUiWidth = 800;
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

uint32_t color_to_argb(lv_color_t color) {
    return 0xFF000000u
        | (static_cast<uint32_t>(color.red) << 16)
        | (static_cast<uint32_t>(color.green) << 8)
        | static_cast<uint32_t>(color.blue);
}

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

}  // namespace

int main(int argc, char **argv) {
    if (std::getenv("LART_TEST_MAPPINGS") != nullptr) {
        init_lvgl();
        ui_init();
        
        std::printf("[TEST] Running telemetry mapping unit tests...\n");

        TelemetryData t = {};
        t.brk_press_f = 45.2f;
        t.brk_press_r = 10.0f;
        t.apps1 = 80.0f;
        t.apps2 = 78.0f;
        t.ams_soc = 92.5f;
        t.ivt_u3 = 24300.0f; // 24.3V
        t.vcu_r2d_man = 1.0f; // Ready
        t.dv_spd_act = 55.4f;
        t.inv_temp_ctrl = 38.2f;
        t.inv_temp_mot = 62.1f;
        t.slam_laps = 3.0f;
        t.acu_mission = 4.0f; // Endurance

        ui_update_telemetry_vars(&t);

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
        assert(std::strcmp(val_mission.getString(), "ENDURANCE") == 0);

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
    ui_init();
    ui_set_speed(initial_speed);

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

        lv_tick_inc(elapsed_ms);
        ui_tick();
        lv_timer_handler();
        present_frame();

        SDL_Delay(kFrameDelayMs);
    }

    ui_fini();
    destroy_window();
    SDL_Quit();
    return 0;
}