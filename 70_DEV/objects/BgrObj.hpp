#pragma once

#include "CharObj.hpp"
#include <vector>

class rbgrobj : public rcharobj
{
protected:
    pixelstr m_comb;

public:
    rbgrobj(const int x, const int y, const Pixel::Color col);
    rbgrobj(std::vector<rbgrobj*> &&vec, Pixel::Color end_col);
    inline virtual pixelstr draw(unsigned long long tick) override { return m_comb; }
};
