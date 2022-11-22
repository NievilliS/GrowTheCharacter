#pragma once

#include "CharObj.hpp"

class rspinnyobj : public rcharobj
{
private:
    v2 xy;
    int period;
    int offset;

public:
    rspinnyobj(const int x, const int y, const char c, const int period, const int offset, const Pixel::Color col = Pixel::DEFAULT): rcharobj(x, y, c, col), xy{y, x + 1}, period(period), offset(offset) {}
    rspinnyobj(const int x, const int y, const char c, const int period, const int offset, const LAYER l, const Pixel::Color col = Pixel::DEFAULT): rcharobj(x, y, c, col, l), xy{y, x + 1}, period(period), offset(offset) {}

    inline virtual pixelstr get_coord_str() override {pixelstr ps{}; std::string s = std::string(CSI) + std::to_string(xy.vert + 1) + ';' + std::to_string(xy.hori + 1) + 'H'; Pixel::copy_string_to_pixel_string(ps, s); return ps;}

    inline virtual void physics(unsigned long long tick) override
    {
        auto now = (offset + (tick / period)) % 8;

        switch(now)
        {
            case 0:
                xy = cr + v2{0,1};
            break; case 1:
                xy = cr + v2{1,1};
            break; case 2:
                xy = cr + v2{1,0};
            break; case 3:
                xy = cr + v2{1,-1};
            break; case 4:
                xy = cr + v2{0,-1};
            break; case 5:
                xy = cr + v2{-1,-1};
            break; case 6:
                xy = cr + v2{-1,0};
            break; case 7:
                xy = cr + v2{-1,1};
        }
    }

    inline virtual v2 &coords() {return xy;}

};
