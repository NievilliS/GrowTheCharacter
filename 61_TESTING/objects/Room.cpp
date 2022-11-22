#include "Room.hpp"
#include "../ColorExtensions.hpp"

room::room(const int _index):
    background_color(Pixel::DEFAULT),
    foreground_color(Pixel::DEFAULT),
    size{1,1},
    index(_index)
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
    this->objs.push_back(obj);
}

void room::draw(unsigned long long tick)
{
    debug = COLORT_BLACK "Draw last took: " + std::to_string(x) + "\nPhys last took: " + std::to_string(y);

    ConsoleEngine_context << ConsoleEngine::CLEAR <<
        this->background_color.get_bcontrols() << "";
    
    ConsoleEngine_context << this->base_dat << "\n\n" << debug;

    pixelstr str_B2, str_B1, str_D, str_F1, str_F2, str_F3, str_F4;

    for(auto i = this->objs.begin(); i != objs.end(); i++)
    {
        if((*i)->coords() >= v2{0,0} && (*i)->coords() < this->size)
            switch((*i)->layer())
            {
                case robj::B2:
                    str_B2 += (*i)->get_coord_str() + (*i)->draw(tick);
                case robj::B1:
                    str_B1 += (*i)->get_coord_str() + (*i)->draw(tick);
                case robj::F1:
                    str_F1 += (*i)->get_coord_str() + (*i)->draw(tick);
                case robj::F2:
                    str_F2 += (*i)->get_coord_str() + (*i)->draw(tick);
                case robj::F3:
                    str_F3 += (*i)->get_coord_str() + (*i)->draw(tick);
                case robj::F4:
                    str_F4 += (*i)->get_coord_str() + (*i)->draw(tick);
                case robj::D:
                default:
                    str_D += (*i)->get_coord_str() + (*i)->draw(tick);
            }
    }

    ConsoleEngine_context <<
        str_B2 <<
        str_B1 <<
        str_D <<
        str_F1 <<
        str_F2 <<
        str_F3 <<
        str_F4 <<
        CURSOR_MOVE_TO(1,1) <<
        ConsoleEngine::PRINTOUT;
}

void room::physics(unsigned long long tick)
{
    this->next_trigger = 0ULL;

    //!! Event queue handle
    {
        std::vector<std::vector<reventqueue*>::iterator> indices;
        for(auto i = this->event_queue.begin(); i != event_queue.end(); i++)
        {
            if((*i)->check_run(tick))
                indices.push_back(i);
        }

        for(auto i = indices.begin(); i != indices.end(); i++)
        {
            this->event_queue.erase(*i);
        }
    }

    for(auto i = this->objs.begin(); i != objs.end(); i++)
    {
        (*i)->physics(tick);
    }
    this->triggers = this->next_trigger;
}

v2 room::get_pixelstr_dim(const pixelstr &bdat)
{
    int w = 0;
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

int room::collision_with_base(rplayerobj *rpo)
{
    switch(Pixel::get_pixel_char(this->base_dat[rpo->coords().hori + rpo->coords().vert * (this->size.hori + 1)]))
    {
        case '#':
            return 1;
        case ' ':
            return 0;
    }
    return 2;
}

void room::register_event(const unsigned long long tick, const std::function<void()> fct)
{
    event_queue.push_back(reventqueue::make(tick, fct));
}

void room::run_trigger(int ID)
{
    this->next_trigger |= 1 << ID;
}

bool room::is_triggered(int ID)
{
    return (this->triggers & (1 << ID)) > 0;
}

room::~room()
{
    for(std::vector<robj*>::iterator i = this->objs.begin(); i != this->objs.end(); i++)
    {
        delete *i;
    }
    this->objs.clear();

    for(std::vector<reventqueue*>::iterator i = this->event_queue.begin(); i != this->event_queue.end(); i++)
    {
        delete *i;
    }
    this->event_queue.clear();
}
