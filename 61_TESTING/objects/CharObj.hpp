#pragma once

#include "RObj.hpp"

class rcharobj : public robj
{
protected:
    char chr;
    Pixel::Color col;

public:
    rcharobj(): robj(), chr(0), col(Pixel::DEFAULT) {}
    rcharobj(const int x, const int y): robj(x, y), chr(0), col(Pixel::DEFAULT) {}
    rcharobj(const int x, const int y, const char c): robj(x, y), chr(c), col(Pixel::DEFAULT) {}
    rcharobj(const int x, const int y, const char c, const Pixel::Color col): robj(x, y), chr(c), col(col) {}
    rcharobj(const int x, const int y, const LAYER l): robj(x, y, l), chr(0), col(Pixel::DEFAULT) {}
    rcharobj(const int x, const int y, const char c, const LAYER l): robj(x, y, l), chr(c), col(Pixel::DEFAULT) {}
    rcharobj(const int x, const int y, const char c, const Pixel::Color col, const LAYER l): robj(x, y, l), chr(c), col(col) {}

    inline virtual pixelstr draw(unsigned long long tick) override
    {
        pixelstr ps;
        ps += Pixel::create_pixel(chr, col);
        return ps;
    }
};
