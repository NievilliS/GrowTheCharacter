#pragma once

#include "CharObj.hpp"
#include "Utils.hpp"

class rbuttonobj : public rcharobj
{
protected:
    const static char CH_UNP = 'O';
    int ID = 64;
    int rID = 64;
    int tick_delay = 0;
    int tick_start = 0;
    bool pressed = false;
    char chr_pressed = '_';
    Pixel::Color col_pressed = Pixel::BLACK;

public:
    rbuttonobj(const int x, const int y, const int _tick_delay, const int ID_press = 64u, const int ID_release = 64u) : rcharobj(x, y, CH_UNP, Pixel::YELLOW), ID(ID_press), rID(ID_release), tick_delay(_tick_delay) {}
    rbuttonobj(const int x, const int y, const int _tick_delay, LAYER l, const int ID_press = 64u, const int ID_release = 64u) : rcharobj(x, y, CH_UNP, Pixel::YELLOW, l), ID(ID_press), rID(ID_release), tick_delay(_tick_delay) {}
    rbuttonobj(const int x, const int y, const int _tick_delay, const bool _pressed, const int ID_press = 64u, const int ID_release = 64u) : rcharobj(x, y, CH_UNP, Pixel::YELLOW), ID(ID_press), rID(ID_release), tick_delay(_tick_delay), pressed(_pressed) {}
    rbuttonobj(const int x, const int y, const int _tick_delay, const bool _pressed, LAYER l, const int ID_press = 64u, const int ID_release = 64u) : rcharobj(x, y, CH_UNP, Pixel::YELLOW, l), ID(ID_press), rID(ID_release), tick_delay(_tick_delay), pressed(_pressed) {}

    inline virtual pixelstr draw(unsigned long long tick) override
    {
        pixelstr ps{};
        if (pressed)
        {
            ps = Pixel::create_pixel(chr_pressed, col_pressed);
        }
        else
        {
            ps = Pixel::create_pixel(chr, col);
        }
        return ps;
    }

    inline virtual void physics(unsigned long long tick) override
    {
        if(tick_delay != -1 && pressed && tick > tick_start + tick_delay)
        {
            pressed = false;
            Utils::do_trig(rID);
        }
    }

    inline void press(unsigned long long tick)
    {
        if(!pressed)
        {
            tick_start = tick;
            pressed = true;
            Utils::do_trig(ID);
        }
    }
};
