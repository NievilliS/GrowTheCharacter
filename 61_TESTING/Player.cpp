#include "Player.hpp"
#include "GameEnvironment.hpp"

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
    if(env.left_kbpressed() && this->cr.hori > 0)
        cr.hori--;
    if(env.right_kbpressed() && this->cr.hori + 1 < env_room->getsize().hori)
        cr.hori++;
    if(env_room->collision_with_base(this) == 1)
    {
        cr = bp;
    }
}
