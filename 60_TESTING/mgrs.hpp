#pragma once

#include "RTE.hpp"

class Mgr : public DManager::DManager
{
public:
    Mgr() : DManager(DManagerFlags{
        .can_skip = false,
        .log_lag = false,
        .target_period = std::chrono::milliseconds(1000),
        .delay_target_period = std::chrono::milliseconds(0),
        .log_ela = false,
        .name = "Hello",
        .adress_millis = nullptr
    })
    {
    }

    int ct = 0;

    virtual void run_user()
    {
        ct++;
        if(ct > 10)
            RTE::stop();
    }
};
