#pragma once
#include <cstdlib>
#include <cstdint>
#include <cstdbool>
#include <memory>
#include <string>

#include "ht16k33.h"

enum Dots {
    None = 0,
    Colon = 1,
    TL = 2,
    BL = 4,
    TR = 8
};

class SevenSegmentDisplay {
public:
    SevenSegmentDisplay(std::shared_ptr<HT16K33> device);

    void setChar(int position, char value);
    void setDots(Dots dots);
    void setText(std::string str);

    void commit();

private:
    uint8_t getLightcode(char c);

    std::shared_ptr<HT16K33> device;
    uint8_t displayBuffer[5];
};