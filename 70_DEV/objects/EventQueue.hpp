#include <functional>

class revent
{
protected:
    bool m_was_run = false;
    const unsigned long long m_tick_schedule;
    const std::function<void()> m_function;
    revent(const unsigned long long _tick_schedule, const std::function<void()> _fct):
        m_tick_schedule(_tick_schedule),
        m_function(_fct)
    {
    }

public:
    static revent *make(const unsigned long long _tick_schedule, const std::function<void()> _fct)
    {
        return new revent(_tick_schedule, _fct);
    }
    const bool check_run(const unsigned long long _tick_now)
    {
        if(!m_was_run && _tick_now >= m_tick_schedule)
        {
            m_was_run = true;
            m_function();
        }
        return m_was_run;
    }
};
