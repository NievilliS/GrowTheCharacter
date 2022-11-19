#pragma once

#include "RObj.hpp"

class rcharobj : public robj
{
protected:
    char chr;
    Pixel::Color col;

public:
    rcharobj(): robj(0, 0, 1, 1), chr(0), col(Pixel::DEFAULT) {}
    rcharobj(const int x, const int y): robj(x, y, 1, 1), chr(0), col(Pixel::DEFAULT) {}
    rcharobj(const int x, const int y, const char c): robj(x, y, 1, 1), chr(c), col(Pixel::DEFAULT) {}
    rcharobj(const int x, const int y, const char c, const Pixel::Color col): robj(x, y, 1, 1), chr(c), col(col) {}

    inline virtual pixelstr draw(unsigned long long tick) override
    {
        pixelstr ps;
        ps += Pixel::create_pixel(chr, col);
        return ps;
    }
};
