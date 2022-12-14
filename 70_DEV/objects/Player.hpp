#pragma once

#include "CharObj.hpp"

class rplayerobj : public rcharobj
{
protected:
    void *m_env_ptr;
    bool m_can_move = true;
    void *m_checkpoint_ptr = nullptr;
    v2 m_prev_pos;
    bool m_god_mode = false;

public:
    rplayerobj(void *env_ptr, const int x, const int y, const char c) : rcharobj(x, y, c, F3), m_env_ptr(env_ptr) {}
    rplayerobj(void *env_ptr, const int x, const int y, const char c, const Pixel::Color col) : rcharobj(x, y, c, col, F3), m_env_ptr(env_ptr) {}

    virtual bool physics(unsigned long long tick) override;
    bool can_move() const;
    void set_can_move(const bool b);
    inline v2 *get_pcoord_ptr() { return &this->m_primary_coords; };
    void set_checkpoint(void *_checkpoint_ptr);
    void die();
    void check_ded(const unsigned char base_c);
    void set_god_mode(const bool b) {this->m_god_mode = b;}
    bool is_god_mode() const {return this->m_god_mode;}

    template<typename T>
    void plr_check_collision(T &t, const unsigned long long tick, int xtra_par = 0);
};
