#pragma once

#include <consoleengine/Pixel.hpp>

inline __UINT8_TYPE__ __pxclext__next__()
{
    static __UINT8_TYPE__ __stat_signature_ = 9;
    return __stat_signature_++;
}
#define as_color(hex)                                                     \
    {                                                                     \
        __pxclext__next__(), CSI "38:5:" #hex ":m", CSI "48:5:" #hex ":m" \
    }

namespace Pixel::Extensions
{

    const static inline Pixel::Color
        GRAY as_color(8),
        DARK_RED as_color(52);

};

#undef as_color
