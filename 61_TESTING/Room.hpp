#pragma once

#include <consoleengine/ConsoleEngine.hpp>
#include "RObj.hpp"
#include <memory>

class room
{
private:
    pixelstr base_dat;
    Pixel::Color background_color;
    v2 size;
    std::vector<std::unique_ptr<robj>> objs;

public:
    room();
    
    void set_base_dat_str(pixelstr &bdat);
    void add_obj(robj *obj);
    void draw();
    void physics();

    static v2 get_pixelstr_dim(const pixelstr &bdat);

    room &operator+(robj *obj);
};
