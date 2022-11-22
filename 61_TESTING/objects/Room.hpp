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
    pixelstr base_dat{};
    Pixel::Color background_color;
    Pixel::Color foreground_color;
    v2 size;
    std::vector<robj*> objs;
    std::vector<reventqueue*> event_queue;
    std::string name;
    int index;
    unsigned long long triggers = 0ULL;
    unsigned long long next_trigger = 0ULL;

public:
    int x = 0;
    int y = 0;
    std::string debug;
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
        for(auto i = this->objs.begin(); i != objs.end(); i++)
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

    inline v2 getsize() {return size;}
    inline void setname(const std::string _name) {this->name = _name;}
    inline std::string getname() const {return this->name;}
    inline void setindex(const int _index) {this->index = _index;}
    inline int getindex() const {return this->index;}

    static v2 get_pixelstr_dim(const pixelstr &bdat);
    inline pixelstr get_base_dat_str() const {return this->base_dat;}
};
