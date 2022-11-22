#include "DisappearingWall.hpp"
#include "Utils.hpp"

void rdisappearingwall::physics(unsigned long long tick)
{
    if(Utils::is_trig(dID))
    {
        this->col = Pixel::BLACK;
        this->solid = false;
    }
    if(Utils::is_trig(rID))
    {
        this->col = this->col_a;
        this->solid = true;
    }
}

bool rdisappearingwall::is_solid()
{
    if(Utils::is_trig(dID))
    {
        this->col = Pixel::BLACK;
        this->solid = false;
    }
    if(Utils::is_trig(rID))
    {
        this->col = this->col_a;
        this->solid = true;
    }
    return this->solid;
}
