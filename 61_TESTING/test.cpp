#include "GameEnvironment.hpp"
#include "ColorExtensions.hpp"
#include "objects/Spinny.hpp"
#include "objects/SpeedBump.hpp"

int main(void)
{
    gameenv genv;

    std::string st =
        "############\n"
        "#         x#\n"
        "#  #      x#\n"
        "# #       x#\n"
        "#         x#\n"
        "############"
    ;

    pixelstr ps;
    Pixel::copy_string_to_pixel_string(ps, st, [](auto a, auto &b, auto &c, auto &d, auto &e){
        d = Pixel::WHITE;
        if(b == '#')
        d = a % 2 ? Pixel::RED : Pixel::Extensions::DARK_RED;
    });

    genv.subscribe_new_room(new room());
    genv.set_active_room(0);
    room *env_room = genv.get_active_room();
    env_room->set_background_color(Pixel::BLACK);
    env_room->set_base_dat_str(ps);
    for(int i = 0; i < 4; i++)
        env_room->add_obj(new rspeedbump(rspeedbump::RIGHT, 4 + i, 3));
    env_room->add_obj(new rplayerobj(&genv, 1, 1, '@'));
    env_room->add_obj(new rspinnyobj(4, 4, 'o', robj::F2));

    genv.launch();
    return 0;
}
