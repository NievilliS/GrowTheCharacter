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

    #ifdef BRUH
    genv->stop();genv->debug_terminate();genv->debug_join();
    #else
    genv->launch();
    #endif

    return 0;
}
