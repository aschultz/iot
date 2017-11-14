/*
 Name:		rgb_keypad.ino
 Created:	4/17/2017 11:15:34 PM
 Author:	Aaron
*/

#include "TLC59711.h"
#include "Common.h"
#include "Adafruit_TLC59711.h"
#include "Keypad.h"
#include "Color.h"
#include "ScramblePad.h"

#define NUM_TLC59711 1
#define TLC_SPI_DATA_PIN   11
#define TLC_SPI_CLOCK_PIN  13

const uint8_t keypadBtnRowPins[] = { 4, 5, 6, 7 };
const uint8_t keypadBtnColPins[] = { 8, 9, 14, 15 };
const uint8_t keypadLEDColPins[] = { 16, 17,18, 19 };

// List keys in pin order, 
array<const char> keypadChars = toArray("12345678ABCDEFGH");

uint8_t keyCode[] = { 0, 1, 2, 3 };

TLC59711 tlc(NUM_TLC59711);
Keypad keypad(toArray(keypadBtnRowPins), toArray(keypadBtnColPins));
ScramblePad scramblePad(keypad);

void setup()
{
	Serial.begin(9600);
	Serial.println("Test");

    scramblePad.SetKeyCode( toArray(keyCode) );

	tlc.begin();
	tlc.submit();
}

void loop()
{
    // Read in the current state of the button pad 
    keypad.Update();

    for (uint8_t i = 0; i < countOf(keypadLEDColPins); ++i)
    {
        // Configure the TLC with the colors we want for this column
        tlc.setLED(0, 65535, 0, 0);
        tlc.setLED(1, 0, 65535, 0);
        tlc.setLED(2, 0, 0, 65535);
        tlc.setLED(3, 65535, 65535, 65535);
        tlc.submit();

        // Enable the transistor for this column, allowing power to flow
        uint8_t ledPin = keypadLEDColPins[i];
        digitalWrite(ledPin, HIGH);

        // Wait for some time so you can see the light
        delayMicroseconds(1000);

        // Disable the transistor
        digitalWrite(ledPin, LOW);
    }
}
