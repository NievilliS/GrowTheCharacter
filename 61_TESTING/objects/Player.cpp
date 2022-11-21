#include "Player.hpp"
#include "../GameEnvironment.hpp"
#include "Spinny.hpp"
#include "SpeedBump.hpp"

void rplayerobj::physics(unsigned long long tick)
{
    gameenv &env = *(gameenv*) this->env_ptr;
    room *env_room = env.get_active_room();

    // Controls
    v2 bp = cr;
    if(env.up_kbpressed() && this->cr.vert > 0)
        cr.vert--;
    if(env.down_kbpressed() && this->cr.vert + 1 < env_room->getsize().vert)
        cr.vert++;

    // Disallow for clipping
    if(env_room->collision_with_base(this) == 1)
    {
        cr = bp;
    }

    if(env.left_kbpressed() && this->cr.hori > 0)
        cr.hori--;
    if(env.right_kbpressed() && this->cr.hori + 1 < env_room->getsize().hori)
        cr.hori++;

    // On collision eat inputs
    env_room->for_each<rspeedbump>([&](rspeedbump &t){
        static __UINT8_TYPE__ offset = 0u;
        if(bp == t.coords())
        {
            this->cr = bp;
            offset++;
            if(offset % 5 == 0)
            {
                offset = 0;
                t.push_v2(&this->cr);
            }
        }
    });
    if(env_room->collision_with_base(this) == 1)
    {
        cr = bp;
    }
}
