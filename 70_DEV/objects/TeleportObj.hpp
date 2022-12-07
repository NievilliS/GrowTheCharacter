#pragma once

#include "CharObj.hpp"
#include "Utils.hpp"

class rteleportobj : public rcharobj
{
protected:
    v2 m_secondary_coords;
    bool m_one_way;
    bool m_active = true;

public:
    rteleportobj(const int x, const int y, const int xd, const int yd, const int _one_way) : rcharobj(x, y, '@', Pixel::RED, D), m_secondary_coords{yd, xd}, m_one_way(_one_way > 0)
    {
    }

    inline v2 &coords2()
    {
        return m_secondary_coords;
    }

    inline pixelstr get_coord_str2()
    {
        pixelstr ps{};
        std::string s = std::string(CSI) + std::to_string(m_secondary_coords.vert + 1) + ';' + std::to_string(m_secondary_coords.hori + 1) + 'H';
        Pixel::copy_string_to_pixel_string(ps, s);
        return ps;
    }

    inline virtual pixelstr draw(unsigned long long tick) override
    {
        static Pixel_t pxl2nd = Pixel::create_pixel('@', Pixel::BLUE);
        static Pixel_t pxldea = Pixel::create_pixel('@', Pixel::BLACK);
        if (m_active)
            return rcharobj::draw(tick) + get_coord_str2() + pxl2nd;
        else
            return pixelstr{pxldea + get_coord_str2() + pxldea};
    }

    inline bool is_one_way()
    {
        return m_one_way;
    }

    inline void teleport(v2 *v)
    {
        m_active = false;
        if (*v == m_secondary_coords)
        {
            *v = m_primary_coords;
        }
        else if (*v == m_primary_coords)
        {
            *v = m_secondary_coords;
        }
    }

    inline bool is_active() const
    {
        return this->m_active;
    }

    inline void set_active()
    {
        this->m_active = true;
    }
};
