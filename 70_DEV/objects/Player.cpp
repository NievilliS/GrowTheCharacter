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
    if(env.up_kbpressed())
        m_primary_coords.vert--;
    if(env.down_kbpressed())
        m_primary_coords.vert++;

    // Disallow for clipping
    if(env_room->collision_with_base(this) == 1)
    {
        m_primary_coords = bp;
    }
    // Room transition
    else if(env_room->collision_with_base(this) == 3)
    {
        roomtransition *rt = env_room->get_if_collide(m_primary_coords);
        if(rt != nullptr)
        {
            roomtransition *nx = rt->get_link();

            if(rt->do_next_level() || rt->do_prev_level())
            {
                int nrx = nx->get_room_index();
                int nlx = nx->get_level_index();

                env_room->cancel_phys();
                env_room->remove_obj(this);
                this->m_primary_coords = nx->get_coord_of_player();
                env.set_active_index_level(nlx);
                env.get_active_level()->set_active_index_room(nrx);
                env.get_active_level()->get_active_room()->add_obj(this);
            }
            else
            {
                int nrx = nx->get_room_index();
                env_room->cancel_phys();
                env_room->remove_obj(this);
                this->m_primary_coords = nx->get_coord_of_player();
                env_level->set_active_index_room(nrx);
                env_level->get_active_room()->add_obj(this);
            }
            return;
        }
        else
        {
           m_primary_coords = bp; 
        }
    }

    // On collision check and reset
    env_room->for_each<rsolidobj>([&](rsolidobj &t){
        if(m_primary_coords == t.coords() && t.is_solid())
        {
            m_primary_coords = bp;
        }
    });

    if(env.left_kbpressed())
        m_primary_coords.hori--;
    if(env.right_kbpressed())
        m_primary_coords.hori++;

    // Enable better controls by resetting individually
    if(env_room->collision_with_base(this) == 1)
    {
        m_primary_coords.hori = bp.hori;
    }

    // Room transition
    else if(env_room->collision_with_base(this) == 3)
    {
        roomtransition *rt = env_room->get_if_collide(m_primary_coords);
        if(rt != nullptr)
        {
            roomtransition *nx = rt->get_link();

            if(rt->do_next_level() || rt->do_prev_level())
            {
                int nrx = nx->get_room_index();
                int nlx = nx->get_level_index();

                env_room->cancel_phys();
                env_room->remove_obj(this);
                this->m_primary_coords = nx->get_coord_of_player();
                env.set_active_index_level(nlx);
                env.get_active_level()->set_active_index_room(nrx);
                env.get_active_level()->get_active_room()->add_obj(this);
            }
            else
            {
                int nrx = nx->get_room_index();
                env_room->cancel_phys();
                env_room->remove_obj(this);
                this->m_primary_coords = nx->get_coord_of_player();
                env_level->set_active_index_room(nrx);
                env_level->get_active_room()->add_obj(this);
            }
            return;
        }
        else
        {
           m_primary_coords = bp; 
        }
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

bool rplayerobj::can_move() const
{
    return this->m_can_move;
}

void rplayerobj::set_can_move(const bool b)
{
    
}
