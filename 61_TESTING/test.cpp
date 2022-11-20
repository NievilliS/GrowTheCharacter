#include "GameEnvironment.hpp"
#include "ColorExtensions.hpp"
#include "Spinny.hpp"

int main(void)
{
    gameenv genv;

    std::string st =
        "############\n"
        "#         x#\n"
        "#          #\n"
        "#          #\n"
        "#          #\n"
        "############"
    ;

    pixelstr ps;
    Pixel::copy_string_to_pixel_string(ps, st, [](auto a, auto &b, auto &c, auto &d, auto &e){
        d = a % 2 ? Pixel::RED : Pixel::Extensions::DARK_RED;
    });

    genv.subscribe_new_room(new room());
    genv.set_active_room(0);
    genv.get_active_room()->set_base_dat_str(ps);
    genv.get_active_room()->add_obj(new rplayerobj(&genv, 1, 1, '@'));
    genv.get_active_room()->add_obj(new rspinnyobj(4, 4, 'o'));

    genv.launch();
}
