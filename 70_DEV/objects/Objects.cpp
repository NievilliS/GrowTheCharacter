#include <consoleengine/ConsoleEngine.hpp>
#include "BgrObj.hpp"

/*****************************
 * BGR OBJ
 *********/

rbgrobj::rbgrobj(const int x, const int y, const Pixel::Color col) :
    rcharobj(x, y, ' ', col, B1),
    m_comb{Pixel::create_pixel(' ',col,Pixel::BACKGROUND)}
{}

rbgrobj::rbgrobj(std::vector<rbgrobj*> &&vec, Pixel::Color end_col)
{
    v2 start, end;
    start = (*vec.begin())->m_primary_coords;
    end = start;
    m_primary_coords = start;

    for(auto i = ++vec.begin(); i != vec.end(); i++)
    {
        v2 c = (*i)->m_primary_coords;
        start.vert = ((c.vert < start.vert) ? c.vert : start.vert);
        start.hori = ((c.hori < start.hori) ? c.hori : start.hori);
        end.vert = ((c.vert > end.vert) ? c.vert : end.vert);
        end.hori = ((c.hori > end.hori) ? c.hori : end.hori);
        m_primary_coords = (c.vert < m_primary_coords.vert || c.vert == m_primary_coords.vert && c.hori < m_primary_coords.hori) ? c : m_primary_coords;
    }

    m_size = v2{end.vert - start.vert + 1, end.hori - start.hori + 1};
    std::vector<Pixel_t> dat{(std::size_t) (m_size.hori * m_size.vert), std::allocator<Pixel_t>{}};

    for(auto i = vec.begin(); i != vec.end(); i++)
    {
        v2 c = (*i)->m_primary_coords - start;
        dat[c.hori + c.vert * m_size.hori] = Pixel::create_pixel((*i)->m_character, (*i)->col, Pixel::BACKGROUND);
    }

    bool flag_a = true;
    bool flag_b = false;
    int index = 0;
    for(auto i = dat.begin(); i != dat.end(); i++)
    {
        if(!*i)
            flag_b = true;
        else if(flag_a)
        {
            flag_a = false;
            m_comb += *i;
        }
        else
        {
            if(flag_b || !(index % m_size.hori))
            {
                flag_b = false;
                std::string s = CSI + std::to_string(1 + start.vert + (index / m_size.hori)) + ';' + std::to_string(1 + start.hori + (index % m_size.hori)) + 'H';
                pixelstr ps{};
                Pixel::copy_string_to_pixel_string(ps, s);
                m_comb += ps;
            }
            m_comb += *i;
        }
        index++;
    }

    pixelstr p_idler{};
    std::string s_idler = CURSOR_MOVE_TO(1,1);
    Pixel::copy_string_to_pixel_string(p_idler, s_idler, [&end_col](const size_t index, char &c, Pixel::ColorType &t, Pixel::Color &o, Pixel::Font &f){
        t = Pixel::BACKGROUND;
        o = end_col;
    });
    m_comb += p_idler;
}

/*****************************
 * BGR OBJ
 *********/