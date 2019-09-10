#include "../include/ht16k33.h"
#include <cstring>
#include <algorithm>


// 8-bit command codes
//  
//  00000000 - Set display address pointer to 0. Follow with 16 bytes of display data.
//  0010000X - Enable device. X=1 -> enable. X=0 -> standby.
//  10000XXY - Configure display. Set XX to configure blinking. Set Y to enable/disable display.
//  1110XXXX - Configure brightness. XXXX = 1 of 16 dimming levels


HT16K33::HT16K33(std::shared_ptr<I2CDevice> device)
    : device(device) {

    // Enable device clock
    writeCommand(0b00100001);
}

void HT16K33::setBlinkRate(BlinkRate rate) {
    this->blinkRate = rate & 0x3;

    configureDisplay();
}

void HT16K33::setBrightness(uint8_t brightness) {
    if (brightness > 16) {
        brightness = 16;
    }
    this->brightness = brightness;

    configureDisplay();
}

void HT16K33::configureDisplay() {
    uint8_t brightness = this->brightness > 0 ? this->brightness - 1 : 0;
    uint8_t cmd = 0b11100000 | brightness;

    writeCommand(cmd);

    cmd = 0b10000000 | (this->blinkRate << 1) | (this->brightness > 0 ? 1 : 0);

    writeCommand(cmd);
}

void HT16K33::writeCommand(uint8_t command) {
    uint8_t buffer[1] = { command };

    this->device->writeData(buffer, 1);
}

void HT16K33::fillBuffer(uint8_t value) {
    uint8_t buffer[17] = {};

    // First byte is command = 0
    memset(buffer + 1, value, 16);

    this->device->writeData(buffer, 17);
}

void HT16K33::setBuffer(uint8_t *data, size_t size) {
    uint8_t buffer[17] = {};

    // First byte is command = 0
    memcpy(buffer + 1, data, std::min<size_t>(size, 16));

    this->device->writeData(buffer, 17);
}