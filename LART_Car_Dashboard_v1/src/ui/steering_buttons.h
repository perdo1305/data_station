#ifndef LART_STEERING_BUTTONS_H
#define LART_STEERING_BUTTONS_H

// Polls one steering wheel button wired to a Raspberry Pi GPIO line.
// Wiring matches src/lart_bringup/config/rpi_config.yaml: pull-up enabled,
// switch to GND, so pressed = LOW. Uses the kernel GPIO character device
// directly, so no extra library is needed.
//
// Call poll_pressed() from the LVGL thread once per frame. It makes no LVGL calls.
class SteeringButton {
public:
    SteeringButton() = default;
    ~SteeringButton();

    SteeringButton(const SteeringButton &) = delete;
    SteeringButton &operator=(const SteeringButton &) = delete;

    // False when the line cannot be requested, for example when the GPIO
    // chip is missing or another process (input_handler) already owns it.
    bool open(unsigned int line_offset);

    // True once per press, after the level has stayed LOW for the debounce time.
    bool poll_pressed();

private:
    bool read_level(bool &level) const;

    int fd_ = -1;
    bool stable_ = true;     // last debounced level, true = released
    bool candidate_ = true;  // newest raw level
    long long candidate_since_ms_ = 0;
};

#endif // LART_STEERING_BUTTONS_H
