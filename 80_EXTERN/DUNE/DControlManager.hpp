/*****************************************
|      ____    _  _   _   _   ____       |
|     |  _ \  | || | | \ | | |  __|      |
|     | | \ \ | || | |  \| | | |_        |
|     | | | | | || | |     | |  _|       |
|     | |_/ / | || | | |\  | | |__       |
|     |____/  \____/ |_| \_| |____|      |
|                                        |
|   DUNE - Sandbox Depth Visualizer      |
|  A project by                          |
|  Ludwig Zeller and David Schoosleitner |
|                                        |
|****************************************|
|                                        |
| @file DControlManager.cpp              |
| @author David Schoosleitner            |
| @date Oct. 26th 2022                   |
|                                        |
| This file is responsible for:          |
|  - Controling timings of multiple      |
|    application sections and threads    |
| This file depends on:                  |
|  - Managers                            |
|****************************************|
|                                        |
| USED LIBRARIES ARE PROPERTY OF THEIR   |
| CORRESPECTIVE OWNERS.                  |
|                                        |
| SEE COPYRIGHT.md IN PROJECT'S ROOT     |
| DIRECTORY, AS WELL AS                  |
| INDIVIDUAL LICENSES AND TOS.           |
|                                        |
*****************************************/

#pragma once

/*****************************************
|               INCLUDES                 |
*****************************************/
#include <iostream>
#include <chrono>
#include <functional>
#include <exception>
#include "DManager.hpp"


/*****************************************
|          GLOBAL DECLARATIONS           |
*****************************************/

/**********************
 * NAMESPACE DManager *
 **********************
 * @namespace DManager
 * @brief Contains multithreading-related
 * @brief tools for DUNE
 **********************/
namespace DManager
{

/**********************
 * CLASS DControlManager
 **********************
 * @class DControlManager
 * @brief Responsible for timings and execution of individual
 * @brief Managers and Threads. To be launched in main thread!
 **********************/
class DControlManager
{
private:

    //!! Managers are stored as members. Define and override their user functions via included .hpp
    std::vector<DManager*> managers;
    std::vector<std::function<void()>> post_terminate;
    std::atomic<bool> running = true;

public:

    /**
     * @fn DManager::DControlManager::~DControlManager()
     * @brief Destroy the DControlManager object
     */
    ~DControlManager()
    {
        call_termination();
    }

    /**
     * @fn void DManager::DControlManager::call_termination()
     * @brief Invokes termination of all manager members
     * @deprecated Not required as managers are automatically terminated and their threads freed upon destruction.
     */
    void call_termination()
    {
        for(auto i = managers.begin(); i != managers.end(); i++)
        {
            (*i)->terminate();
        }
    }

    void stop()
    {
        running = false;
    }

    /**
     * @fn void DManager::DControlManager::launch()
     * @brief Loops and notifies managers according to their scedule, launch in Main thread!
     */
    void launch()
    {
        using namespace std::chrono;

        //!! Only run if managers are subscribed
        if(managers.size() <= 0)
        {
            throw std::invalid_argument("No managers have been registered");
        }

        //!! Wait until all members finish initialization
        for(auto i = managers.begin(); i != managers.end(); i++)
        {
            while(!(*i)->can_run());
        }
    
        //!! Main loop
        while(this->running) { for(auto i = managers.begin(); i != managers.end(); i++)
        {
            if((*i)->in_schedule())
                (*i)->notify();
        }}
        //this->call_termination();
        
        for(auto i = post_terminate.begin(); i != post_terminate.end(); i++)
        {
            (*i)();
        }
    }

    void subscribe(void *ptr)
    {
        managers.push_back((DManager*) ptr);
    }

    void subscribe_post(std::function<void()> fct)
    {
        post_terminate.push_back(fct);
    }

    inline bool is_running()
    {
        return running;
    }
};

};
