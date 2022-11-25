#pragma once

#include "CharObj.hpp"
#include <cmath>

#define sign(a) ((a) < 0 ? -1.0 : 1.0)
#define period(a) (sign(a) * (((int) abs(a)) % 32 > 15 ? -1.0 : 1.0))
#define abs(a) ((a) < 0 ? -(a) : (a))
inline double aprx_sin16(double in)
{
    const static double _A = -1.0/9216.0;
    const static double _B = -5.0/384.0;
    const static double _C = 17.0/72.0;

    double _abs = fmod(abs(in), 16.0);
    return period(in) * ((_A * _abs * _abs * _abs) + (_B * _abs * _abs) + (_C * _abs));
}

inline double aprx_cos16(double in)
{
    return aprx_sin16(in + 8.0);
}

class rspinnyobj : public rcharobj
{
private:
    v2 m_secondary_coords;
    double m_period;
    double m_offset;
    double m_radius;

public:
    rspinnyobj(const int x, const int y, const char c, const double period, const double offset, const int radius, const Pixel::Color col = Pixel::DEFAULT): rcharobj(x, y, c, col), m_secondary_coords{y, x + 1}, m_period(period), m_offset(offset), m_radius(radius + 1) {}
    rspinnyobj(const int x, const int y, const char c, const double period, const double offset, const int radius, const layer_e l, const Pixel::Color col = Pixel::DEFAULT): rcharobj(x, y, c, col, l), m_secondary_coords{y, x + 1}, m_period(period), m_offset(offset), m_radius(radius + 1) {}

    inline virtual pixelstr get_coord_str() override {pixelstr ps{}; std::string s = std::string(CSI) + std::to_string(m_secondary_coords.vert + 1) + ';' + std::to_string(m_secondary_coords.hori + 1) + 'H'; Pixel::copy_string_to_pixel_string(ps, s); return ps;}

    inline virtual void physics(unsigned long long tick) override
    {
        auto now = (m_offset + (tick / m_period));

        m_secondary_coords = m_primary_coords + v2{(int) (aprx_sin16(now) * (m_radius - .2)), (int) (aprx_cos16(now) * (m_radius -.2))};
    }

    inline virtual v2 &coords() {return m_secondary_coords;}

};
