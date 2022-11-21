#pragma once

#include "../vec2.hpp"
#include "../../80_EXTERN/consoleengine/Pixel.hpp"

class robj
{
public:
    enum LAYER
    {
        B2, B1, D, F1, F2, F3, F4
    };

protected:
    v2 cr;
    v2 wh;
    LAYER lay;

public:
    robj(): cr{0,0}, wh{1,1}, lay(D) {}
    robj(const int x, const int y): cr{y, x}, wh{1,1}, lay(D) {}
    robj(const int x, const int y, const LAYER l): cr{y, x}, wh{1,1}, lay(l) {}
    robj(const int x, const int y, const int w, const int h): cr{y, x}, wh{h, w}, lay(D) {}

    inline virtual pixelstr draw(unsigned long long tick) {return pixelstr();}
    inline virtual void physics(unsigned long long tick) {}
    inline virtual pixelstr get_coord_str() {pixelstr ps; std::string s = std::string(CSI) + std::to_string(cr.vert + 1) + ';' + std::to_string(cr.hori + 1) + 'H'; Pixel::copy_string_to_pixel_string(ps, s); return ps;}

    inline v2 &coords() {return cr;}
    inline v2 &size() {return wh;}
    inline LAYER &layer() {return lay;}
};
