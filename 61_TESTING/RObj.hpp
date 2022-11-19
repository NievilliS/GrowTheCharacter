#pragma once

#include "vec2.hpp"
#include "../80_EXTERN/consoleengine/Pixel.hpp"

class robj
{
protected:
    v2 cr;
    v2 wh;

public:
    robj(): cr{0,0}, wh{1,1} {}
    robj(const int x, const int y): cr{y, x}, wh{1,1} {}
    robj(const int x, const int y, const int w, const int h): cr{y, x}, wh{h, w} {}

    inline virtual pixelstr draw(unsigned long long tick) {return pixelstr();}
    inline virtual void physics(unsigned long long tick) {}
    inline virtual std::string get_coord_str() {return std::string(CSI) + std::to_string(cr.vert + 1) + ';' + std::to_string(cr.hori + 1) + 'H';}

    inline v2 &coords() {return cr;}
    inline v2 &size() {return wh;}
};
