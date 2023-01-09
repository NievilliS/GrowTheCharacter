#pragma once

#include "RObj.hpp"

class rcharobj : public robj
{
protected:
    char m_character;
    Pixel::Color col;

public:
    rcharobj() : robj(), m_character(0), col(Pixel::DEFAULT) {}
    rcharobj(const int x, const int y) : robj(x, y), m_character(0), col(Pixel::DEFAULT) {}
    rcharobj(const int x, const int y, const char c) : robj(x, y), m_character(c), col(Pixel::DEFAULT) {}
    rcharobj(const int x, const int y, const char c, const Pixel::Color col) : robj(x, y), m_character(c), col(col) {}
    rcharobj(const int x, const int y, const layer_e l) : robj(x, y, l), m_character(0), col(Pixel::DEFAULT) {}
    rcharobj(const int x, const int y, const char c, const layer_e l) : robj(x, y, l), m_character(c), col(Pixel::DEFAULT) {}
    rcharobj(const int x, const int y, const char c, const Pixel::Color col, const layer_e l) : robj(x, y, l), m_character(c), col(col) {}

    inline virtual pixelstr draw(unsigned long long tick) override
    {
        pixelstr ps{};
        if(m_character <= 0) return ps;
        ps += Pixel::create_pixel(m_character, col);
        return ps;
    }

    inline virtual char get_raw_character()
    {
        return m_character;
    }
};
