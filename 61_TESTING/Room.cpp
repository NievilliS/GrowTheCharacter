#include "Room.hpp"

room::room():
    background_color(Pixel::DEFAULT),
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
        ConsoleEngine_context << (*i)->get_coord_str() << (*i)->draw();
    }

    ConsoleEngine_context << ConsoleEngine::PRINTOUT;
}

v2 room::get_pixelstr_dim(const pixelstr &bdat)
{
    v2 cr;
    v2 wh{1,0};
    const size_t pss = bdat.size();

    for(size_t i = 0; i < pss; i++)
    {
        cr.hori++;
        if(bdat[i] == '\n')
        {
            cr.hori = 0;
            cr.vert++;
        }

        if(cr.hori + 1 >= wh.hori) wh.hori++;
        if(cr.vert + 1 >= wh.vert) wh.vert++;
    }

    return wh;
}
