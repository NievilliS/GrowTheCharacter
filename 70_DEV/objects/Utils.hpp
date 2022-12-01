#pragma once

#include <functional>
#include "../vec2.hpp"

class Utils
{
protected:
    inline static void *m_env_ptr;

public:
    static void init(void *_env_ptr);
    static void do_trig(const __UINT8_TYPE__ t);
    static bool is_trig(const __UINT8_TYPE__ t);
    static void do_event(const unsigned long long delay, std::function<void()> fct);
    static v2 *get_env_pcoord();
};
