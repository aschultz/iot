#include "../include/sevenSegment.h"
#include "../include/util.h"

// Each segment of a 7-segment digit is mapped to a single bit.
//        0
//     -------
//   5 |     | 1
//     |  6  |
//     |-----|
//   4 |     | 2
//     |  3  |
//     -------
//
//

const uint8_t charMap[] = {
    0, // (space)
    0, // !
    0, // "
    0, // #
    0, // $
    0, // %
    0, // &
    0, // '
    0, // (
    0, // )
    0, // *
    0, // +
    0, // ,
    0b01000000, // -
    0, // .
    0, // /

    0b00011111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111, // 9

    0, // :
    0, // ;
    0, // <
    0, // =
    0, // >
    0, // ?
    0, // @

    0b01110111, // A
    0b01111100, // b,
    0b00111001, // C,
    0b01011110, // d,
    0b01111001, // E,
    0b01110001, // F,
    0b01111101, // G,
    0b01110110, // H,
    0b00000110, // I,
    0b00001110, // J,
    0, // K
    0b00111000, // L
    0, // M
    0, // N
    0b00011111, // O
    0b01110011, // P
    0, // Q
    0b01010000, // r
    0b01101101, // S
    0b00110001, // T
    0b00111110, // U
    0, // V
    0, // W
    0, // X
    0b01101111, // y
    0b01011011, // Z

    0b00111001, // [
    0, // \ 
    0b00001111, // ]
    0, // ^
    0b00001000, // _
    0, // `
};

SevenSegmentDisplay::SevenSegmentDisplay(std::shared_ptr<HT16K33> device)
    : device(device) {
}

void SevenSegmentDisplay::setChar(int position, char value) {
    if (position > 3) {
        return;
    }

    if (position > 1) {
        ++position;
    }

    this->displayBuffer[position] = getLightcode(value);
 }

void SevenSegmentDisplay::setText(std::string str) {

}

void SevenSegmentDisplay::setDots(Dots dots) {
    this->displayBuffer[2] = (dots << 1) & 0b00011110;
}

void SevenSegmentDisplay::commit() {
    this->device->setBuffer(this->displayBuffer, sizeOf(this->displayBuffer));
}

uint8_t SevenSegmentDisplay::getLightcode(char c) {
    c = toupper(c);

    if (c >= ' ' && c <= '`') {
        return charMap[c - ' '];
    }

    return 0;
}