#pragma once
#include <cstdlib>
#include <cstdint>
#include <cstdbool>
#include <memory>

#include "i2c.h"

enum BlinkRate {
    Off = 0,
    TwoHz = 1,
    OneHz = 2,
    HalfHz = 3,
};

class HT16K33 {
public:
    HT16K33(std::shared_ptr<I2CDevice> device);

    // Configure display blink frequency
    void setBlinkRate(BlinkRate rate);

    // Set display brightness. Accepts value from 0 to 16.
    void setBrightness(uint8_t brightness);

    void setBuffer(uint8_t *data, size_t size);
    void fillBuffer(uint8_t value);

private:
    void configureDisplay();
    void writeCommand(uint8_t command);

    std::shared_ptr<I2CDevice> device;
    uint8_t buffer[17];
    uint8_t blinkRate;
    uint8_t brightness;
};