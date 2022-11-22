#pragma once

#include "SolidObj.hpp"

class rdisappearingwall : public rsolidobj
{
protected:
    int dID = 64;
    int rID = 64;
    Pixel::Color col_a;

public:
    rdisappearingwall(const int x, const int y, const int dID = 64, const int rID = 64): rsolidobj(x,y,true,'#'), dID(dID), rID(rID), col_a(Pixel::DEFAULT) {}
    rdisappearingwall(const int x, const int y, const Pixel::Color col, const int dID = 64, const int rID = 64): rsolidobj(x,y,true,'#',col), dID(dID), rID(rID), col_a(col) {}
    rdisappearingwall(const int x, const int y, const LAYER l, const int dID = 64, const int rID = 64): rsolidobj(x,y,true,'#',l), dID(dID), rID(rID), col_a(Pixel::DEFAULT) {}
    rdisappearingwall(const int x, const int y, const LAYER l, const Pixel::Color col, const int dID = 64, const int rID = 64): rsolidobj(x,y,true,'#',col,l), dID(dID), rID(rID), col_a(col) {}

    virtual void physics(unsigned long long tick) override;
    virtual bool is_solid() override;
};
