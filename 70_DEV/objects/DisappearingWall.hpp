#pragma once

#include "SolidObj.hpp"

class rdisappearingwall : public rsolidobj
{
protected:
    int m_press_ID = 64;
    int m_release_ID = 64;
    Pixel::Color m_appear_color;

public:
    rdisappearingwall(const int x, const int y, const int dID = 64, const int rID = 64) : rsolidobj(x, y, true, '#'), m_press_ID(dID), m_release_ID(rID), m_appear_color(Pixel::DEFAULT) {}
    rdisappearingwall(const int x, const int y, const Pixel::Color col, const int dID = 64, const int rID = 64) : rsolidobj(x, y, true, '#', col), m_press_ID(dID), m_release_ID(rID), m_appear_color(col) {}
    rdisappearingwall(const int x, const int y, const layer_e l, const int dID = 64, const int rID = 64) : rsolidobj(x, y, true, '#', l), m_press_ID(dID), m_release_ID(rID), m_appear_color(Pixel::DEFAULT) {}
    rdisappearingwall(const int x, const int y, const layer_e l, const Pixel::Color col, const int dID = 64, const int rID = 64) : rsolidobj(x, y, true, '#', col, l), m_press_ID(dID), m_release_ID(rID), m_appear_color(col) {}

    virtual bool physics(unsigned long long tick) override;
    virtual bool is_solid() override;
};
