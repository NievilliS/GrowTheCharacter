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
    {
        ConsoleEngine ce(std::cout);
        ConsoleEngine::_set_context(ce);
    }

    DManager::DControlManager dcm;
    RTE::dcm_ptr = &dcm;

    class : DManager::DManager {
        using DManager::DManager::DManager;

        virtual void init_user() override
        {
        }

        volatile int count = 0;

        virtual void run_user() override
        {
            count++;
            ConsoleEngine_context.clear();
            ConsoleEngine_context.add_string("Count: ", Pixel::DEFAULT, Pixel::TEXT, Pixel::BOLD)
                .add_string(std::to_string(count), Pixel::YELLOW, Pixel::TEXT, Pixel::UNDERLINED);

            if(count > 10000)
            {
                RTE::dcm_ptr->stop();
            }
        }

    } mgr_stack(DManager::DManager::DManagerFlags{
        .can_skip               = true,    
        .log_lag                = true,    
        .target_period          = std::chrono::milliseconds(2),        
        .delay_target_period    = std::chrono::milliseconds(0),                
        .log_ela                = false,    
        .name                   = "Test Manager",
        .adress_millis          = &RTE::millis        
    });

    class : DManager::DManager {
        using DManager::DManager::DManager;

        virtual void init_user() override
        {
        }

        virtual void run_user() override
        {
            std::cout << ConsoleEngine_context.add_string(std::string(" Elapsation: ") + std::to_string(RTE::millis) + "ms", Pixel::BLACK);
        }

    } mgr_print(DManager::DManager::DManagerFlags{
        .can_skip               = true,    
        .log_lag                = true,    
        .target_period          = std::chrono::milliseconds(10),        
        .delay_target_period    = std::chrono::milliseconds(0),                
        .log_ela                = false,    
        .name                   = "Test Manager",
        .adress_millis          = nullptr        
    });


    dcm.subscribe(&mgr_stack);
    dcm.subscribe(&mgr_print);
    dcm.launch();
}

