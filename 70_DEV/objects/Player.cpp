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
            ptenv->get_active_level()->get_active_room()->del_sensitive_objects();
            ptenv->set_active_index_level(nlx);
            ptenv->get_active_level()->set_active_index_room(nrx);
            ptenv->get_active_level()->get_active_room()->add_obj(pl);
            checkpoint *cp = ptenv->get_active_level()->get_active_room()->get_checkpoint();
            if(cp != nullptr)
                pl->set_checkpoint(cp);
        });
    }
    else
    {
        env.register_event_current(4, [env_level, nrx, pl, plprcr, cop]{
            *plprcr = cop;
            env_level->get_active_room()->del_sensitive_objects();
            env_level->set_active_index_room(nrx);
            env_level->get_active_room()->add_obj(pl);
            checkpoint *cp = env_level->get_active_room()->get_checkpoint();
            if(cp != nullptr)
                pl->set_checkpoint(cp);
        });
    }

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

template<typename T>
inline void room_foreach(rplayerobj *p, room *env_room, const unsigned long long tick, int xtra_par = 0)
{
    env_room->for_each<T>([&](T &t){
        p->plr_check_collision<T>(t, tick, xtra_par);
    });
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
    this->m_prev_pos = this->m_primary_coords;
    if (env.up_kbpressed() && this->m_can_move)
        this->m_primary_coords.vert--;
    if (env.down_kbpressed() && this->m_can_move)
        this->m_primary_coords.vert++;

    // Disallow for clipping
    int _env_room_coll = env_room->collision_with_base(this->m_primary_coords);
    if (_env_room_coll == 1 && !this->m_god_mode)
    {
        this->m_primary_coords = this->m_prev_pos;
    }
    else if (_env_room_coll == 2 && this->m_can_move && !this->m_god_mode)
    {
        check_ded(env_room->get_char_at_base(this->m_primary_coords));
    }
    // Room transition
    else if (_env_room_coll == 3)
    {
        roomtransition *rt = env_room->get_if_collide(this->m_primary_coords);
        if (!transition(env, env_level, env_room, this, rt, &this->m_primary_coords))
        {
            this->m_primary_coords = this->m_prev_pos;
        }
        else
            return false;
    }

    // On collision check and reset
    room_foreach<rsolidobj>(this, env_room, tick, 0);

    if (env.left_kbpressed() && this->m_can_move)
        this->m_primary_coords.hori--;
    if (env.right_kbpressed() && this->m_can_move)
        this->m_primary_coords.hori++;

    // Enable better controls by resetting individually
    _env_room_coll = env_room->collision_with_base(this->m_primary_coords);
    if (_env_room_coll == 1 && !this->m_god_mode)
    {
        this->m_primary_coords.hori = this->m_prev_pos.hori;
    }

    else if (_env_room_coll == 2 && this->m_can_move && !this->m_god_mode)
    {
        check_ded(env_room->get_char_at_base(this->m_primary_coords));
    }

    // Room transition
    else if (_env_room_coll == 3)
    {
        roomtransition *rt = env_room->get_if_collide(this->m_primary_coords);
        if (!transition(env, env_level, env_room, this, rt, &this->m_primary_coords))
        {
            this->m_primary_coords.hori = this->m_prev_pos.hori;
        }
        else
            return false;
    }

    // On collision check and reset horizontal
    room_foreach<rsolidobj>(this, env_room, tick, 1);

    // On collision eat inputs and reset moving ability + disable for buffering
    room_foreach<rspeedbump>(this, env_room, tick);

    // On collision press button
    room_foreach<rbuttonobj>(this, env_room, tick);

    // Teleporter
    room_foreach<rteleportobj>(this, env_room, tick);
    
    // Spinny obj
    room_foreach<rspinnyobj>(this, env_room, tick);

    // rolling obj
    room_foreach<rrollerobj>(this, env_room, tick);

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
    if(!this->m_can_move) return;
    gameenv &env = *(gameenv *)this->m_env_ptr;
    this->set_can_move(false);
    room *rm = env.get_active_level()->get_active_room();

    this->col = Pixel::RED;

    env.register_event_current(5, [&]{
        this->col = Pixel::DEFAULT;
        this->m_character = 0;
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
        Utils::do_trig(env_room->get_on_die_trig());
        env_room->remove_obj(this);
        env_room->del_sensitive_objects();
        this->m_primary_coords = target_pos;
        this->m_character = target_char;
        env_level->set_active_index_room(target_room_index);
        env.get_active_level()->get_active_room()->add_obj(this);
        this->set_can_move(true); });
}

template<>
void rplayerobj::plr_check_collision<>(rrollerobj &t, const unsigned long long tick, int xtra_par)
{
    if((t.coords() == this->m_primary_coords || t.coords() == this->m_prev_pos) && !this->m_god_mode)
    {
        this->check_ded(t.get_raw_character());
    }
}

template<>
void rplayerobj::plr_check_collision<>(rspinnyobj &t, const unsigned long long tick, int xtra_par)
{
    if(t.coords() == this->m_primary_coords && this->m_can_move && !this->m_god_mode)
    {
        this->check_ded(t.get_raw_character());
    }
}

template<>
void rplayerobj::plr_check_collision<>(rteleportobj &t, const unsigned long long tick, int xtra_par)
{
    if(!t.is_active() || !this->m_can_move || this->m_god_mode) return;
    if(this->m_primary_coords == t.coords() || (!t.is_one_way() && this->m_primary_coords == t.coords2()))
    {
        this->set_can_move(false);
        ((gameenv*) this->m_env_ptr)->register_event_current(5, [this, &t]{
            t.teleport(&this->m_primary_coords);
            this->set_can_move(true);
        });
        ((gameenv*) this->m_env_ptr)->register_event_current(40, [&t]{
            t.set_active();
        });
    }
}

template<>
void rplayerobj::plr_check_collision<>(rbuttonobj &t, const unsigned long long tick, int xtra_par)
{
    if(this->m_primary_coords == t.coords())
    {
        t.press(tick);
    }
}

template<>
void rplayerobj::plr_check_collision<>(rspeedbump &t, const unsigned long long tick, int xtra_par)
{
    static __UINT8_TYPE__ offset = 0u;
    if(!this->m_can_move)
        this->set_can_move(true);
    if(this->m_prev_pos == t.coords() && !this->m_god_mode)
    {
        this->set_can_move(false);
        this->m_primary_coords = this->m_prev_pos;
        offset++;
        if(offset % 5 == 0)
        {
            offset = 0;
            t.push_v2(&this->m_primary_coords);
        }
    }
}

template<>
void rplayerobj::plr_check_collision<>(rsolidobj &t, const unsigned long long tick, int xtra_par)
{
    if(this->m_god_mode) return;
    if(this->m_prev_pos == t.coords() && t.is_solid() && this->m_can_move)
    {
        this->die();
    }
    if(this->m_primary_coords == t.coords() && t.is_solid())
    {
        if(xtra_par == 1)
            this->m_primary_coords.hori = this->m_prev_pos.hori;
        else
            this->m_primary_coords = this->m_prev_pos;
    }
}
