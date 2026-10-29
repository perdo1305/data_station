#ifndef LART_STEERING_BUTTONS_H
#define LART_STEERING_BUTTONS_H

// Polls one steering wheel button wired to a Raspberry Pi GPIO line.
// Uses the kernel GPIO character device directly, so no extra library is needed.
//
// Polarity is chosen in open(): an active-high button switches the pin to
// 3.3 V and gets a pull-down; an active-low one switches to GND and gets a
// pull-up. The current steering wheel switches to 3.3 V.
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
    bool open(unsigned int line_offset, bool active_high);

    // True once per press, after the button has stayed pressed for the debounce time.
    bool poll_pressed();

private:
    bool read_pressed(bool &pressed) const;

    int fd_ = -1;
    bool active_high_ = false;
    bool stable_ = false;     // last debounced state, true = pressed
    bool candidate_ = false;  // newest raw state
    long long candidate_since_ms_ = 0;
};

#endif // LART_STEERING_BUTTONS_H
