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
#include "cutils.hpp"
#include "mgrs.hpp"
#include "RTE.hpp"

static std::thread thr;

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

    dcm.subscribe_post([]{
        simKeyPress();
    });

    thr = std::thread([]() -> void {
        setBufferedInput(false);
        int c = 0;

        while(RTE::dcm_ptr->is_running())
        {
            if(c == 'q')
            {
                CE << ConsoleEngine::CLEAR << "Ending by q press" << ConsoleEngine::PRINTOUT;
                RTE::dcm_ptr->stop();
                getChar();
                break;
            }

            switch(c)
            {
                case 119:
                case 65:
                    RTE::up_kbpress();
                break;

                case 66:
                case 115:
                    RTE::down_kbpress();
                break;

                default:
                    std::cout << "Char: " << c << std::endl;
            }

            c = getChar();
        }

        setBufferedInput(true);
    });

    dcm.launch([]{});

    thr.join();

}
