#pragma once

#include "CharObj.hpp"

class rplayerobj : public rcharobj
{
protected:
    void *env_ptr;

public:
    rplayerobj(void *env_ptr, const int x, const int y, const char c): rcharobj(x, y, c, F3), env_ptr(env_ptr) {}
    rplayerobj(void *env_ptr, const int x, const int y, const char c, const Pixel::Color col): rcharobj(x, y, c, col, F3), env_ptr(env_ptr) {}

    virtual void physics(unsigned long long tick) override;
};
