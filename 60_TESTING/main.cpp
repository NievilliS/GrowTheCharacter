/***********************************
|* GTC
|*  Grow the Character by Nievillis
|***********************************
|* TESTING
|***********************************
|* @file room.hpp
|* 
|* This file is responsible for:
|*  - IO
|* This file depends on:
|*  - @ref room.hpp
|*
|***********************************/

/************************************
|* INCLUDE
|***********************************/

#include <iostream>
#include "utils.hpp"
#include "mgrs.hpp"
#include <consoleengine/ConsoleEngine.hpp>
#include "RTE.hpp"

/************************************
|* MAIN fct
|************************************
|* @fn 
|*
|***********************************/
int main(void)
{
    #define CE ConsoleEngine_context
    {
        ConsoleEngine ce(std::cout);
        ConsoleEngine::_set_context(ce);
    }

    DManager::DControlManager dcm;
    RTE::dcm_ptr = &dcm;
    Mgr m;

    std::cout << ERASE_DISPLAY << CURSOR_MOVE_TO(1,1) << std::flush;

    dcm.subscribe(&m);

    dcm.launch([&]{
        using namespace RTE;
        static int last_pr = 0;
        int i_key = getkey();
        while(i_key != -1)
        {
            last_pr = 1;
            std::cout << (int)i_key << std::endl;
        }
        if(last_pr == 1)
        {
            last_pr = 0;
            #define releasekeyif(name) if(name ## _kbisdown()) name ## _kbrelease()
            releasekeyif(up);
            releasekeyif(down);
            releasekeyif(left);
            releasekeyif(right);
            releasekeyif(escape);
        }
    });
}
