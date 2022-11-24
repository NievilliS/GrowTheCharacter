#pragma once

#include "CharObj.hpp"

class rspinnyobj : public rcharobj
{
private:
    v2 m_secondary_coords;
    int m_period;
    int m_offset;
    int m_radius;

public:
    rspinnyobj(const int x, const int y, const char c, const int period, const int offset, const int radius, const Pixel::Color col = Pixel::DEFAULT): rcharobj(x, y, c, col), m_secondary_coords{y, x + 1}, m_period(period), m_offset(offset), m_radius(radius) {}
    rspinnyobj(const int x, const int y, const char c, const int period, const int offset, const int radius, const layer_e l, const Pixel::Color col = Pixel::DEFAULT): rcharobj(x, y, c, col, l), m_secondary_coords{y, x + 1}, m_period(period), m_offset(offset), m_radius(radius) {}

    inline virtual pixelstr get_coord_str() override {pixelstr ps{}; std::string s = std::string(CSI) + std::to_string(m_secondary_coords.vert + 1) + ';' + std::to_string(m_secondary_coords.hori + 1) + 'H'; Pixel::copy_string_to_pixel_string(ps, s); return ps;}

    inline virtual void physics(unsigned long long tick) override
    {
        auto now = (m_offset + (tick / m_period));

        m_secondary_coords = w
    }

    inline virtual v2 &coords() {return m_secondary_coords;}

};
