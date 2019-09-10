#include <cstdlib>

template <typename T, size_t N>
constexpr size_t sizeOf(T (&)[N]) {
    return N;
}