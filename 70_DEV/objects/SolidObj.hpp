#pragma once

#include "CharObj.hpp"

class rsolidobj : public rcharobj
{
protected:
    bool m_solid = true;

public:
    rsolidobj(): rcharobj() {}
    rsolidobj(const int x, const int y, const bool _solid): rcharobj(x,y), m_solid(_solid) {}
    rsolidobj(const int x, const int y, const bool _solid, const char c): rcharobj(x, y, c), m_solid(_solid) {}
    rsolidobj(const int x, const int y, const bool _solid, const char c, const Pixel::Color col): rcharobj(x,y,c,col), m_solid(_solid) {}
    rsolidobj(const int x, const int y, const bool _solid, const layer_e l): rcharobj(x,y,l), m_solid(_solid) {}
    rsolidobj(const int x, const int y, const bool _solid, const char c, const layer_e l): rcharobj(x,y,c,l), m_solid(_solid) {}
    rsolidobj(const int x, const int y, const bool _solid, const char c, const Pixel::Color col, const layer_e l): rcharobj(x,y,c,col,l), m_solid(_solid) {}

    inline virtual bool is_solid()
    {
        return m_solid;
    }
};
