#pragma once

#include "CharObj.hpp"

class rspinnyobj : public rcharobj
{
private:
    v2 m_secondary_coords;
    int m_period;
    int m_offset;

public:
    rspinnyobj(const int x, const int y, const char c, const int period, const int offset, const Pixel::Color col = Pixel::DEFAULT): rcharobj(x, y, c, col), m_secondary_coords{y, x + 1}, m_period(period), m_offset(offset) {}
    rspinnyobj(const int x, const int y, const char c, const int period, const int offset, const layer_e l, const Pixel::Color col = Pixel::DEFAULT): rcharobj(x, y, c, col, l), m_secondary_coords{y, x + 1}, m_period(period), m_offset(offset) {}

    inline virtual pixelstr get_coord_str() override {pixelstr ps{}; std::string s = std::string(CSI) + std::to_string(m_secondary_coords.vert + 1) + ';' + std::to_string(m_secondary_coords.hori + 1) + 'H'; Pixel::copy_string_to_pixel_string(ps, s); return ps;}

    inline virtual void physics(unsigned long long tick) override
    {
        auto now = (m_offset + (tick / m_period)) % 8;

        switch(now)
        {
            case 0:
                m_secondary_coords = m_primary_coords + v2{0,1};
            break; case 1:
                m_secondary_coords = m_primary_coords + v2{1,1};
            break; case 2:
                m_secondary_coords = m_primary_coords + v2{1,0};
            break; case 3:
                m_secondary_coords = m_primary_coords + v2{1,-1};
            break; case 4:
                m_secondary_coords = m_primary_coords + v2{0,-1};
            break; case 5:
                m_secondary_coords = m_primary_coords + v2{-1,-1};
            break; case 6:
                m_secondary_coords = m_primary_coords + v2{-1,0};
            break; case 7:
                m_secondary_coords = m_primary_coords + v2{-1,1};
        }
    }

    inline virtual v2 &coords() {return m_secondary_coords;}

};
