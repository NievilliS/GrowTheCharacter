#pragma once

#include "SolidObj.hpp"

class rrollerspawnerobj : public rsolidobj
{
protected:
    void *m_env;
    direction_e m_direction;
    int m_period_self;
    int m_period_roll;
    char m_spawns_char;
    unsigned int m_t_ct = 0;

public:
    rrollerspawnerobj(void *_env, const int x, const int y, const char sp_ch, const char ro_ch, const direction_e _d, const int period_self, const int period_roller, const Pixel::Color _col = Pixel::DEFAULT):
        rsolidobj(x, y, true, sp_ch, _col), m_env(_env), m_direction(_d), m_period_self(period_self), m_period_roll(period_roller), m_spawns_char(ro_ch) {}
    rrollerspawnerobj(void *_env, const int x, const int y, const char sp_ch, const char ro_ch, const direction_e _d, const int period_self, const int period_roller, const layer_e _l, const Pixel::Color _col = Pixel::DEFAULT):
        rsolidobj(x, y, true, sp_ch, _col, _l), m_env(_env), m_direction(_d), m_period_self(period_self), m_period_roll(period_roller), m_spawns_char(ro_ch) {}

    virtual bool physics(unsigned long long tick) override;
};
