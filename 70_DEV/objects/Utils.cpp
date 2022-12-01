#include "Utils.hpp"
#include "../GameEnvironment.hpp"

void Utils::init(void *_env_ptr)
{
    m_env_ptr = _env_ptr;
}

void Utils::do_trig(const __UINT8_TYPE__ ID)
{
    if(ID > 64 && ID < 129)
    {
        gameenv &genv = *(gameenv*) m_env_ptr;
        level *env_level = genv.get_active_level();
        env_level->do_ltri(ID);
        env_level->propagate_ltriggers();
        return;
    }
    if(ID < 0 || ID > 63) return;
    gameenv &genv = *(gameenv*) m_env_ptr;
    level *env_level = genv.get_active_level();
    room *env_room = env_level->get_active_room();
    env_room->run_trigger(ID);
}

bool Utils::is_trig(const __UINT8_TYPE__ ID)
{
    if(ID > 64 && ID < 129)
    {
        gameenv &genv = *(gameenv*) m_env_ptr;
        level *env_level = genv.get_active_level();
        return env_level->is_ltrig(ID);
    }
    if(ID < 0 || ID > 63) return false;
    gameenv &genv = *(gameenv*) m_env_ptr;
    level *env_level = genv.get_active_level();
    room *env_room = env_level->get_active_room();
    return env_room->is_triggered(ID);
}

void Utils::do_event(const unsigned long long delay, std::function<void()> fct)
{
    gameenv &genv = *(gameenv*) m_env_ptr;
    genv.register_event_current(delay, fct);
}

v2 *Utils::get_env_pcoord()
{
    gameenv &genv = *(gameenv*) m_env_ptr;
    return genv.get_pcoord(); 
}
