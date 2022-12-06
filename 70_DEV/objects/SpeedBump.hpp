#pragma once

#include "CharObj.hpp"

class rspeedbump : public rcharobj
{
public:
    static const char dir_to_char(const direction_e &d)
    {
        switch (d)
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
    direction_e m_direction;

public:
    rspeedbump(const direction_e _direction, const int x, const int y) : rcharobj(x, y, dir_to_char(_direction), Pixel::YELLOW, B1), m_direction(_direction) {}
    rspeedbump(const direction_e _direction, const int x, const int y, const layer_e l) : rcharobj(x, y, dir_to_char(_direction), Pixel::YELLOW, l), m_direction(_direction) {}

    const void push_v2(v2 *cord)
    {
        switch (this->m_direction)
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
