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
    pixelstr m_content;
    std::mutex m_mutex;
    std::ostream *m_stream;
    int m_start_x = 1;
    int m_start_y = 1;
    
public:
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
        pixelstr ps;
        Pixel::copy_string_to_pixel_string(ps, std::to_string(obj));

        _CE_lexpr m_content += ps;
        _CE_uexpr return *this;
    }

    inline ConsoleEngine &operator<<(const pixelstr &obj)
    {
        _CE_lexpr m_content += obj;
        _CE_uexpr return *this;
    }

    inline ConsoleEngine &operator<<(const char *c)
    {
        pixelstr ps;
        Pixel::copy_string_to_pixel_string(ps, c);

        _CE_lexpr m_content += ps;
        _CE_uexpr return *this;
    }

    inline ConsoleEngine &add_string(const std::string &str,
        const Pixel::Color color = Pixel::DEFAULT,
        const Pixel::ColorType color_type = Pixel::TEXT,
        const Pixel::Font font = Pixel::NORMAL
    ) {
        pixelstr ps;
        Pixel::copy_string_to_pixel_string_par(ps, str,
            [color_type, color, font](const size_t _index, char &_ch, Pixel::ColorType &_type, Pixel::Color &_col, Pixel::Font &_font)
            {
                _type = color_type;
                _col = color;
                _font = font;
            }
        );

        _CE_lexpr m_content += ps;
        _CE_uexpr return *this;
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

    friend inline std::ostream &operator<<(std::ostream &o, ConsoleEngine &ce)
    {
        ce.erase_screen();
        return o << ce.get_cursor_string() << ce.sync() << COLOR_RESET << std::endl;
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
