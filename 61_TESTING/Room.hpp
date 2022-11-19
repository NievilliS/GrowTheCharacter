#pragma once

#include <consoleengine/ConsoleEngine.hpp>
#include "RObj.hpp"
#include <memory>

class room
{
private:
    pixelstr base_dat;
    Pixel::Color background_color;
    Pixel::Color foreground_color;
    v2 size;
    std::vector<std::unique_ptr<robj>> objs;
    unsigned long long tick = 0;

public:
    room();
    
    void set_base_dat_str(pixelstr &bdat);
    void add_obj(robj *obj);
    void draw();
    void physics();
    void set_background_color(const Pixel::Color &bgr);
    void set_foreground_color(const Pixel::Color &fgr);

    inline v2 getsize() {return size;}

    static v2 get_pixelstr_dim(const pixelstr &bdat);

    room &operator+(robj *obj);
};
