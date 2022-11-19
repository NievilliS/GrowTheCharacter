#include "Room.hpp"
#include "ColorExtensions.hpp"

room::room():
    background_color(Pixel::DEFAULT),
    foreground_color(Pixel::DEFAULT),
    size{1,1}
{
}

room &room::operator+(robj *obj)
{
    this->add_obj(obj);
    return *this;
}

void room::set_base_dat_str(pixelstr &bdat)
{
    this->base_dat = bdat;
    this->size = get_pixelstr_dim(this->base_dat);

    Pixel::for_each(this->base_dat, [this](const size_t a, char &b, Pixel::ColorType &c, Pixel::Color &d, Pixel::Font &e){
        if(d == Pixel::DEFAULT)
        {
            c = Pixel::TEXT;
            d = this->foreground_color;
        }
    });
}

void room::add_obj(robj *obj)
{
    this->objs.push_back(std::unique_ptr<robj>(obj));
}

void room::draw()
{
    ConsoleEngine_context << ConsoleEngine::CLEAR <<
        this->background_color.get_bcontrols() << "";
    
    ConsoleEngine_context << this->base_dat;

    for(auto i = this->objs.begin(); i != objs.end(); i++)
    {
        ConsoleEngine_context << (*i)->get_coord_str() << (*i)->draw(tick);
    }

    ConsoleEngine_context << ConsoleEngine::PRINTOUT;
}

void room::physics()
{
    
    tick++;
}

v2 room::get_pixelstr_dim(const pixelstr &bdat)
{
    int w;
    v2 wh{0,0};
    const size_t pss = bdat.size();

    for(size_t i = 0; i < pss; i++)
    {
        if(wh.vert <= 0) wh.vert = 1;
        
        if(Pixel::get_pixel_char(bdat[i]) == '\n')
        {
            w = 0;
            wh.vert++;
            continue;
        }
        w++;
        if(w > wh.hori) wh.hori++;
    }

    return wh;
}

void room::set_background_color(const Pixel::Color &bgr)
{
    this->background_color = bgr;
}

void room::set_foreground_color(const Pixel::Color &bgr)
{
    this->foreground_color = bgr;
}
