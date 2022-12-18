#include "GameMaker.hpp"
#include "ColorExtensions.hpp"
#include <fstream>
#include <string.h>
#include "objects/Roller.hpp"

int main(int argc, char **argv)
{
    std::regex _mog{"-lr([0-9]+)-([0-9]+)"};
    std::smatch smat;

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

    for(int i = 1; i < argc; i++)
    {
        std::string s = argv[i];
        if(std::regex_search(s, smat, _mog))
        {
            rplayerobj *rpp;
            genv->get_active_level()->get_active_room()->for_each<rplayerobj>([genv,&rpp](rplayerobj &t)
            {
                rpp = &t;
            });

            genv->get_active_level()->get_active_room()->remove_obj(rpp);
            genv->set_active_index_level(std::atoi(smat[1].str().c_str()));
            genv->get_active_level()->set_active_index_room(std::atoi(smat[2].str().c_str()));
            genv->get_active_level()->get_active_room()->add_obj(rpp);

            if(genv->get_active_level()->get_active_room()->get_checkpoint())
            {
                rpp->coords() = genv->get_active_level()->get_active_room()->get_checkpoint()->m_room_pos;
            }
            else
            {
                rpp->coords().hori = genv->get_active_level()->get_active_room()->get_size().hori / 2;
                rpp->coords().vert = genv->get_active_level()->get_active_room()->get_size().vert / 2;
            }
        }
    }

    genv->launch();

    return 0;
}
