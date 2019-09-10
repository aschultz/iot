#include "../include/util.h"
#include <cstdarg>
#include <system_error>

std::string stringFormat(const char *__restrict __format, ...) {
    va_list args;
    int len;

    va_start(args, __format);
    len = vsnprintf( nullptr, 0, __format, args);
    va_end(args);
    
    if (len < 0) {
        throw std::runtime_error("Failed to calculate formatted string length");
    }

    std::string result;
    result.resize(len);
    
    va_start(args, __format);
    len = vsnprintf( result.data(), result.length() + 1, __format, args);
    va_end(args);

    if (len != result.length()) {
        throw std::runtime_error("Formatted string does not match calculation");
    }

    return result;
}