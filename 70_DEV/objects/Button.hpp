#pragma once

#include "CharObj.hpp"
#include "Utils.hpp"
#include "../ColorExtensions.hpp"

class rbuttonobj : public rcharobj
{
protected:
    const static char c_char_unpressed = 'O';
    const static char c_char_pressed = '_';
    const Pixel::Color col_pressed = Pixel::Extensions::GRAY;

    int m_press_ID = 64;
    int m_release_ID = 64;
    int m_tick_delay = 0;
    int m_tick_start = 0;
    bool m_pressed = false;

public:
    rbuttonobj(const int x, const int y, const int _tick_delay, const int ID_press = 64u, const int ID_release = 64u) :
        rcharobj(x, y, c_char_unpressed, Pixel::WHITE),
        m_press_ID(ID_press), m_release_ID(ID_release), m_tick_delay(_tick_delay)
    {}

    rbuttonobj(const int x, const int y, const int _tick_delay, layer_e l, const int ID_press = 64u, const int ID_release = 64u) :
        rcharobj(x, y, c_char_unpressed, Pixel::WHITE, l),
        m_press_ID(ID_press), m_release_ID(ID_release), m_tick_delay(_tick_delay)
    {}

    rbuttonobj(const int x, const int y, const int _tick_delay, const bool _pressed, const int ID_press = 64u, const int ID_release = 64u) :
        rcharobj(x, y, c_char_unpressed, Pixel::WHITE),
        m_press_ID(ID_press), m_release_ID(ID_release), m_tick_delay(_tick_delay), m_pressed(_pressed)
    {}

    rbuttonobj(const int x, const int y, const int _tick_delay, const bool _pressed, layer_e l, const int ID_press = 64u, const int ID_release = 64u) :
        rcharobj(x, y, c_char_unpressed, Pixel::WHITE, l),
        m_press_ID(ID_press), m_release_ID(ID_release), m_tick_delay(_tick_delay), m_pressed(_pressed)
    {}

    inline virtual pixelstr draw(unsigned long long tick) override
    {
        pixelstr ps{};
        if (m_pressed)
        {
            ps = Pixel::create_pixel(c_char_pressed, col_pressed);
        }
        else
        {
            ps = Pixel::create_pixel(m_character, col);
        }
        return ps;
    }

    inline virtual void physics(unsigned long long tick) override
    {
        if(m_tick_delay != -1 && m_pressed && tick > m_tick_start + m_tick_delay)
        {
            m_pressed = false;
            Utils::do_trig(m_release_ID);
        }
    }

    inline void press(unsigned long long tick)
    {
        if(!m_pressed)
        {
            m_tick_start = tick;
            m_pressed = true;
            Utils::do_trig(m_press_ID);
        }
    }
};
