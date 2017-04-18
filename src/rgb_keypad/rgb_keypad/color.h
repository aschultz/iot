// color.h

#ifndef _COLOR_h
#define _COLOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


void hsvToRgb(int h, int s, int v, byte rgb[3]);

byte applyGamma(byte value);



#endif
