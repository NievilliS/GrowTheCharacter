#pragma once

#include "RTE.hpp"

class Mgr : public DManager::DManager
{
public:
    Mgr() : DManager(DManagerFlags{
        .can_skip = false,
        .log_lag = false,
        .target_period = std::chrono::milliseconds(5),
        .delay_target_period = std::chrono::milliseconds(0),
        .log_ela = false,
        .name = "Hello",
        .adress_millis = nullptr
    })
    {
    }

    unsigned long long tick = 0ULL;

    virtual void run_user()
    {
        tick++;

        if(RTE::up_kbpressed())
        {
            int x, y;
            ConsoleEngine_context.getxy(&y, &x);
            if(y > 1)
            {
                ConsoleEngine_context.setxy(y - 1, x);
            }
        }
        if(RTE::down_kbpressed())
        {
            int x, y;
            ConsoleEngine_context.getxy(&y, &x);
            ConsoleEngine_context.setxy(y + 1, x);
        }
        if(RTE::left_kbpressed())
        {
            int x, y;
            ConsoleEngine_context.getxy(&y, &x);
            if(x > 1)
            {
                ConsoleEngine_context.setxy(y, x - 1);
            }
        }
        if(RTE::right_kbpressed())
        {
            int x, y;
            ConsoleEngine_context.getxy(&y, &x);
            ConsoleEngine_context.setxy(y, x + 1);
        }

        ConsoleEngine_context <<
            ConsoleEngine::CLEAR << ConsoleEngine::ERASE_SCREEN << Pixel::BACKGROUND << Pixel::YELLOW << Pixel::BOLD <<
            "Count: " << tick << ConsoleEngine::PRINTOUT;
    }
};
