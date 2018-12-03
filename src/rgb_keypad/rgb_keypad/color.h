// color.h

#ifndef _COLOR_h
#define _COLOR_h

#include <Arduino.h>

template< typename T >
struct ColorRGB
{
    T r;
    T g;
    T b;
};

typedef ColorRGB<uint8_t> ColorRGB8;
typedef ColorRGB<uint16_t> ColorRGB16;

/*
	- The Arduino Uno supports 8-bits per PWM channel
	- The TLC59711 supports 16-bits per PWM channel
*/


/*
	Scale an 8-bit value into a 16-bit value, linearly. 0 -> 0, 255 -> 65536
*/
uint16_t upscaleLinear(uint8_t value);

/*
	Downscale a
*/
uint8_t downscaleLinear(uint16_t value);



/*
	Applies gamma correction to an 8-bit color channel. This maps the input onto a quadratic curve, f(x)=x^gamma.
	The resulting curve, when used to set the physical intensity of a light, will appear to the human eye as a linear scale of brightness.
	@value: [0, 255]
	@return: A number in the range [0, 255]
*/
uint8_t applyGamma(uint8_t value);

/*
	Applies gamma correction to a 12-bit color channel and maps the value to a 16-bit output.
	@value:  A number in the range [0, 4095]  (2^12)
	@return: A number in the range [0, 65535] (2^16)
*/
uint16_t applyGamma(uint16_t value);


/*
	Convert 24-bit HSV coordinates to 24-bit RGB
*/
void hsvToRgb(uint8_t h, uint8_t s, uint8_t v, uint8_t rgb[3]);

/*
	Convert 36-bit HSV coordinates to 36-bit RGB (12-bit per channel)
	@h: Hue in the range [0, 4095]
	@s: Saturation in the range [0, 4095]
	@v: Value in the range [0, 4095]
	@r: 
*/
void hsvToRgb(uint16_t h, uint16_t s, uint16_t v, uint16_t &r, uint16_t &g, uint16_t &b);


// Map a value in the range [0, 255] to the range [0, 65535]
inline uint16_t eightToSixteen(uint8_t x) {
	return ((x + 1) << 8u) - 1;
}



#ifndef COLOR_DEPTH
#define COLOR_DEPTH 12
#endif // !COLOR_DEPTH





/*
template<size_t... II> struct integer_sequence { typedef integer_sequence type; };
template<size_t N, size_t... II> struct make_index_sequence;
template<size_t... II> struct make_index_sequence<0, II...> : integer_sequence<II...> {};
template<size_t N, size_t... II> struct make_index_sequence : make_index_sequence<N - 1, N - 1, II...> {};

template<typename T, int N, T(*Func)(int), typename Indices>
struct GeneratedArrayHelper;

template<typename T, int N, T(*Func)(int), size_t... i>
struct GeneratedArrayHelper<T, N, Func, integer_sequence<i...>> 
{
    static const T data[N];  // element i is initialized with Func(i)
};

template<typename T, int N, T(*Func)(int), size_t... i>
const T GeneratedArrayHelper<T, N, Func, integer_sequence<i...>>::data[N] = { Func(i)... };

template<typename T, int N, T(*Func)(int)>
struct GeneratedArray : GeneratedArrayHelper<T, N, Func, typename make_index_sequence<N>::type > {};

constexpr uint8_t GenerateGamma(int index) { return index; };

GeneratedArray<uint8_t, 256, GenerateGamma>::data Foo;
*/
#endif
