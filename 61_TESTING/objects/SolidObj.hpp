#pragma once

#include "CharObj.hpp"

class rsolidobj : public rcharobj
{
protected:
    bool solid = true;

public:
    rsolidobj(): rcharobj() {}
    rsolidobj(const int x, const int y, const bool _solid): rcharobj(x,y), solid(_solid) {}
    rsolidobj(const int x, const int y, const bool _solid, const char c): rcharobj(x, y, c), solid(_solid) {}
    rsolidobj(const int x, const int y, const bool _solid, const char c, const Pixel::Color col): rcharobj(x,y,c,col), solid(_solid) {}
    rsolidobj(const int x, const int y, const bool _solid, const LAYER l): rcharobj(x,y,l), solid(_solid) {}
    rsolidobj(const int x, const int y, const bool _solid, const char c, const LAYER l): rcharobj(x,y,c,l), solid(_solid) {}
    rsolidobj(const int x, const int y, const bool _solid, const char c, const Pixel::Color col, const LAYER l): rcharobj(x,y,c,col,l), solid(_solid) {}

    inline virtual bool is_solid()
    {
        return solid;
    }
};
