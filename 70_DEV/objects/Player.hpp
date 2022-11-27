#pragma once

#include "CharObj.hpp"

class rplayerobj : public rcharobj
{
protected:
    void *m_env_ptr;
    bool m_can_move = true;

public:
    rplayerobj(void *env_ptr, const int x, const int y, const char c): rcharobj(x, y, c, F3), m_env_ptr(env_ptr) {}
    rplayerobj(void *env_ptr, const int x, const int y, const char c, const Pixel::Color col): rcharobj(x, y, c, col, F3), m_env_ptr(env_ptr) {}

    virtual void physics(unsigned long long tick) override;
    bool can_move() const;
    void set_can_move(const bool b);
};
