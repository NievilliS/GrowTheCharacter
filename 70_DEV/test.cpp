#include "GameMaker.hpp"
#include "ColorExtensions.hpp"
#include <fstream>
#include <string.h>
#include "objects/Roller.hpp"

int main(int argc, char **argv)
{
    std::ifstream h("./Prototype0.dat");
    std::stringbuf sb;
    h.operator>>(&sb);
    h.close();

    gamemaker maker(sb.str());
    gameenv *genv = maker.createenvironment();
    if (argc > 1 && strcmp(argv[1], "d") == 0)
        genv->__debug_set_flag();

    genv->get_active_level()->get_active_room()->for_each<rplayerobj>([genv](rplayerobj &t)
                                                                      { genv->set_pcoord(t.get_pcoord_ptr()); });

    //genv->get_active_level()->get_active_room()->
    //    add_obj(new rrollerobj(genv, 1, 5, 'x', robj::RIGHT, 10));

    genv->launch();

    return 0;
}
