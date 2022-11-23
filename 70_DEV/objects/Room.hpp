#pragma once

#include <consoleengine/ConsoleEngine.hpp>
#include "RObj.hpp"
#include "Player.hpp"
#include "EventQueue.hpp"
#include <atomic>
#include <memory>

class room
{
private:
    pixelstr m_base_dat{};
    Pixel::Color m_background_color;
    Pixel::Color m_foreground_color;
    v2 m_size;
    std::vector<robj*> m_object_storage;
    std::vector<reventqueue*> m_event_queue;
    std::string m_room_name;
    int m_index;
    unsigned long long m_trigger_map = 0ULL;
    unsigned long long m_next_trigger_map = 0ULL;

public:
    room(const int _index);
    ~room();
    
    void set_base_dat_str(pixelstr &bdat);
    void add_obj(robj *obj);
    room &operator+(robj *obj);
    void draw(unsigned long long tick);
    void physics(unsigned long long tick);
    void set_background_color(const Pixel::Color &bgr);
    void set_foreground_color(const Pixel::Color &fgr);
    int collision_with_base(rplayerobj *rpo);
    void register_event(const unsigned long long tick, const std::function<void()> fct);
    void run_trigger(int ID);
    bool is_triggered(int ID);
    template<typename T> void for_each(std::function<void(T &)> fct)
    {
        std::vector<T*> vct;
        for(auto i = this->m_object_storage.begin(); i != m_object_storage.end(); i++)
        {
            T* pt = dynamic_cast<T*>(*i);
            if(pt != nullptr)
                vct.push_back(pt);
        }
    
        for(auto i = vct.begin(); i != vct.end(); i++)
        {
            fct(**i);
        }
    }

    inline v2 get_size() {return m_size;}
    inline void set_name(const std::string _name) {this->m_room_name = _name;}
    inline std::string get_name() const {return this->m_room_name;}
    inline void set_index(const int _index) {this->m_index = _index;}
    inline int get_index() const {return this->m_index;}

    static v2 get_pixelstr_dim(const pixelstr &bdat);
    inline pixelstr get_base_dat_str() const {return this->m_base_dat;}
};
