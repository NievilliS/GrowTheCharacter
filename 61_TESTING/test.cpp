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

    pixelstr ps{};
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
    
    for(int x = 1; x < env_room->getsize().hori; x += 4)
    for(int y = 1; y < env_room->getsize().vert; y += 4)
    env_room->add_obj(new rspinnyobj(x, y, 'o', (x+y) % 4 + 4, (x+y)%8, robj::F2, ((x+y) % 2 == 0) ? Pixel::BLUE : Pixel::GREEN));

    genv.launch();

    return 0;
}
