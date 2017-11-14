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


inline uint8_t applyGamma(uint8_t value);
inline uint16_t applyGamma(uint16_t value);




void hsvToRgb(uint8_t h, uint8_t s, uint8_t v, uint8_t rgb[3]);


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
