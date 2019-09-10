#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstdbool>
#include <ctime>
#include <chrono>
#include <thread>

#include "../include/util.h"
#include "../include/i2c.h"
#include "../include/ht16k33.h"
#include "../include/sevenSegment.h"

const int adapterId = 1;
const int deviceAddress = 0x70;

std::string getTime() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto timeInfo = localtime(&time);
    char buffer[5] = {};

    strftime(buffer, sizeOf(buffer), "%H%M", timeInfo);

    return std::string(buffer);
}

int main(int argc, const char * argv[]) {

    auto rawDevice = std::make_shared<I2CDevice>(adapterId, deviceAddress);
    auto deviceDriver = std::make_shared<HT16K33>(rawDevice);
    auto display = std::make_shared<SevenSegmentDisplay>(deviceDriver);

    deviceDriver->setBlinkRate(BlinkRate::Off);
    deviceDriver->setBrightness(16);
    display->setDots(Dots::Colon);

    while (true) {
        auto time = getTime();
        display->setText(time);
        std::this_thread::sleep_for (std::chrono::milliseconds(500));
    }

    printf("Hello world!");
    return 0;
}

