/**
 * @file Pixel.hpp
 * @author David Schoosleitner (nievillis@github.com)
 * @brief Contains definitions of pixel character and pixel string
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

#define ANSI_DEFAULT_STREAM std::cout
#define ANSI_DEFAULT_FLUSH std::flush
#include "ANSICONSTANTS"

#include <iostream>
#include <cstring>
#include <functional>

//! Bit Structure FFFO OOOT CCCC CCCC 
typedef __UINT16_TYPE__ Pixel_t;
typedef std::basic_string<Pixel_t> pixelstr;

constexpr Pixel_t operator "" us(unsigned long long val)
{
    return (Pixel_t) val;
}

constexpr __UINT8_TYPE__ operator "" uc(unsigned long long val)
{
    return (__UINT8_TYPE__) val;
}

namespace Pixel
{
    struct ColorType
    {
    private:
        __UINT8_TYPE__ signature;
    
    public:
        ColorType(const __UINT8_TYPE__ _signature): signature(_signature) {}
        ColorType(const ColorType &ct): signature(ct.signature) {}
        inline int operator<<(const int shift) const {return ((int) signature) << shift;}
        inline ColorType operator=(const ColorType &ct) {this->signature = ct.signature; return *this;}
        inline bool operator==(const ColorType &ct) {return this->signature == ct.signature;}
        inline bool operator!=(const ColorType &ct) {return this->signature != ct.signature;}

    } const TEXT(0), BACKGROUND(1);

    struct Color
    {
    private:
        __UINT8_TYPE__ signature;
        std::string linked_tcontrols;
        std::string linked_bcontrols;
    
    public:
        Color(const __UINT8_TYPE__ _signature, const std::string _linked_tcontrols, const std::string _linked_bcontrols): signature(_signature), linked_tcontrols(_linked_tcontrols), linked_bcontrols(_linked_bcontrols) {}
        Color(const Color &ct): signature(ct.signature), linked_tcontrols(ct.linked_tcontrols), linked_bcontrols(ct.linked_bcontrols) {}
        inline int operator<<(const int shift) const {return ((int) signature) << shift;}
        inline Color &operator=(const Color &ct) {this->signature = ct.signature; this->linked_tcontrols = ct.linked_tcontrols; this->linked_bcontrols = ct.linked_bcontrols; return *this;}
        inline bool operator==(const Color &ct) {return this->signature == ct.signature;}
        inline bool operator!=(const Color &ct) {return this->signature != ct.signature;}
        inline std::string get_tcontrols() const {return this->linked_tcontrols;}
        inline std::string get_bcontrols() const {return this->linked_bcontrols;}
        inline int getsign() const {return this->signature;}
    }
        const BLACK(      0,  COLORT_BLACK,   COLORB_BLACK),
        RED(        1,  COLORT_RED,     COLORB_RED),
        GREEN(      2,  COLORT_GREEN,   COLORB_GREEN),
        YELLOW(     3,  COLORT_YELLOW,  COLORB_YELLOW),
        BLUE(       4,  COLORT_BLUE,    COLORB_BLUE),
        MAGENTA(    5,  COLORT_MAGENTA, COLORB_MAGENTA),
        CYAN(       6,  COLORT_CYAN,    COLORB_CYAN),
        WHITE(      7,  COLORT_WHITE,   COLORB_WHITE),
        DEFAULT(    8,  COLORT_DEFAULT, COLORB_DEFAULT)
    ;

    struct Font
    {
    private:
        __UINT8_TYPE__ signature;
    
    public:
        Font(const __UINT8_TYPE__ _signature): signature(_signature) {}
        Font(const Font &ct): signature(ct.signature){}
        inline int operator<<(const int shift) const {return ((int) signature) << shift;}
        inline Font &operator=(const Font &ct) {this->signature = ct.signature; return *this;}
        inline bool operator==(const Font &ct) const {return this->signature == ct.signature;}
        inline bool operator==(const int ct) const {return this->signature == ct;}
        inline bool operator!=(const Font &ct) const {return this->signature != ct.signature;}
        inline int operator&(const Font &ct) const {return this->signature & ct.signature;}

    }
        const NORMAL(       0   ),
        BOLD(               1   ),
        UNDERLINED(         2   ),
        BOLD_UNDERLINED(    3   ),
        INVERSE(            4   ),
        INVERSE_BOLD(       5   ),
        INVERSE_UNDERLINED( 6   ),
        ALL(                7   )
    ;

    typedef const std::function<void(const size_t, char &, ColorType &, Color &, Font &)> pixel_lambda; 

    inline static const Color &int_to_color(const __UINT8_TYPE__ val)
    {
        switch(val)
        {
            case 0u:
                return BLACK;
            case 1u:
                return RED;
            case 2u:
                return GREEN;
            case 3u:
                return YELLOW;
            case 4u:
                return BLUE;
            case 5u:
                return MAGENTA;
            case 6u:
                return CYAN;
            case 7u:
                return WHITE;
        }
        return DEFAULT;
    }

    inline static const Font &int_to_font(const __UINT8_TYPE__ val)
    {
        switch(val)
        {
            case 1u:
                return BOLD;
            case 2u:
                return UNDERLINED;
            case 3u:
                return BOLD_UNDERLINED;
            case 4u:
                return INVERSE;
            case 5u:
                return INVERSE_BOLD;
            case 6u:
                return INVERSE_UNDERLINED;
            case 7u:
                return ALL;
        }
        return NORMAL;
    }
    
    inline static Pixel_t create_pixel(const unsigned char character, const ColorType &color_type, const Color &color, const Font &font)
    {
        return (character > 0uc) ? (character | (color_type << 8) | (color << 9) | (font << 13)) : 0us;
    }

    inline static Pixel_t create_pixel(const unsigned char character)
    {
        return (character > 0uc) ? (character | (TEXT << 8) | (DEFAULT << 9) | (NORMAL << 13)) : 0us;
    }

    inline static Pixel_t create_pixel(const unsigned char character, const Color &color, const ColorType &color_type = TEXT)
    {
        return (character > 0uc) ? (character | (color_type << 8) | (color << 9) | (NORMAL << 13)) : 0us;
    }

    inline static Pixel_t create_pixel(const unsigned char character, const Color &color, const Font &font)
    {
        return (character > 0uc) ? (character | (TEXT << 8) | (color << 9) | (font << 13)) : 0us;
    }

    inline static Pixel_t create_pixel(const unsigned char character, const Font &font)
    {
        return (character > 0uc) ? (character | (TEXT << 8) | (DEFAULT << 9) | (font << 13)) : 0us;
    }

    inline static Color get_pixel_color(const Pixel_t pixel)
    {
        return int_to_color(((__UINT8_TYPE__)(pixel >> 9)) & 0x0Fuc);
    }

    inline static void set_pixel_color(Pixel_t &pixel, const Color &color)
    {
        pixel = (pixel & 0xE1FFus) | ((color << 9) & 0x1E00us);
    }

    inline static ColorType get_pixel_color_type(const Pixel_t pixel)
    {
        return ((((__UINT8_TYPE__)(pixel >> 8)) & 0x01uc) > 0x00uc) ? BACKGROUND : TEXT;
    }

    inline static void set_pixel_color_type(Pixel_t &pixel, const ColorType &color_type)
    {
        pixel = (pixel & 0xFEFFus) | ((color_type << 8) & 0x0100us);
    }

    inline static unsigned char get_pixel_char(const Pixel_t pixel)
    {
        return (unsigned char)(pixel & 0x00FFus);
    }

    inline static void set_pixel_char(Pixel_t &pixel, unsigned char character)
    {
        pixel = (pixel & 0xFF00us) | (character & 0x00FFus);
    }

    inline static const bool get_is_normal_font(const Pixel_t pixel)
    {
        return NORMAL == ((__UINT8_TYPE__)(pixel >> 13) & 0x7uc);
    }

    inline static void set_normal_font(Pixel_t &pixel)
    {
        pixel &= 0x1FFFus;
    }

    inline static const bool get_is_bold_font(const Pixel_t pixel)
    {
        return (BOLD & ((__UINT8_TYPE__)(pixel >> 13) & 0x7uc)) > 0uc;
    }

    inline static void set_use_bold_font(Pixel_t &pixel, const bool b = true)
    {
        pixel = b ? (pixel | (BOLD << 13)) : (pixel & ~(BOLD << 13));
    }

    inline static const bool get_is_underlined_font(const Pixel_t pixel)
    {
        return (UNDERLINED & ((__UINT8_TYPE__)(pixel >> 13) & 0x7uc)) > 0uc;
    }

    inline static void set_use_underlined_font(Pixel_t &pixel, const bool b = true)
    {
        pixel = b ? (pixel | (UNDERLINED << 13)) : (pixel & ~(UNDERLINED << 13));
    }

    inline static const bool get_is_inverse_font(const Pixel_t pixel)
    {
        return (INVERSE & ((__UINT8_TYPE__)(pixel >> 13) & 0x7uc)) > 0uc;
    }

    inline static void set_use_inverse_font(Pixel_t &pixel, const bool b = true)
    {
        pixel = b ? (pixel | (INVERSE << 13)) : (pixel & ~(INVERSE << 13));
    }

    inline static Font get_pixel_font(const Pixel_t &pixel)
    {
        return int_to_font((pixel >> 13) & 0x7us);
    }

    inline static void set_pixel_font(Pixel_t &pixel, const Font &font)
    {
        pixel = (font << 13) | (pixel & 0x1FFFus);
    }

    inline static std::string to_string(const Pixel_t &pixel)
    {
        std::string ret = "";
        static Color old_color = DEFAULT;
        static ColorType old_type = TEXT;
        static Font old_font = NORMAL;
        Color now_color = get_pixel_color(pixel);
        Font now_font = get_pixel_font(pixel);

        if(old_color != now_color && get_pixel_color_type(pixel) == TEXT)
        {
            ret += now_color.get_tcontrols();
        }
        else if(old_color != now_color)
        {
            ret += now_color.get_bcontrols();
        }

        if(old_font != now_font)
        {
            if(now_font == NORMAL)
            {
                ret += COLOR_FONT_RESET;
            }
            else
            {
                if((now_font & BOLD) != (old_font & BOLD))
                {
                    ret += ((now_font & BOLD) > 0uc) ? COLOR_FONT_BOLD : COLOR_FONT_NOT_BOLD;
                }
                if((now_font & UNDERLINED) != (old_font & UNDERLINED))
                {
                    ret += ((now_font & BOLD) > 0uc) ? COLOR_FONT_UNDERLINED : COLOR_FONT_NOT_UNDERLINED;
                }
                if((now_font & INVERSE) != (old_font & INVERSE))
                {
                    ret += ((now_font & BOLD) > 0uc) ? COLOR_FONT_INVERSE : COLOR_FONT_NOT_INVERSE;
                }
            }
        }

        old_color = now_color;
        old_font = now_font;
        
        ret += get_pixel_char(pixel);
        return ret;
    }

    inline static void reset_cl()
    {
        (void) to_string(0x1000us);
    }

    inline static const size_t get_pixel_string_length(const Pixel_t *pixel)
    {
        size_t i = 0;
        for(; pixel[i] & 0x00FFus > 0u; i++);
        return i;
    }

    inline static std::string to_string(const Pixel_t *pixels)
    {
        std::string ret = "";
        const size_t max = get_pixel_string_length(pixels);
        for(size_t i = 0; (pixels[i] & 0x00FFus) > 0u; i++)
        {
            ret += to_string(pixels[i]);
        }
        return ret;
    }

    inline static std::string to_string(const pixelstr ps)
    {
        return to_string(ps.c_str());
    }

    inline static void copy_string_to_pixel_string(const size_t max_size, Pixel_t *pixel_dest, const char *cstr_source,
        pixel_lambda lambda_function
    ) {
        for(size_t i = 0; i < max_size; i++)
        {
            char character = (char) cstr_source[i];
            ColorType color_type = TEXT;
            Color color = DEFAULT;
            Font font = NORMAL;
            lambda_function(i, character, color_type, color, font);
            pixel_dest[i] = Pixel::create_pixel(character, color, color_type);
        }
    }

    inline static void copy_string_to_pixel_string(const size_t max_size, Pixel_t *pixel_dest, const char *cstr_source)
    {
        for(size_t i = 0; i < max_size; i++)
        {
            pixel_dest[i] = Pixel::create_pixel(cstr_source[i]);
        }
    }

    inline static void copy_string_to_pixel_string(Pixel_t *pixel_dest, const std::string str_source,
        pixel_lambda lambda_function
    ) {
        size_t i = 0;
        for(char c : str_source)
        {
            char character = c;
            ColorType color_type = TEXT;
            Color color = DEFAULT;
            Font font = NORMAL;
            lambda_function(i, character, color_type, color, font);
            pixel_dest[i++] = Pixel::create_pixel(character, color_type, color, font);
        }
    }

    inline static void copy_string_to_pixel_string(Pixel_t *pixel_dest, const std::string str_source)
    {
        size_t i = 0;
        for(char c : str_source)
        {
            pixel_dest[i++] = Pixel::create_pixel(c);
        }
    }

    inline static void copy_string_to_pixel_string(const size_t max_size, pixelstr &pixel_dest, const char *cstr_source,
        pixel_lambda lambda_function
    ) {
        pixel_dest.clear();
        for(size_t i = 0; i < max_size; i++)
        {
            char character = (char) cstr_source[i];
            ColorType color_type = TEXT;
            Color color = DEFAULT;
            Font font = NORMAL;
            lambda_function(i, character, color_type, color, font);
            pixel_dest += Pixel::create_pixel(character, color_type, color, font);
        }
    }

    inline static void copy_string_to_pixel_string(const size_t max_size, pixelstr &pixel_dest, const char *cstr_source)
    {
        pixel_dest.clear();
        for(size_t i = 0; i < max_size; i++)
        {
            pixel_dest += Pixel::create_pixel((char) cstr_source[i]);
        }
    }

    inline static void copy_string_to_pixel_string(pixelstr &pixel_dest, const std::string str_source,
        pixel_lambda lambda_function
    ) {
        pixel_dest.clear();
        size_t i = 0;
        for(char c : str_source)
        {
            char character = c;
            ColorType color_type = TEXT;
            Color color = DEFAULT;
            Font font = NORMAL;
            lambda_function(i++, character, color_type, color, font);
            pixel_dest += Pixel::create_pixel(character, color_type, color, font);
        }
    }

    inline static void copy_string_to_pixel_string(pixelstr &pixel_dest, const std::string str_source)
    {
        pixel_dest.clear();
        for(char c : str_source)
        {
            pixel_dest += Pixel::create_pixel(c);
        }
    }

    inline static void copy_string_to_pixel_string_par(pixelstr &pixel_dest, const std::string str_source, pixel_lambda lambda_function)
    {
        pixel_dest.clear();
        const size_t size = str_source.size();
        #pragma omp parallel for
        for(size_t i = 0; i < size; i++)
        {
            char character = str_source.at(i);
            ColorType color_type = TEXT;
            Color color = DEFAULT;
            Font font = NORMAL;
            lambda_function(i, character, color_type, color, font);
            pixel_dest += Pixel::create_pixel(character, color_type, color, font);
        }
    }

    inline static void for_each(pixelstr &pixel_dest, pixel_lambda lambda_function)
    {
        size_t i = 0;

        for(Pixel_t &p : pixel_dest)
        {
            char character = get_pixel_char(p);
            ColorType color_type = get_pixel_color_type(p);
            Color color = get_pixel_color(p);
            Font font = get_pixel_font(p);
            lambda_function(i++, character, color_type, color, font);
            p = create_pixel(character, color_type, color, font);
        }
    }

    inline static void for_each_par(pixelstr &pixel_dest, pixel_lambda lambda_function)
    {
        size_t i = 0;
        size_t max = pixel_dest.size();

        #pragma omp parallel for
        for(size_t i = 0; i < max; i++)
        {
            Pixel_t &p = pixel_dest.at(i);
            char character = get_pixel_char(p);
            ColorType color_type = get_pixel_color_type(p);
            Color color = get_pixel_color(p);
            Font font = get_pixel_font(p);
            lambda_function(i++, character, color_type, color, font);
            p = create_pixel(character, color_type, color, font);
        }
    }

    inline static void for_each(size_t size, Pixel_t *pixel_dest, pixel_lambda lambda_function)
    {
        for(size_t i = 0; i < size; i++)
        {
            char character = get_pixel_char(pixel_dest[i]);
            ColorType color_type = get_pixel_color_type(pixel_dest[i]);
            Color color = get_pixel_color(pixel_dest[i]);
            Font font = get_pixel_font(pixel_dest[i]);
            lambda_function(i, character, color_type, color, font);
            pixel_dest[i] = create_pixel(character, color_type, color, font);
        }
    }
};

inline static std::ostream &operator<<(std::ostream &o, pixelstr ps)
{
    return o << Pixel::to_string(ps);
}

inline static pixelstr operator "" ps(const char *c, size_t size)
{
    pixelstr ps;
    for(size_t i = 0ULL; i < size; i++)
        ps += Pixel::create_pixel(c[i]);
    return ps;
}
