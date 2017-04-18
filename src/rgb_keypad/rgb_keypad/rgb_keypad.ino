/*
 Name:		rgb_keypad.ino
 Created:	4/17/2017 11:15:34 PM
 Author:	Aaron
*/

#include "Adafruit_TLC59711.h"
#include "color.h"

#define NUM_TLC59711 1
#define TLC_SPI_DATA_PIN   11
#define TLC_SPI_CLOCK_PIN  13

Adafruit_TLC59711 tlc = Adafruit_TLC59711(NUM_TLC59711, TLC_SPI_CLOCK_PIN, TLC_SPI_DATA_PIN);

void setup()
{
	Serial.begin(9600);
	Serial.println("Test");

	tlc.begin();
	tlc.write();
}

void loop()
{
  
}
