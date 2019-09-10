#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstdbool>

#include "../include/i2c.h"
#include "../include/ht16k33.h"
#include "../include/sevenSegment.h"

const int adapterId = 1;
const int deviceAddress = 0x70;

int main(int argc, const char * argv[]) {

    auto rawDevice = std::make_shared<I2CDevice>(adapterId, deviceAddress);
    auto deviceDriver = std::make_shared<HT16K33>(rawDevice);
    auto display = std::make_shared<SevenSegmentDisplay>(deviceDriver);

    deviceDriver->setBlinkRate(BlinkRate::Off);
    deviceDriver->setBrightness(16);

    printf("Hello world!");
    return 0;
}

