#pragma once

#include <iostream>
#include <DUNE/DControlManager.hpp>

#define KEY_UP_BIT (0)
#define KEY_DOWN_BIT (2)
#define KEY_LEFT_BIT (4)
#define KEY_RIGHT_BIT (6)
#define KEY_ESC_BIT (8)

#define RTE_keypress_press(bit)     (keymap = (keymap & ~(1 << (1 + (bit)))) | (1 << (bit)))
#define RTE_keypress_release(bit)   (keymap = (keymap & ~(1 << (bit))) | (1 << (1 + (bit))))
#define RTE_keypress_is_down(bit)   ((keymap & (1 << (bit))) > 0)
#define RTE_keypress_is_up(bit)     ((keymap & (1 << (bit))) <= 0)
#define RTE_keypress_pressed(bit)   ((keymap & (1 << (1 + (bit)))) > ((keymap &= ~(1 << (1 + (bit)))) & 0))

#define _keypress_fcts_make(name, bit)                                                  \
    inline static void name ## _kbpress()   {RTE_keypress_press(bit);}                  \
    inline static void name ## _kbrelease() {RTE_keypress_release(bit);}                \
    inline static bool name ## _kbisdown()  {return RTE_keypress_is_down(bit);}         \
    inline static bool name ## _kbisup()    {return RTE_keypress_is_up(bit);}           \
    inline static bool name ## _kbpressed() {return RTE_keypress_pressed(bit);}         \


namespace RTE
{
    inline extern DManager::DControlManager *dcm_ptr = nullptr;
    inline extern int millis = 0;
    inline extern int excount = 0;

    inline extern int keymap = 0;

    /**** FUNCTIONS ****/
    _keypress_fcts_make(up, KEY_UP_BIT);
    _keypress_fcts_make(down, KEY_DOWN_BIT);
    _keypress_fcts_make(left, KEY_LEFT_BIT);
    _keypress_fcts_make(right, KEY_RIGHT_BIT);
    _keypress_fcts_make(escape, KEY_ESC_BIT);
    
    inline static void stop()
    {
        dcm_ptr->stop();
    }

    #define ms
};
