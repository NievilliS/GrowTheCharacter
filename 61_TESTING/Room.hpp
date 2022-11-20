#pragma once

#include <consoleengine/ConsoleEngine.hpp>
#include "RObj.hpp"
#include "Player.hpp"
#include <memory>

class room
{
private:
    pixelstr base_dat;
    Pixel::Color background_color;
    Pixel::Color foreground_color;
    v2 size;
    std::vector<std::unique_ptr<robj>> objs;

public:
    room();
    
    void set_base_dat_str(pixelstr &bdat);
    void add_obj(robj *obj);
    room &operator+(robj *obj);
    void draw(unsigned long long tick);
    void physics(unsigned long long tick);
    void set_background_color(const Pixel::Color &bgr);
    void set_foreground_color(const Pixel::Color &fgr);
    int collision_with_base(rplayerobj *rpo);

    inline v2 getsize() {return size;}

    static v2 get_pixelstr_dim(const pixelstr &bdat);
};
