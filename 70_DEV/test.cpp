#include "GameMaker.hpp"
#include "ColorExtensions.hpp"
#include <fstream>

int main(void)
{
    std::ifstream h("./20_CONCEPT/RoomData.dat");
    std::stringbuf sb;
    h.operator>>(&sb);
    h.close();

    gamemaker maker(sb.str());
    gameenv *genv = maker.createenvironment();

    //roomtransition *r0tr = new roomtransition(6, 15, 4, 1, 48, 48, 0, 0);
    //roomtransition *r1tr = new roomtransition(4, -1, 4, 1, 49, 48, 1, 0);
    //r0tr->set_link(r1tr);
    //r1tr->set_link(r0tr);

    //genv->get_active_level()->get_active_room()->add_transition(r0tr);
    //genv->get_active_level()->set_active_room(1);
    //genv->get_active_level()->get_active_room()->add_transition(r1tr);
    //genv->get_active_level()->set_active_room(0);

    #ifdef BRUH
    genv->stop();genv->debug_terminate();genv->debug_join();
    #else
    genv->launch();
    #endif

    return 0;
}
