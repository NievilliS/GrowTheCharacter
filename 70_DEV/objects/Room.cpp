#include "Room.hpp"
#include "../ColorExtensions.hpp"

room::room(const int _index):
    m_background_color(Pixel::DEFAULT),
    m_foreground_color(Pixel::DEFAULT),
    m_size{1,1},
    m_index(_index)
{
}

room &room::operator+(robj *obj)
{
    this->add_obj(obj);
    return *this;
}

void room::set_base_dat_str(pixelstr &bdat)
{
    this->m_base_dat = bdat;
    this->m_size = get_pixelstr_dim(this->m_base_dat);

    Pixel::for_each(this->m_base_dat, [this](const size_t a, char &b, Pixel::ColorType &c, Pixel::Color &d, Pixel::Font &e){
        if(d == Pixel::DEFAULT)
        {
            c = Pixel::TEXT;
            d = this->m_foreground_color;
        }
    });
}

void room::add_obj(robj *obj)
{
    this->m_object_storage.push_back(obj);
}

void room::draw(unsigned long long tick)
{
    ConsoleEngine_context << ConsoleEngine::CLEAR <<
        this->m_background_color.get_bcontrols() << "" <<
        this->m_base_dat;

    pixelstr str_B2, str_B1, str_D, str_F1, str_F2, str_F3, str_F4;

    for(auto i = this->m_object_storage.begin(); i != m_object_storage.end(); i++)
    {
        if((*i)->coords() >= v2{0,0} && (*i)->coords() < this->m_size)
            switch((*i)->get_layer())
            {
                case robj::B2:
                    str_B2 += (*i)->get_coord_str() + (*i)->draw(tick);
                break;
                case robj::B1:
                    str_B1 += (*i)->get_coord_str() + (*i)->draw(tick);
                break;
                case robj::F1:
                    str_F1 += (*i)->get_coord_str() + (*i)->draw(tick);
                break;
                case robj::F2:
                    str_F2 += (*i)->get_coord_str() + (*i)->draw(tick);
                break;
                case robj::F3:
                    str_F3 += (*i)->get_coord_str() + (*i)->draw(tick);
                break;
                case robj::F4:
                    str_F4 += (*i)->get_coord_str() + (*i)->draw(tick);
                break;
                case robj::D:
                default:
                    str_D += (*i)->get_coord_str() + (*i)->draw(tick);
                break;
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
    this->m_next_trigger_map = 0ULL;

    //!! Event queue handle
    {
        std::vector<std::vector<reventqueue*>::iterator> indices;
        for(auto i = this->m_event_queue.begin(); i != m_event_queue.end(); i++)
        {
            if((*i)->check_run(tick))
                indices.push_back(i);
        }

        for(auto i = indices.begin(); i != indices.end(); i++)
        {
            this->m_event_queue.erase(*i);
        }
    }

    for(auto i = this->m_object_storage.begin(); i != m_object_storage.end(); i++)
    {
        if(m_cancel_phys)
        {
            m_cancel_phys = false;
            return;
        }

        (*i)->physics(tick);
    }
    this->m_trigger_map = this->m_next_trigger_map;
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
    this->m_background_color = bgr;
}

void room::set_foreground_color(const Pixel::Color &bgr)
{
    this->m_foreground_color = bgr;
}

int room::collision_with_base(rplayerobj *rpo)
{
    if(rpo->coords().hori < 0 || rpo->coords().hori >= this->m_size.hori || rpo->coords().vert < 0 || rpo->coords().vert >= this->m_size.vert)
        return 3;
    switch(Pixel::get_pixel_char(this->m_base_dat[rpo->coords().hori + rpo->coords().vert * (this->m_size.hori + 1)]))
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
    this->m_event_queue.push_back(reventqueue::make(tick, fct));
}

void room::run_trigger(int ID)
{
    this->m_next_trigger_map |= 1 << ID;
}

bool room::is_triggered(int ID)
{
    return (this->m_trigger_map & (1 << ID)) > 0;
}

room::~room()
{
    for(std::vector<robj*>::iterator i = this->m_object_storage.begin(); i != this->m_object_storage.end(); i++)
    {
        delete *i;
    }
    this->m_object_storage.clear();

    for(std::vector<reventqueue*>::iterator i = this->m_event_queue.begin(); i != this->m_event_queue.end(); i++)
    {
        delete *i;
    }
    this->m_event_queue.clear();

    for(std::vector<roomtransition*>::iterator i = this->m_transition_storage.begin(); i != this->m_transition_storage.end(); i++)
    {
        delete *i;
    }
    this->m_transition_storage.clear();
}

roomtransition *room::get_if_collide(const v2 &v)
{
    for(auto i = this->m_transition_storage.begin(); i != this->m_transition_storage.end(); i++)
    {
        if((*i)->collides(v))
            return (*i);
    }
    return nullptr;
}

void room::remove_obj(const robj *obj)
{
    for(auto i = this->m_object_storage.begin(); i != this->m_object_storage.end(); i++)
    {
        if(*i == obj)
        { 
            this->m_object_storage.erase(i);
            return;
        }
    }
}
