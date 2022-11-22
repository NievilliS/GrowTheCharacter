#include <functional>

class reventqueue
{
protected:
    bool runned = false;
    const unsigned long long tick_schedule;
    const std::function<void()> fct;
    reventqueue(const unsigned long long _tick_schedule, const std::function<void()> _fct):
        tick_schedule(_tick_schedule),
        fct(_fct)
    {
    }

public:
    static reventqueue *make(const unsigned long long _tick_schedule, const std::function<void()> _fct)
    {
        return new reventqueue(_tick_schedule, _fct);
    }
    const bool check_run(const unsigned long long _tick_now)
    {
        if(!runned && _tick_now >= tick_schedule)
        {
            runned = true;
            fct();
        }
        return runned;
    }
};
