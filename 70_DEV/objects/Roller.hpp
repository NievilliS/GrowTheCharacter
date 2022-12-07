#pragma once

#include "CharObj.hpp"

class rrollerobj : public rcharobj
{
protected:
    void *m_env;
    direction_e m_direction;
    int m_period;
    bool m_destroy = false;
    unsigned long long m_ltick;

public:
    rrollerobj(void *_env, const int x, const int y, const char c, const direction_e _dir, const int _period, const Pixel::Color _pcol = Pixel::DEFAULT);
    rrollerobj(void *_env, const int x, const int y, const char c, const direction_e _dir, const int _period, const layer_e l, const Pixel::Color _pcol = Pixel::DEFAULT);
    
    virtual bool physics(unsigned long long tick) override;
};
