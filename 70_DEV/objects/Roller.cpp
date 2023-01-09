#include "Roller.hpp"
#include "../GameEnvironment.hpp"
#include "SolidObj.hpp"
#include "SpeedBump.hpp"
#include "Button.hpp"
#include "TeleportObj.hpp"
#include "RollerSpawner.hpp"

rrollerobj::rrollerobj(void *_env, const int x, const int y, const char c, const direction_e _dir, const int _period, const Pixel::Color _pcol): 
        rcharobj{x, y, c, _pcol, F1}, m_direction(_dir), m_period(_period), m_env(_env), m_ltick(((gameenv*) _env)->get_tick()) {}
rrollerobj::rrollerobj(void *_env, const int x, const int y, const char c, const direction_e _dir, const int _period, const layer_e l, const Pixel::Color _pcol): 
        rcharobj{x, y, c, _pcol, l}, m_direction(_dir), m_period(_period), m_env(_env), m_ltick(((gameenv*) _env)->get_tick()) {}

bool rrollerobj::physics(unsigned long long tick)
{
    if(m_ltick + m_period >= tick)
    {
        return true;
    }
    if(m_destroy)
    {
        invalidate();
        return false;
    }

    m_ltick = tick;

    const int dx = m_direction == LEFT ? -1 : m_direction == RIGHT ? 1 : 0;
    const int dy = m_direction == UP ? -1 : m_direction == DOWN ? 1 : 0;

    gameenv *env = (gameenv*) m_env;
    room *env_room = env->get_active_level()->get_active_room();

    const v2 prv = this->m_primary_coords;
    this->m_primary_coords += v2{dy, dx};

    // Destroy outside, wait on collision with wall or higher dmg
    int _env_room_coll = env_room->collision_with_base(this->m_primary_coords);
    if(_env_room_coll == 3)
    {
        invalidate();
        return false;
    }
    else if(_env_room_coll == 1)
    {
        m_destroy = true;
        return true;
    }
    else if(_env_room_coll == 2)
    {
        const int this_lvl = env->get_damage_index(this->m_character);
        const int base_lvl = env->get_damage_index(env_room->get_char_at_base(this->m_primary_coords));

        if(base_lvl > this_lvl)
        {
            m_destroy = true;
            return true;
        }
    }

    // On collision or destroy inside
    env_room->for_each<rsolidobj>([&](rsolidobj &t){
        if(prv == t.coords() && t.is_solid())
        {
            this->m_primary_coords = prv;
            m_destroy = true;
        }
        else if(this->m_primary_coords == t.coords() && t.is_solid())
        {
            m_destroy = true;
        }
    });

    if(m_destroy) return true;

    // On collision with speed bump change direction
    env_room->for_each<rspeedbump>([&](rspeedbump &t){
        if(this->m_primary_coords == t.coords())
        {
            this->m_direction = t.get_direction();
        }
    });

    // Press button :)
    env_room->for_each<rbuttonobj>([&](rbuttonobj &t){
        if(m_primary_coords == t.coords())
        {
            t.press(tick);
        }
    });

    // Teleporter
    env_room->for_each<rteleportobj>([&](rteleportobj &t){
        if(!t.is_active()) return;
        if(this->m_primary_coords == t.coords() || (!t.is_one_way() && this->m_primary_coords == t.coords2()))
        {
            env->register_event_current(1, [this, &t]{
                t.teleport(&this->m_primary_coords);
            });

            env->register_event_current(40, [&t]{
                t.set_active();
            });
        }
    });

    return true;
}

bool rrollerspawnerobj::physics(unsigned long long tick)
{
    if(m_t_ct++ < m_period_self)
    {
        return true;
    }
    m_t_ct = 0;

    const int dx = m_direction == LEFT ? -1 : m_direction == RIGHT ? 1 : 0;
    const int dy = m_direction == UP ? -1 : m_direction == DOWN ? 1 : 0;

    gameenv *env = (gameenv*) m_env;
    room *env_room = env->get_active_level()->get_active_room();

    env_room->add_obj(new rrollerobj(
        m_env, m_primary_coords.hori + dx, m_primary_coords.vert + dy, m_spawns_char, m_direction, m_period_roll
    ));

    return true;
}
