#include "Room.hpp"
#include "CharObj.hpp"

int main(void)
{
    ConsoleEngine ce;
    ConsoleEngine::_set_context(ce);
    
    room rm;

    std::string st =
        "############\n"
        "#          #\n"
        "#          #\n"
        "#          #\n"
        "#          #\n"
        "############"
    ;

    pixelstr ps;
    Pixel::copy_string_to_pixel_string(ps, st);

        rm.set_base_dat_str(ps);

    std::string sssssss = "Hello world";

int i = 0;
    for(char c : sssssss)

    rm + new rcharobj(++i,1,c);

    rm.draw();
    return 0;

}
