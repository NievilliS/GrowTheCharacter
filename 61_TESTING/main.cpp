#include "Room.hpp"
#include "CharObj.hpp"
#include "ColorExtensions.hpp"

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

    pixelstr ps{};
    Pixel::copy_string_to_pixel_string(ps, st, [](auto a, auto &b, auto &c, auto &d, auto &e){
        d = a % 2 ? Pixel::RED : Pixel::Extensions::DARK_RED;
    });
        rm.set_background_color(Pixel::BLACK);
        rm.set_foreground_color(Pixel::DEFAULT);
        rm.set_base_dat_str(ps);

    std::string sssssss = "Hello world";

int i = 0;
    for(char c : sssssss)

    rm + new rcharobj(++i,1,c);

    rm.draw();

    std::cout << CURSOR_MOVE_TO(8,1) << std::endl;

    return 0;

}
