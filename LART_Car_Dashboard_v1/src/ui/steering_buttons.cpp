#include "steering_buttons.h"

#include <fcntl.h>
#include <linux/gpio.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <cerrno>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <initializer_list>

namespace {
constexpr long long kDebounceMs = 50; // matches debounce_ms in rpi_config.yaml

long long now_ms() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

// Same chip choice as input_handler: the RPi 5 header lives on gpiochip4.
const char *find_chip() {
    for (const char *path : {"/dev/gpiochip4", "/dev/gpiochip0"}) {
        if (access(path, F_OK) == 0) {
            return path;
        }
    }
    return nullptr;
}
} // namespace

SteeringButton::~SteeringButton() {
    if (fd_ >= 0) {
        close(fd_);
    }
}

bool SteeringButton::open(unsigned int line_offset, bool active_high) {
    const char *chip_path = find_chip();
    if (!chip_path) {
        std::fprintf(stderr, "[steering] no gpiochip device, GPIO%u disabled\n", line_offset);
        return false;
    }

    const int chip = ::open(chip_path, O_RDONLY | O_CLOEXEC);
    if (chip < 0) {
        std::fprintf(stderr, "[steering] cannot open %s: %s\n", chip_path, std::strerror(errno));
        return false;
    }

    gpio_v2_line_request req{};
    std::strncpy(req.consumer, "lart_ui", sizeof(req.consumer) - 1);
    req.num_lines = 1;
    req.offsets[0] = line_offset;
    req.config.flags = GPIO_V2_LINE_FLAG_INPUT |
        (active_high ? GPIO_V2_LINE_FLAG_BIAS_PULL_DOWN : GPIO_V2_LINE_FLAG_BIAS_PULL_UP);
    const int ret = ioctl(chip, GPIO_V2_GET_LINE_IOCTL, &req);
    close(chip);
    if (ret < 0) {
        std::fprintf(stderr, "[steering] GPIO%u request failed: %s\n", line_offset, std::strerror(errno));
        return false;
    }

    fd_ = req.fd;
    active_high_ = active_high;
    bool pressed = false;
    if (read_pressed(pressed)) {
        stable_ = candidate_ = pressed;
    }
    candidate_since_ms_ = now_ms();
    return true;
}

bool SteeringButton::poll_pressed() {
    bool pressed = false;
    if (fd_ < 0 || !read_pressed(pressed)) {
        return false;
    }

    const long long now = now_ms();
    if (pressed != candidate_) {
        candidate_ = pressed;
        candidate_since_ms_ = now;
        return false;
    }
    if (candidate_ == stable_ || now - candidate_since_ms_ < kDebounceMs) {
        return false;
    }

    stable_ = candidate_;
    return stable_;
}

bool SteeringButton::read_pressed(bool &pressed) const {
    gpio_v2_line_values values{};
    values.mask = 1;
    if (ioctl(fd_, GPIO_V2_LINE_GET_VALUES_IOCTL, &values) < 0) {
        return false;
    }
    const bool high = (values.bits & 1u) != 0;
    pressed = high == active_high_;
    return true;
}
