#include "GameMaker.hpp"
#include "ColorExtensions.hpp"
#include <fstream>
#include <string.h>
#include "objects/Roller.hpp"

int main(int argc, char **argv)
{
    std::regex _mog{"-lr([0-9]+)-([0-9]+)"};
    std::regex _mog2{"-d"};
    std::regex _mog3{"-i(.*)"};
    std::smatch smat;

    std::string path = "./Prototype0.dat";

    for(int i = 1; i < argc; i++)
    {
        std::string s = argv[i];
        if(std::regex_search(s, smat, _mog3))
        {
            path = smat[1].str();
        }
    }

    std::ifstream h(path);
    std::stringbuf sb;
    h.operator>>(&sb);
    h.close();

    gamemaker maker(sb.str());
    gameenv *genv = maker.createenvironment();

    genv->get_active_level()->get_active_room()->for_each<rplayerobj>([genv](rplayerobj &t)
                                                                      { genv->set_plr(&t); });

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
            genv->get_active_level()->get_active_room()->direct_commit();
            genv->set_active_index_level(std::atoi(smat[1].str().c_str()));
            genv->get_active_level()->set_active_index_room(std::atoi(smat[2].str().c_str()));
            genv->get_active_level()->get_active_room()->add_obj(rpp);
            genv->get_active_level()->get_active_room()->direct_commit();

            if(genv->get_active_level()->get_active_room()->get_checkpoint())
            {
                rpp->coords() = genv->get_active_level()->get_active_room()->get_checkpoint()->m_room_pos;
                rpp->set_checkpoint(genv->get_active_level()->get_active_room()->get_checkpoint());
            }
            else
            {
                rpp->coords().hori = genv->get_active_level()->get_active_room()->get_size().hori / 2;
                rpp->coords().vert = genv->get_active_level()->get_active_room()->get_size().vert / 2;
                genv->get_active_level()->get_active_room()->set_checkpoint({rpp->coords().vert, rpp->coords().hori});
                rpp->set_checkpoint(genv->get_active_level()->get_active_room()->get_checkpoint());
            }
        }
        else if(std::regex_search(s, _mog2))
        {
            genv->__debug_set_flag();
        }
    }

    genv->launch();

    return 0;
}
