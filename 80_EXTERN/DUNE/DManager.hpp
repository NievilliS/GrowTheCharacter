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
| @file main.cpp                         |
| @author David Schoosleitner            |
| @date Nov. 3rd 2022                    |
|                                        |
| This file is responsible for:          |
|  - Default Manager class               |
| This file depends on:                  |
|  -                                     |
|                                        |
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
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <thread>
#include <mutex>
#include <iostream>
#include <string>


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
 *  CLASS DManager    *
 **********************
 * @class DManager
 * @brief Manager that takes care of a single thread.
 * @brief This class in particular is a template. Override user functions.
 * @brief Can not manage alone, requires DControlManager.
 **********************/
class DManager
{
public:

    /**********************
     * STRUCT DManagerFlags
     **********************
     * @struct DManagerFlags
     * @brief Contains primitive values that act as simple control flags for manager.
     * @brief Use R-Value literal definition in DManager Constructor params!
     **********************/
    struct DManagerFlags {
        const bool can_skip;                                            //< 1: Flag allows for thread ticks to be skipped, rather than to catch up (by skipping wait_for)
        const bool log_lag;                                             //< 2: Call std::cout if a tick lags behind
        const std::chrono::milliseconds target_period;                  //< 3: Target period to check for schedule
        const std::chrono::milliseconds delay_target_period;            //< 4: Target delay; it acts as an offset
        const bool log_ela;                                             //< 5: Call std::cout for every elapsed time per tick
        const std::string name;                                         //< 6: Name used in log calls.
        const int *adress_millis;                                       //< 7: Address to store millis elapse into.
    };

private:
    std::condition_variable cv;                             //< Used to pause thread
    std::mutex mutex;                                       //< Mutex is responsible for thread control
    std::thread thread;                                     //< Working thread
    std::atomic<int> running;                               //< Atomic: If a tick is now running
    std::atomic<int> terminated;                            //< Atomic: If the thread has been terminated
    std::atomic<int> milli_last_ela;                        //< Atomic: How long the last tick has taken
    std::atomic<int> canrun;                                //< Atomic: If initialization is done
    const struct DManagerFlags flags;                       //< Control flags for manager
    std::chrono::steady_clock::time_point schedule;         //< Next schedule to be executed at

    /**
     * @fn bool DManager::DManager::wait_for()
     * @brief Wait for is called by DManager::run
     * 
     * @return true = thread while loop continues ||
     * @return false = thread while loop breaks
     */
    bool wait_for()
    {
        //! Do not run when terminated
        if(this->terminated > 0)
            return false;

        //! Skip a waiting sequence, that means, catch up if lagging
        if(--this->running > 0)
            return true;

        //!! @ref https://stackoverflow.com/questions/52610776/how-to-wake-a-stdthread-while-it-is-sleeping
        //!! Waiting for 10 seconds or until notified
        std::unique_lock<std::mutex> l(this->mutex);
        return !this->cv.wait_for(
            l,
            std::chrono::seconds(10),
            [this] () -> bool { return this->is_closed() || this->is_running(); }
        );
    }

public:

    /**
     * @fn DManager::DManager::DManager(const struct DManagerFlags &&d)
     * @brief Construct a new DManager object
     * @param d Flags to control Manager parameters
     */
    DManager(const struct DManagerFlags &&d):
        running(0),
        thread([this] () -> void { this->run(); }),
        terminated(0),
        milli_last_ela(0),
        canrun(0),
        flags(d),
        schedule(std::chrono::steady_clock::now() + d.delay_target_period)
    {
    }

    /**
     * @fn DManager::DManager::~DManager()
     * @brief Termination is called upon destruction. Thread and mutex are "freed"
     * @ref https://stackoverflow.com/questions/52610776/how-to-wake-a-stdthread-while-it-is-sleeping
     */
    ~DManager()
    {
        //! Takes ownership over mutex in scope. Mutex "freed" upon destruction
        {
            std::lock_guard<std::mutex> l(this->mutex);
        }

        //!! Terminates the thread if not already
        if(this->terminated == 0)
        {
            this->terminated = 1;
            this->cv.notify_one();
            this->thread.join();
            std::cout << "Automatic termination of " << this->flags.name << " " << std::endl;
        }
    }

    /**
     * @fn void DManager::DManager::run()
     * @brief this code will be run in a seperate thread
     */
    void run()
    {
        //! Timing init
        auto start = std::chrono::steady_clock::now();
        auto now = start;
        auto ela = start-start;

        //! User defined init (in scope)
        this->init_user();

        //! Flag as available and wait
        this->canrun = 1;
        this->wait_for();

        //!! Thread loops
        while(!this->is_closed())
        {
            //! Start time
            start = std::chrono::steady_clock::now();

            //! Run User code
            this->run_user();

            //! Elaborate time
            now = std::chrono::steady_clock::now();
            this->milli_last_ela = 
                std::chrono::duration_cast
                <std::chrono::milliseconds>
                (now - start)
                .count();
            if(this->flags.adress_millis != NULL)
            {
                *((int*)(this->flags.adress_millis)) = this->milli_last_ela;
            }

            //! Print ela
            if(this->flags.log_ela)
            {
                std::cout << "Thread " << std::this_thread::get_id() << ", \"" << this->flags.name << "\" took " << this->milli_last_ela << "ms." << std::endl;
            }
            else if(this->flags.log_lag && this->milli_last_ela > this->flags.target_period.count())
            {
                std::cout << "Thread " << std::this_thread::get_id() << ", \"" << this->flags.name << "\" is lagging behind! Took " << this->milli_last_ela << "ms." << std::endl;
            }

            //! Wait until next notify
            this->wait_for();
        }
    }

    /**
     * @fn virtual void init_user()
     * @brief Override this function for user init code
     */
    virtual void init_user()
    {
    }

    /**
     * @fn virtual void run_user()
     * @brief Override this function for user run code
     */
    virtual void run_user()
    {
    }

    /**
     * @fn void DManager::DManager::notify()
     * @brief Called by DManager::DControlManager, wakes up from waiting
     */
    void notify()
    {
        if(this->flags.can_skip)
            this->running = 1;
        else
            this->running++;
        this->cv.notify_one();
    }

    /**
     * @fn void DManager::DManager::terminate()
     * @brief Manual termination of thread, mutex is not freed
     */
    void terminate()
    {
        //!! Terminate thread, do not interfere with mutex outside of deconstructor
        //!! Also prevent double termination
        if(this->terminated == 0)
        {
            this->terminated = 1;
            this->cv.notify_one();
            this->thread.join();
            std::cout << "Manual termination of " << this->flags.name << " " << std::endl;
        }
    }

    /**
     * @fn inline int get_millis() const
     * @return int of last tick elapsed time
     */
    inline int get_millis() const
    {
        return this->milli_last_ela;
    }

    /**
     * @fn inline bool is_closed() const
     * @return bool if terminated
     */
    inline bool is_closed() const
    {
        return this->terminated > 0;
    }

    /**
     * @fn inline bool is_running() const
     * @return bool if tick is running
     */
    inline bool is_running() const
    {
        return this->running > 0;
    }

    /**
     * @fn inline bool can_run() const
     * @return bool if init has finished
     */
    inline bool can_run() const
    {
        return this->canrun > 0;
    }

    /**
     * @fn inline std::chrono::milliseconds get_target_period() const
     * @return milliseconds of target period
     */
    inline std::chrono::milliseconds get_target_period() const
    {
        return this->flags.target_period;
    }

    /**
     * @fn inline std::chrono::milliseconds get_target_delay() const
     * @return milliseconds of target delay
     */
    inline std::chrono::milliseconds get_target_delay() const
    {
        return this->flags.delay_target_period;
    }

    /**
     * @fn inline bool in_schedule()
     * @brief Runned by DControlManager::launch, gets if past schedule, reschedules if so
     * 
     * @return if past schedule
     */
    inline bool in_schedule()
    {
        if(this->schedule <= std::chrono::steady_clock::now())
        {
            this->reschedule();
            return true;
        }
        return false;
    }

    /**
     * @fn inline void reschedule()
     * @brief Sets schedule to now + target period
     */
    inline void reschedule()
    {
        this->schedule = std::chrono::steady_clock::now() + this->flags.target_period;
    }

    /**
     * @fn inline bool DManager::operator bool()
     * @brief Runned by DControlManager::launch, gets if past schedule, reschedules if so.
     * @brief Short version of DManager::in_schedule()
     * @return if past schedule
     */
    inline operator bool()
    {
        return in_schedule();
    }
};

};
