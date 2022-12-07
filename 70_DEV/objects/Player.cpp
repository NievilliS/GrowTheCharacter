#include "Player.hpp"
#include "../GameEnvironment.hpp"
#include "Spinny.hpp"
#include "SpeedBump.hpp"
#include "Button.hpp"
#include "SolidObj.hpp"
#include "TeleportObj.hpp"
#include "Roller.hpp"

inline bool transition(gameenv &env, level *env_level, room *env_room, rplayerobj *pl, roomtransition *rt, v2 *plprcr)
{
    if (rt == nullptr)
        return false;
    roomtransition *nx = rt->get_link();
    if (nx == nullptr)
        return false;

    int nrx = nx->get_room_index();
    gameenv *ptenv = &env;
    v2 cop = nx->get_coord_of_player();

    if (rt->do_next_level() || rt->do_prev_level())
    {
        int nlx = nx->get_level_index();
        env.register_event_current(4, [ptenv, nlx, nrx, pl, plprcr, cop]{
            *plprcr = cop;
            ptenv->set_active_index_level(nlx);
            ptenv->get_active_level()->set_active_index_room(nrx);
            ptenv->get_active_level()->get_active_room()->cancel_phys();
            ptenv->get_active_level()->get_active_room()->add_obj(pl);
        });
    }
    else
    {
        env.register_event_current(4, [env_level, nrx, pl, plprcr, cop]{
            *plprcr = cop;
            env_level->set_active_index_room(nrx);
            env_level->get_active_room()->cancel_phys();
            env_level->get_active_room()->add_obj(pl);
        });
    }

    checkpoint *cp = env.get_active_level()->get_active_room()->get_checkpoint();
    if (cp != nullptr)
        pl->set_checkpoint(cp);

    return true;
}

void rplayerobj::check_ded(const unsigned char base_c)
{
    gameenv *env = (gameenv *)this->m_env_ptr;

    const int base_lvl = env->get_damage_index(base_c);
    const int plyr_lvl = env->get_damage_index(this->m_character);

    //Dying
    if(base_lvl > plyr_lvl)
    {
        this->die();
        return;
    }
    //Absorbing and leveling up
    else if(base_lvl == plyr_lvl)
    {
        // TODO: Effect and base char deletion
        this->m_character = env->get_char_at_index(plyr_lvl+1);

        this->col = Pixel::GREEN;

        for(int i = 1; i < 3; i++)
        {
            env->register_event_current(i * 4, [&]{
                this->col = Pixel::GREEN;
            });
        }

        for(int i = 0; i < 3; i++)
        {
            env->register_event_current(i * 4 + 2, [&]{
                this->col = Pixel::DEFAULT;
            });
        }
    }
    //Ignoring otherwise
}

bool rplayerobj::physics(unsigned long long tick)
{
    gameenv &env = *(gameenv *)this->m_env_ptr;
    level *env_level = env.get_active_level();
    room *env_room = env_level->get_active_room();

    if (env.r_kbpressed())
    {
        die();
        return true;
    }

    // Controls
    v2 bp = m_primary_coords;
    if (env.up_kbpressed() && m_can_move)
        m_primary_coords.vert--;
    if (env.down_kbpressed() && m_can_move)
        m_primary_coords.vert++;

    // Disallow for clipping
    int _env_room_coll = env_room->collision_with_base(this->m_primary_coords);
    if (_env_room_coll == 1)
    {
        m_primary_coords = bp;
    }
    else if (_env_room_coll == 2 && this->m_can_move)
    {
        check_ded(env_room->get_char_at_base(this->m_primary_coords));
    }
    // Room transition
    else if (_env_room_coll == 3)
    {
        roomtransition *rt = env_room->get_if_collide(m_primary_coords);
        if (!transition(env, env_level, env_room, this, rt, &this->m_primary_coords))
        {
            m_primary_coords = bp;
        }
        else
            return false;
    }

    // On collision check and reset
    env_room->for_each<rsolidobj>([&](rsolidobj &t)
                                  {
        if(bp == t.coords() && t.is_solid() && this->m_can_move)
        {
            die();
        }
        if(m_primary_coords == t.coords() && t.is_solid())
        {
            m_primary_coords = bp;
        } });

    if (env.left_kbpressed() && m_can_move)
        m_primary_coords.hori--;
    if (env.right_kbpressed() && m_can_move)
        m_primary_coords.hori++;

    // Enable better controls by resetting individually
    _env_room_coll = env_room->collision_with_base(this->m_primary_coords);
    if (_env_room_coll == 1)
    {
        m_primary_coords.hori = bp.hori;
    }

    else if (_env_room_coll == 2 && this->m_can_move)
    {
        check_ded(env_room->get_char_at_base(this->m_primary_coords));
    }

    // Room transition
    else if (_env_room_coll == 3)
    {
        roomtransition *rt = env_room->get_if_collide(m_primary_coords);
        if (!transition(env, env_level, env_room, this, rt, &this->m_primary_coords))
        {
            m_primary_coords.hori = bp.hori;
        }
        else
            return false;
    }

    // On collision check and reset
    env_room->for_each<rsolidobj>([&](rsolidobj &t)
                                  {
        if(bp == t.coords() && t.is_solid() && this->m_can_move)
        {
            die();
        }
        if(m_primary_coords == t.coords() && t.is_solid())
        {
            m_primary_coords.hori = bp.hori;
        } });

    // On collision eat inputs and reset moving ability + disable for buffering
    env_room->for_each<rspeedbump>([&](rspeedbump &t)
                                   {
        static __UINT8_TYPE__ offset = 0u;
        if(bp == t.coords())
        {
            this->set_can_move(false);
            this->m_primary_coords = bp;
            offset++;
            if(offset % 5 == 0)
            {
                offset = 0;
                t.push_v2(&this->m_primary_coords);
                env.register_event_current(1, [this, env_room]{
                    bool rt = false;
                    env_room->for_each<rspeedbump>([this, &rt](rspeedbump &t){
                        if(!rt && this->coords() == t.coords())
                        {
                            rt = true;
                            return;
                        }
                    });
                    if(!rt)
                        this->set_can_move(true);
                });
            }
        } });

    // On collision press button
    env_room->for_each<rbuttonobj>([&](rbuttonobj &t)
                                   {
        if(m_primary_coords == t.coords())
        {
            t.press(tick);
        } });

    // Teleporter
    env_room->for_each<rteleportobj>([&](rteleportobj &t)
                                     {
        if(!t.is_active()) return;
        if(!this->m_can_move) return;
        if(this->m_primary_coords == t.coords() || (!t.is_one_way() && this->m_primary_coords == t.coords2()))
        {
            this->set_can_move(false);
            env.register_event_current(5, [this, &t]{
                t.teleport(&this->m_primary_coords);
                this->set_can_move(true);
            });

            env.register_event_current(40, [&t]{
                t.set_active();
            });
        } });
    
    // Spinny obj
    env_room->for_each<rspinnyobj>([&](rspinnyobj &t){
        if(t.coords() == this->m_primary_coords && this->m_can_move)
        {
            check_ded(t.get_raw_character());
        }
    });

    // rolling obj
    env_room->for_each<rrollerobj>([&](rrollerobj &t){
        if(t.coords() == this->m_primary_coords || t.coords() == bp)
        {
            check_ded(t.get_raw_character());
        }
    });

    return true;
}

bool rplayerobj::can_move() const
{
    return this->m_can_move;
}

void rplayerobj::set_can_move(const bool b)
{
    this->m_can_move = b;
}

void rplayerobj::set_checkpoint(void *_checkpoint_ptr)
{
    checkpoint *cp = (checkpoint *)_checkpoint_ptr;
    cp->m_player_char = this->m_character;
    this->m_checkpoint_ptr = cp;
}

void rplayerobj::die()
{
    gameenv &env = *(gameenv *)this->m_env_ptr;
    this->set_can_move(false);
    room *rm = env.get_active_level()->get_active_room();

    this->col = Pixel::RED;

    env.register_event_current(5, [&]{
        this->col = Pixel::DEFAULT;
        this->m_character = 1;
    });

    /* TODO: Effect */
    env.register_event_current(20, [&]
                               {
        gameenv &env = *(gameenv*) this->m_env_ptr;
        level *env_level = env.get_active_level();
        room *env_room = env_level->get_active_room();
        checkpoint *cp = (checkpoint*) this->m_checkpoint_ptr;
        room *target_room = (room*) cp->m_room_ptr;
        int target_room_index = target_room->get_index();
        v2 target_pos = cp->m_room_pos;
        char target_char = cp->m_player_char;

        env_room->cancel_phys();
        env_room->remove_obj(this);
        this->m_primary_coords = target_pos;
        this->m_character = target_char;
        env_level->set_active_index_room(target_room_index);
        env.get_active_level()->get_active_room()->add_obj(this);
        this->set_can_move(true); });
}
