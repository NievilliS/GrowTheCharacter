#pragma once

#include "CharObj.hpp"

class rspeedbump : public rcharobj
{
public:
    enum direction : __UINT8_TYPE__
    {
        UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3
    };

    static const char dir_to_char(const direction &d)
    {
        switch(d)
        {
            case UP:
                return '^';
            case DOWN:
                return 'v';
            case LEFT:
                return '<';
            case RIGHT:
                return '>';
        }
        return '?';
    }

protected:
    direction m_direction;

public:
    rspeedbump(const direction _direction, const int x, const int y): rcharobj(x,y,dir_to_char(_direction),Pixel::YELLOW,B1), m_direction(_direction) {}
    rspeedbump(const direction _direction, const int x, const int y, const LAYER l): rcharobj(x,y,dir_to_char(_direction),Pixel::YELLOW,l), m_direction(_direction) {}

    const void push_v2(v2 *cord)
    {
        switch(this->m_direction)
        {
            case UP:
                cord->vert--;
            break;
            case DOWN:
                cord->vert++;
            break;
            case LEFT:
                cord->hori--;
            break;
            case RIGHT:
                cord->hori++;
        }
    }

};
