"""Dashboard display application.

Cleaned MVP layout inspired by the old CTk dashboard:
- 12 shift lights at top
- central speed panel + key metrics
- vertical LV/HV SoC bars
- R2D status banner at bottom
- CAN activity indicator

All update_* and handle_* methods are thread-safe and non-blocking.
"""

import math
import threading
import time

try:
    import pygame

    _HAS_PYGAME = True
except ImportError:
    _HAS_PYGAME = False


SCREEN_W, SCREEN_H = 800, 480

_BG = (12, 14, 18)
_PANEL = (24, 27, 34)
_CARD = (34, 38, 47)
_TEXT = (232, 236, 242)
_TEXT_DIM = (168, 175, 190)
_CARD_BORDER = (58, 64, 77)
_OFF = (70, 74, 84)
_GREEN = (0, 255, 0)
_YELLOW = (255, 255, 0)
_RED = (255, 0, 0)
_CYAN = (0, 255, 255)
_CAN_GREEN = (30, 220, 50)
_CAN_GREEN_BRIGHT = (90, 255, 120)
_CAN_RED = (255, 50, 50)


def _clamp(value: float, low: float, high: float) -> float:
    return max(low, min(high, value))


def _safe_float(value, default=0.0) -> float:
    try:
        out = float(value)
    except (TypeError, ValueError):
        return default
    if not math.isfinite(out):
        return default
    return out


class DashboardApp:
    def __init__(self, rpm_shift: int = 11500, rpm_max: int = 12000):
        self._rpm_shift = float(rpm_shift)
        self._rpm_max = float(rpm_max)

        self._lock = threading.Lock()
        self._has_dashboard_state = False
        self._state = {
            'rpm': 0.0,
            'speed_kph': 0.0,
            'inv_temp_c': None,
            'motor_temp_c': None,
            'bps': None,
            'kw_inst': None,
            'kw_limit': None,
            'soc_lv': None,
            'soc_hv': None,
            'lv_voltage': None,
            'r2d_ready': False,
            'can_active': False,
        }
        self._display_state = dict(self._state)
        self._display_has_state = False

        # Limit UI value updates so numbers stay readable.
        self._display_update_period_s = 0.20
        self._last_display_sync = 0.0

        self._blink_state = False
        self._last_blink = 0.0
        self._last_can_rx = 0.0

    # ------------------------------------------------------------------
    # ROS callbacks (spin thread) — non-blocking
    # ------------------------------------------------------------------

    def update_rpm(self, rpm: float) -> None:
        value = max(0.0, _safe_float(rpm, 0.0))
        with self._lock:
            self._state['rpm'] = value
            if not self._has_dashboard_state:
                self._state['speed_kph'] = value * 0.02454

    def update_dashboard_state(self, state: dict) -> None:
        with self._lock:
            self._state.update(state)
            self._has_dashboard_state = True

    def update_can_frame(self, can_id: int, data: bytes) -> None:
        _ = (can_id, data)
        with self._lock:
            self._last_can_rx = time.time()

    def handle_button(self, button_id: int, pressed: bool) -> None:
        _ = (button_id, pressed)

    def handle_encoder(self, encoder_id: int, delta: int) -> None:
        _ = (encoder_id, delta)

    # ------------------------------------------------------------------
    # UI thread
    # ------------------------------------------------------------------

    def run(self) -> None:
        if not _HAS_PYGAME:
            self._headless_loop()
            return

        pygame.init()
        screen = pygame.display.set_mode((SCREEN_W, SCREEN_H))
        pygame.display.set_caption('LART Dashboard')
        pygame.mouse.set_visible(False)

        f_speed = pygame.font.SysFont('dejavusansmono', 124, bold=True)
        f_unit = pygame.font.SysFont('dejavusansmono', 30, bold=True)
        f_card_title = pygame.font.SysFont('dejavusansmono', 22, bold=True)
        f_card_value = pygame.font.SysFont('dejavusansmono', 46, bold=True)
        f_soc_label = pygame.font.SysFont('dejavusansmono', 32, bold=True)
        f_soc_value = pygame.font.SysFont('dejavusansmono', 24, bold=True)
        f_status = pygame.font.SysFont('dejavusansmono', 34, bold=True)

        clock = pygame.time.Clock()

        while True:
            for ev in pygame.event.get():
                if ev.type == pygame.QUIT:
                    return

            with self._lock:
                state = dict(self._state)
                has_state = self._has_dashboard_state
                last_can_rx = self._last_can_rx

            now = time.time()
            if now - self._last_display_sync >= self._display_update_period_s:
                self._display_state = state
                self._display_has_state = has_state
                self._last_display_sync = now

            shown_state = self._display_state
            shown_has_state = self._display_has_state

            if shown_has_state:
                can_active = bool(shown_state.get('can_active', False))
            else:
                can_active = (now - last_can_rx) < 1.0

            screen.fill(_BG)

            self._draw_shift_lights(screen, _safe_float(shown_state.get('rpm'), 0.0))
            self._draw_can_indicator(screen, can_active)
            self._draw_main_panel(
                screen,
                shown_state,
                shown_has_state,
                f_speed,
                f_unit,
                f_card_title,
                f_card_value,
            )
            self._draw_soc_bars(screen, shown_state, f_soc_label, f_soc_value)
            self._draw_r2d(screen, shown_state, shown_has_state, f_status)

            pygame.display.flip()
            clock.tick(30)

    # ------------------------------------------------------------------

    def _draw_shift_lights(self, screen, rpm: float) -> None:
        lights = 12
        ratio = _clamp(rpm / max(self._rpm_max, 1.0), 0.0, 1.0)
        lights_on = int(ratio * lights)
        for i in range(lights):
            x = 124 + i * 50
            y = 26
            color = _OFF
            if i < lights_on:
                if i < 3:
                    color = _GREEN
                elif i < 6:
                    color = _YELLOW
                elif i < 9:
                    color = _RED
                else:
                    color = _CYAN
            pygame.draw.circle(screen, color, (x, y), 12)

    def _draw_can_indicator(self, screen, can_active: bool) -> None:
        if can_active:
            now = time.time()
            if now - self._last_blink > 0.2:
                self._blink_state = not self._blink_state
                self._last_blink = now
            color = _CAN_GREEN_BRIGHT if self._blink_state else _CAN_GREEN
        else:
            color = _CAN_RED
        pygame.draw.circle(screen, color, (14, 14), 7)

    def _draw_main_panel(
        self,
        screen,
        state,
        has_state,
        f_speed,
        f_unit,
        f_card_title,
        f_card_value,
    ) -> None:
        panel = pygame.Rect(100, 56, 600, 352)
        pygame.draw.rect(screen, _PANEL, panel, border_radius=16)

        # Top cards
        self._draw_card(
            screen,
            pygame.Rect(112, 72, 176, 92),
            'Temp INV',
            self._fmt_number(state.get('inv_temp_c'), suffix=' C'),
            f_card_title,
            f_card_value,
        )
        self._draw_card(
            screen,
            pygame.Rect(312, 72, 176, 92),
            'Temp Motor',
            self._fmt_number(state.get('motor_temp_c'), suffix=' C'),
            f_card_title,
            f_card_value,
        )
        self._draw_card(
            screen,
            pygame.Rect(512, 72, 176, 92),
            'BPS',
            self._fmt_number(state.get('bps'), suffix=' %'),
            f_card_title,
            f_card_value,
        )

        # Center speed
        speed = _safe_float(state.get('speed_kph'), 0.0)
        speed_text = str(int(round(speed))) if has_state else 'ERR'
        speed_surf = f_speed.render(speed_text, True, _TEXT)
        screen.blit(speed_surf, (400 - speed_surf.get_width() // 2, 172))
        unit_surf = f_unit.render('Km/h', True, _TEXT_DIM)
        screen.blit(unit_surf, (526, 270))

        # Bottom cards
        self._draw_card(
            screen,
            pygame.Rect(112, 318, 286, 78),
            'kW Inst',
            self._fmt_number(state.get('kw_inst'), decimals=2),
            f_card_title,
            f_card_value,
        )
        self._draw_card(
            screen,
            pygame.Rect(402, 318, 286, 78),
            'kW Limit',
            self._fmt_number(state.get('kw_limit'), decimals=1),
            f_card_title,
            f_card_value,
        )

    def _draw_card(self, screen, rect, title, value, f_title, f_value) -> None:
        pygame.draw.rect(screen, _CARD, rect, border_radius=12)
        pygame.draw.rect(screen, _CARD_BORDER, rect, 2, border_radius=12)
        title_surf = f_title.render(title, True, _TEXT_DIM)
        value_surf = f_value.render(value, True, _TEXT)
        screen.blit(title_surf, (rect.x + 12, rect.y + 8))
        screen.blit(value_surf, (rect.x + 12, rect.y + 34))

    def _draw_soc_bars(self, screen, state, f_label, f_value) -> None:
        self._draw_soc_bar(
            screen,
            x=20,
            y=80,
            width=60,
            height=320,
            label='LV',
            value=state.get('soc_lv'),
            sublabel=self._format_lv_label(state.get('lv_voltage'), state.get('soc_lv')),
            f_label=f_label,
            f_value=f_value,
        )
        self._draw_soc_bar(
            screen,
            x=720,
            y=80,
            width=60,
            height=320,
            label='HV',
            value=state.get('soc_hv'),
            sublabel=self._format_percent(state.get('soc_hv')),
            f_label=f_label,
            f_value=f_value,
        )

    def _draw_soc_bar(
        self, screen, x, y, width, height, label, value, sublabel, f_label, f_value
    ) -> None:
        ratio = _clamp(_safe_float(value, 0.0) / 100.0, 0.0, 1.0)
        fill_h = int(height * ratio)

        bar_rect = pygame.Rect(x, y, width, height)
        fill_rect = pygame.Rect(x, y + (height - fill_h), width, fill_h)

        pygame.draw.rect(screen, _CARD, bar_rect, border_radius=6)
        if fill_h > 0:
            color = _YELLOW if ratio > 0.2 else _RED
            pygame.draw.rect(screen, color, fill_rect, border_radius=6)
        pygame.draw.rect(screen, _CARD_BORDER, bar_rect, 2, border_radius=6)

        label_surf = f_label.render(label, True, _TEXT)
        screen.blit(label_surf, (x + width // 2 - label_surf.get_width() // 2, y - 38))
        value_surf = f_value.render(sublabel, True, _TEXT)
        screen.blit(value_surf, (x + width // 2 - value_surf.get_width() // 2, y + height + 10))

    def _draw_r2d(self, screen, state, has_state, f_status) -> None:
        if not has_state:
            txt = 'R2D STATE UNKNOWN'
            color = (255, 0, 217)
        elif bool(state.get('r2d_ready', False)):
            txt = 'READY'
            color = _GREEN
        else:
            txt = 'NOT READY'
            color = _RED

        surf = f_status.render(txt, True, color)
        screen.blit(surf, (SCREEN_W // 2 - surf.get_width() // 2, SCREEN_H - 46))

    # ------------------------------------------------------------------

    def _fmt_number(self, value, suffix='', decimals=1) -> str:
        if value is None:
            return 'ERR'
        num = _safe_float(value, float('nan'))
        if not math.isfinite(num):
            return 'ERR'
        if decimals <= 0:
            return f'{int(round(num))}{suffix}'
        if abs(num) >= 100:
            return f'{int(round(num))}{suffix}'
        return f'{num:.{decimals}f}{suffix}'

    def _format_percent(self, value) -> str:
        if value is None:
            return 'ERR'
        num = _safe_float(value, float('nan'))
        if not math.isfinite(num):
            return 'ERR'
        return f'{int(round(_clamp(num, 0.0, 100.0)))}%'

    def _format_lv_label(self, voltage, soc) -> str:
        voltage_val = _safe_float(voltage, float('nan'))
        if math.isfinite(voltage_val):
            return f'{voltage_val:.1f}V'
        return self._format_percent(soc)

    def _headless_loop(self) -> None:
        import logging

        logging.getLogger(__name__).warning(
            'pygame not found — UI running headless. Install: pip install pygame'
        )
        while True:
            time.sleep(1.0)
