#include "Utils.hpp"
#include "../GameEnvironment.hpp"

void Utils::init(void *_env_ptr)
{
    env_ptr = _env_ptr;
}

void Utils::do_trig(const __UINT8_TYPE__ ID)
{
    if(ID < 0 || ID > 63) return;
    gameenv &genv = *(gameenv*) env_ptr;
    level *env_level = genv.get_active_level();
    room *env_room = env_level->get_active_room();
    env_room->run_trigger(ID);
}

bool Utils::is_trig(const __UINT8_TYPE__ ID)
{
    if(ID < 0 || ID > 63) return false;
    gameenv &genv = *(gameenv*) env_ptr;
    level *env_level = genv.get_active_level();
    room *env_room = env_level->get_active_room();
    return env_room->is_triggered(ID);
}
