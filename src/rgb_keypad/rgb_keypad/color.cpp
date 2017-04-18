// 
// 
// 

#include "color.h"

#include <avr/io.h>
#include <avr/pgmspace.h>

const static byte gamma[] PROGMEM = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,4,4,4,4,4,5,5,5,5,5,6,6,6,6,7,7,7,7,8,8,8,9,9,9,9,10,10,11,11,11,12,12,13,13,13,14,14,15,15,16,16,17,17,18,18,19,19,20,21,21,22,23,23,24,25,25,26,27,27,28,29,30,31,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,52,53,54,55,57,58,59,61,62,63,65,66,68,69,71,72,74,75,77,79,80,82,84,85,87,89,91,93,95,96,98,100,102,104,107,109,111,113,115,117,120,122,124,126,129,131,134,136,139,141,144,146,149,152,155,157,160,163,166,169,172,175,178,181,184,187,190,194,197,200,203,207,210,214,217,221,224,228,232,236,239,243,247,251,255 };

void hsvToRgb(int h, int s, int v, byte rgb[3])
{
	int r, g, b, base;

	if (s == 0)
	{
		r = g = b = v;
	}
	else
	{
		// http://forum.arduino.cc/index.php/topic,40601.0.html
		uint8_t sector = h / 60U;
		uint8_t remainder = (h - sector * 60U) * 64U / 15U;
		uint8_t p = v * (255U - s) / 255U;
		uint8_t q = v * (255UL * 255UL - ((long)s)*remainder) / (255UL * 255UL);
		uint8_t t = v * (255UL * 255UL - ((long)s)*(255U - remainder)) / (255UL * 255UL);

		base = ((255 - s)*v) >> 8;

		switch (h / 60)
		{
		case 0:
			r = v;
			g = t;
			b = p;
			break;

		case 1:
			r = q;
			g = v;
			b = p;
			break;

		case 2:
			r = p;
			g = v;
			b = t;
			break;

		case 3:
			r = p;
			g = q;
			b = v;
			break;

		case 4:
			r = t;
			g = p;
			b = v;
			break;

		case 5:
			r = v;
			g = p;
			b = q;
			break;
		}
	}
	rgb[0] = r;
	rgb[1] = g;
	rgb[2] = b;
}

byte applyGamma(byte value)
{
	// https://learn.adafruit.com/led-tricks-gamma-correction/the-issue
	// LED luminance (as perceived by human eye) is nonlinear.
	// Make it more linear by applying gamma curve.
	// Equivalent code:
	// float gamma = 4;
	// x = pow(x/255.0, gamma)*255 + 0.5;

	return pgm_read_byte(&gamma[value]);
}
