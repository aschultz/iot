#pragma once
#include <cstdlib>
#include <string>

template <typename T, size_t N>
constexpr size_t sizeOf(T (&)[N]) {
    return N;
}

std::string stringFormat(const char *__restrict __format, ...);