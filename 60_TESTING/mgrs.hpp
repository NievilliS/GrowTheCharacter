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
        ConsoleEngine_context <<
            ConsoleEngine::CLEAR << ConsoleEngine::ERASE_SCREEN << Pixel::YELLOW <<
            "Count: " << tick << ConsoleEngine::PRINTOUT;
    }
};
