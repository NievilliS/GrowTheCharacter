#pragma once

class Utils
{
protected:
    inline static void *env_ptr;

public:
    static void init(void *_env_ptr);
    static void do_trig(const __UINT8_TYPE__ t);
    static bool is_trig(const __UINT8_TYPE__ t);
};
