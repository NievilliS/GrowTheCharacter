/**
 * @file ConsoleEngine.hpp
 * @author David Schoosleitner (nievillis@github.com)
 * @brief Contains ConsoleEngine functionality
 * @version 0.1
 * @date 2022-11-13
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

#include <iostream>
#include <mutex>
#include "Pixel.hpp"

#define _CE_lexpr m_mutex.lock();
#define _CE_uexpr m_mutex.unlock();

class ConsoleEngine
{
protected:
    pixelstr m_content{};
    std::mutex m_mutex;
    std::ostream *m_stream;
    int m_start_x = 1;
    int m_start_y = 1;
    Pixel::Color m_active_color = Pixel::DEFAULT;
    Pixel::ColorType m_active_type = Pixel::TEXT;
    Pixel::Font m_active_font = Pixel::NORMAL;
    
public:
    enum controls
    {
        CLEAR,
        ERASE_SCREEN,
        PRINTOUT
    };

    ConsoleEngine(std::ostream &stream)
    {
        m_stream = &stream;
    }

    ConsoleEngine(): ConsoleEngine(std::cout)
    {
    }

    ConsoleEngine(std::ostream &stream, const int start_x, const int start_y): m_start_x(start_x), m_start_y(start_y)
    {
        m_stream = &stream;
    }

    ~ConsoleEngine()
    {
        _CE_uexpr *m_stream << COLOR_RESET << std::flush;
    }

    template<typename T>
    inline ConsoleEngine &operator<<(const T &obj)
    {
        return this->add_string(std::to_string(obj));
    }

    inline ConsoleEngine &operator<<(const pixelstr &obj)
    {
        _CE_lexpr m_content += obj;
        _CE_uexpr return *this;
    }

    inline ConsoleEngine &operator<<(const std::string &obj)
    {
        return this->add_string(obj);
    }

    inline ConsoleEngine &operator<<(const char *c)
    {
        return this->add_string(std::string(c));
    }

    inline ConsoleEngine &operator<<(const controls &&c)
    {
        switch(c)
        {
            case CLEAR:
                this->clear();
                break;
            case ERASE_SCREEN:
                this->erase_screen();
                break;
            case PRINTOUT:
                this->print_to_stream();
                break;
        }
        return *this;
    }

    inline ConsoleEngine &operator<<(const Pixel::Color &col)
    {
        _CE_lexpr m_active_color = col;
        _CE_uexpr return *this;
    }

    inline ConsoleEngine &operator<<(const Pixel::ColorType &type)
    {
        _CE_lexpr m_active_type = type;
        _CE_uexpr return *this;
    }

    inline ConsoleEngine &operator<<(const Pixel::Font &font)
    {
        _CE_lexpr m_active_font = font;
        _CE_uexpr return *this;
    }

    inline ConsoleEngine &add_string(const std::string &str,
        const Pixel::Color &color,
        const Pixel::ColorType &color_type = Pixel::TEXT,
        const Pixel::Font &font = Pixel::NORMAL
    ) {
        pixelstr ps{};
        Pixel::copy_string_to_pixel_string_par(ps, str,
            [&](const size_t _index, char &_ch, Pixel::ColorType &_type, Pixel::Color &_col, Pixel::Font &_font)
            {
                _type = color_type;
                _col = color;
                _font = font;
            }
        );

        _CE_lexpr m_content += ps;
        _CE_uexpr return *this;
    }

    inline ConsoleEngine &add_string(const std::string &str)
    {
        return add_string(str, m_active_color, m_active_type, m_active_font);
    }

    inline pixelstr sync()
    {
        _CE_lexpr pixelstr ps = this->m_content;
        this->m_content.clear();
        _CE_uexpr return ps;
    }

    inline void clear()
    {
        _CE_lexpr this->m_content.clear();
        Pixel::reset_cl();
        _CE_uexpr
    }

    inline std::string get_cursor_string()
    {
        return std::string(CSI) + std::to_string(this->m_start_y) + ';' + std::to_string(this->m_start_x) + 'H';
    }

    inline void erase_screen()
    {
        *m_stream << ERASE_DISPLAY << std::flush;
    }

    inline void print_to_stream()
    {
        (*m_stream) << *this;
    }

    inline void getxy(int *y, int *x)
    {
        *y = m_start_y;
        *x = m_start_x;
    }

    inline void setxy(int y, int x)
    {
        m_start_y = y;
        m_start_x = x;
    }

    friend inline std::ostream &operator<<(std::ostream &o, ConsoleEngine &ce)
    {
        ce.erase_screen();
        return o << ce.get_cursor_string() << ce.sync() << COLOR_RESET << std::flush;
    }

private:

    inline ConsoleEngine &operator=(ConsoleEngine &ce)
    {
        this->m_content = ce.m_content;
        this->m_start_x = ce.m_start_x;
        this->m_start_y = ce.m_start_y;
        this->m_stream = ce.m_stream;
        return *this;
    }

public:
    inline static void _set_context(ConsoleEngine &ce);
};

inline extern ConsoleEngine ConsoleEngine_context{};
void ConsoleEngine::_set_context(ConsoleEngine &ce) {
    ConsoleEngine_context = ce;
}

#undef _CE_lexpr
#undef _CE_uexpr
