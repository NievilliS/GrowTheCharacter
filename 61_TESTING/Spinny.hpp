#pragma once

#include "CharObj.hpp"

class rspinnyobj : public rcharobj
{
private:
    v2 xy;

public:
    rspinnyobj(const int x, const int y, const char c, const Pixel::Color col = Pixel::DEFAULT): rcharobj(x, y, c, col), xy{y, x + 1} {}

    inline virtual std::string get_coord_str() {return std::string(CSI) + std::to_string(xy.vert + 1) + ';' + std::to_string(xy.hori + 1) + 'H';}

    inline virtual void physics(unsigned long long tick) override
    {
        auto period = (tick / 4) % 8;

        switch(period)
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

};
