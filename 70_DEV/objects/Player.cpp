#include "Player.hpp"
#include "../GameEnvironment.hpp"
#include "Spinny.hpp"
#include "SpeedBump.hpp"
#include "Button.hpp"
#include "SolidObj.hpp"

void rplayerobj::physics(unsigned long long tick)
{
    gameenv &env = *(gameenv*) this->m_env_ptr;
    level *env_level = env.get_active_level();
    room *env_room = env_level->get_active_room();

    // Controls
    v2 bp = m_primary_coords;
    if(env.up_kbpressed() && this->m_primary_coords.vert > 0)
        m_primary_coords.vert--;
    if(env.down_kbpressed() && this->m_primary_coords.vert + 1 < env_room->get_size().vert)
        m_primary_coords.vert++;

    // Disallow for clipping
    if(env_room->collision_with_base(this) == 1)
    {
        m_primary_coords = bp;
    }
    // On collision check and reset
    env_room->for_each<rsolidobj>([&](rsolidobj &t){
        if(m_primary_coords == t.coords() && t.is_solid())
        {
            m_primary_coords = bp;
        }
    });

    if(env.left_kbpressed() && this->m_primary_coords.hori > 0)
        m_primary_coords.hori--;
    if(env.right_kbpressed() && this->m_primary_coords.hori + 1 < env_room->get_size().hori)
        m_primary_coords.hori++;

    // Enable better controls by resetting individually
    if(env_room->collision_with_base(this) == 1)
    {
        m_primary_coords.hori = bp.hori;
    }
    // On collision check and reset
    env_room->for_each<rsolidobj>([&](rsolidobj &t){
        if(m_primary_coords == t.coords() && t.is_solid())
        {
            m_primary_coords.hori = bp.hori;
        }
    });

    // On collision eat inputs
    env_room->for_each<rspeedbump>([&](rspeedbump &t){
        static __UINT8_TYPE__ offset = 0u;
        if(bp == t.coords())
        {
            this->m_primary_coords = bp;
            offset++;
            if(offset % 5 == 0)
            {
                offset = 0;
                t.push_v2(&this->m_primary_coords);
            }
        }
    });

    // On collision press button
    env_room->for_each<rbuttonobj>([&](rbuttonobj &t){
        if(m_primary_coords == t.coords())
        {
            t.press(tick);
        }
    });
}
