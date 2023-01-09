#include "DisappearingWall.hpp"
#include "Utils.hpp"

bool rdisappearingwall::physics(unsigned long long tick)
{
    is_solid();
    return true;
}

bool rdisappearingwall::is_solid()
{
    if (Utils::is_trig(m_press_ID))
    {
        this->col = Pixel::BLACK;
        this->m_solid = false;
        this->m_character = 0;
    }
    if (Utils::is_trig(m_release_ID))
    {
        this->col = this->m_appear_color;
        this->m_solid = true;
        this->m_character = '#';
    }
    return this->m_solid;
}
