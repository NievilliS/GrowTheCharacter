/**
 * @file Button.hpp
 * @author David Schoosleitner (github.com/Nievillis/GrowTheCharacter)
 * @brief Button object file
 * @version 0.3
 * @date 2022-11-23
 * 
 * MIT License
 * 
 * @copyright Copyright (c) 2022 David Schoosleitner
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

/*******
 * INCLUDES
 **/

#include "CharObj.hpp"
#include "Utils.hpp"
#include "../ColorExtensions.hpp"

/*******
 * OBJECT DEFINITION
 **/

/**
 * @class rbuttonobj : public rcharobj
 * @brief Button object, inherits from rcharobj
 */
class rbuttonobj : public rcharobj
{
protected:
    const static char c_char_unpressed = 'O';
    const static char c_char_pressed = '_';
    const Pixel::Color c_col_pressed = Pixel::Extensions::GRAY;

    int m_press_ID = 64;        //< This will be called upon button press (64 for no effect)
    int m_release_ID = 64;      //< This will be called upon button release (64 for no effect)
    int m_tick_delay = 0;       //< How long the button will be pressed for (-1 for permanent press)
    int m_tick_start = 0;       //< Internal handle: When the button was pressed
    bool m_pressed = false;     //< If button is pressed

public:

    /**
     * @fn rbuttonobj::rbuttonobj(const int _x, const int _y, const int _tick_delay, const int _press_ID = 64u, const int _release_ID = 64u)
     * @brief rbuttonobj Constructor 1:
     * @brief rcharobj(_x, _y, c_char_unpressed, Pixel::WHITE) will be invoked ; 
     * @brief m_press_ID <= _press_ID ; m_release_ID <= release_ID ; m_tick_delay <= _tick_delay
     * 
     * @param _x: (Horizontal) X-coordinate of object [0..room size width]
     * @param _y: (Vertical) Y-coordinate of object [0..room size height]
     * @param _tick_delay: How long the button will be pressed for [-1: Forever, 1..*: in ticks]
     * @param _press_ID: Trigger ID to be called upon button press [0..63: Room trigger, 64: None, 65..128: Level trigger]
     * @param _release_ID: Trigger ID to be called upon button release [0..63: Room trigger, 64: None, 65..128: Level trigger]
     */
    rbuttonobj(const int _x, const int _y, const int _tick_delay, const int _press_ID = 64u, const int _release_ID = 64u) :
        rcharobj(_x, _y, c_char_unpressed, Pixel::WHITE),
        m_press_ID(_press_ID), m_release_ID(_release_ID), m_tick_delay(_tick_delay)
    {}

    /**
     * @fn rbuttonobj::rbuttonobj(const int _x, const int _y, const int _tick_delay, layer_e _layer, const int _press_ID = 64u, const int _release_ID = 64u)
     * @brief rbuttonobj Constructor 2:
     * @brief rcharobj(_x, _y, c_char_unpressed, Pixel::WHITE, _layer) will be invoked ;
     * @brief m_press_ID <= _press_ID ; m_release_ID <= release_ID ; m_tick_delay <= _tick_delay
     * 
     * @param _x: (Horizontal) X-coordinate of object [0..room size width]
     * @param _y: (Vertical) Y-coordinate of object [0..room size height]
     * @param _tick_delay: How long the button will be pressed for [-1: Forever, 1..*: in ticks]
     * @param _layer: Rendering layer [robj::B2,B1,D,F1,F2,F3,F4 ; default:D]
     * @param _press_ID: Trigger ID to be called upon button press [0..63: Room trigger, 64: None, 65..128: Level trigger]
     * @param _release_ID: Trigger ID to be called upon button release [0..63: Room trigger, 64: None, 65..128: Level trigger]
     */
    rbuttonobj(const int _x, const int _y, const int _tick_delay, layer_e _layer, const int _press_ID = 64u, const int _release_ID = 64u) :
        rcharobj(_x, _y, c_char_unpressed, Pixel::WHITE, _layer),
        m_press_ID(_press_ID), m_release_ID(_release_ID), m_tick_delay(_tick_delay)
    {}

    /**
     * @fn rbuttonobj::rbuttonobj(const int _x, const int _y, const int _tick_delay, const bool _pressed, const int _press_ID = 64u, const int _release_ID = 64u)
     * @brief rbuttonobj Constructor 3:
     * @brief rcharobj(_x, _y, c_char_unpressed, Pixel::WHITE) will be invoked ; 
     * @brief m_press_ID <= _press_ID ; m_release_ID <= _release_ID ; m_tick_delay <= tick_delay ; m_pressed <= _pressed
     * 
     * @param _x: (Horizontal) X-coordinate of object [0..room size width]
     * @param _y: (Vertical) Y-coordinate of object [0..room size height]
     * @param _tick_delay: How long the button will be pressed for [-1: Forever, 1..*: in ticks]
     * @param _pressed: If the button is pressed from the start [false,true]
     * @param _press_ID: Trigger ID to be called upon button press [0..63: Room trigger, 64: None, 65..128: Level trigger]
     * @param _release_ID: Trigger ID to be called upon button release [0..63: Room trigger, 64: None, 65..128: Level trigger] 
     */
    rbuttonobj(const int _x, const int _y, const int _tick_delay, const bool _pressed, const int _press_ID = 64u, const int _release_ID = 64u) :
        rcharobj(_x, _y, c_char_unpressed, Pixel::WHITE),
        m_press_ID(_press_ID), m_release_ID(_release_ID), m_tick_delay(_tick_delay), m_pressed(_pressed)
    {}

    /**
     * @fn rbuttonobj::rbuttonobj(const int _x, const int _y, const int _tick_delay, const bool _pressed, layer_e _layer, const int _press_ID = 64u, const int _release_ID = 64u)
     * @brief Construct a new rbuttonobj object
     * 
     * @param _x: (Horizontal) X-coordinate of object [0..room size width]
     * @param _y: (Vertical) Y-coordinate of object [0..room size height]
     * @param _tick_delay: How long the button will be pressed for [-1: Forever, 1..*: in ticks]
     * @param _pressed: If the button is pressed from the start [false,true]
     * @param _layer: Rendering layer [robj::B2,B1,D,F1,F2,F3,F4 ; default:D]
     * @param _press_ID: Trigger ID to be called upon button press [0..63: Room trigger, 64: None, 65..128: Level trigger]
     * @param _release_ID: Trigger ID to be called upon button release [0..63: Room trigger, 64: None, 65..128: Level trigger] 
     */
    rbuttonobj(const int _x, const int _y, const int _tick_delay, const bool _pressed, layer_e _layer, const int _press_ID = 64u, const int _release_ID = 64u) :
        rcharobj(_x, _y, c_char_unpressed, Pixel::WHITE, _layer),
        m_press_ID(_press_ID), m_release_ID(_release_ID), m_tick_delay(_tick_delay), m_pressed(_pressed)
    {}

    /**
     * @fn inline virtual pixelstr rbuttonobj::draw(unsigned long long tick) override
     * @brief Overriden function of robj::draw ; Draws button depending if it is pressed or not
     * 
     * @param tick: Current physics tick
     * @return pixelstr: Render output
     */
    inline virtual pixelstr draw(unsigned long long tick) override
    {
        if (m_pressed)
            return {Pixel::create_pixel(c_char_pressed, c_col_pressed)};
        else
            return {Pixel::create_pixel(m_character, col)};
    }

    /**
     * @fn inline virtual void rbuttonobj::physics(unsigned long long tick) override
     * @brief Overriden function of robj::physics ; Only unpresses the button. Pressing is handled by Player.cpp
     * 
     * @param tick: Current physics tick
     */
    inline virtual void physics(unsigned long long tick) override
    {
        if(m_tick_delay != -1 && m_pressed && tick > m_tick_start + m_tick_delay)
        {
            m_pressed = false;
            Utils::do_trig(m_release_ID);
        }
    }

    /**
     * @fn inline void press(unsigned long long tick)
     * @brief Press the button at a tick
     * 
     * @param tick: Current physics tick
     */
    inline void press(unsigned long long tick)
    {
        if(!m_pressed)
        {
            m_tick_start = tick;
            m_pressed = true;
            Utils::do_trig(m_press_ID);
        }
    }
};
