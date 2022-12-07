#include "DisappearingWall.hpp"
#include "Utils.hpp"

bool rdisappearingwall::physics(unsigned long long tick)
{
    if (Utils::is_trig(m_press_ID))
    {
        this->col = Pixel::BLACK;
        this->m_solid = false;
    }
    if (Utils::is_trig(m_release_ID))
    {
        this->col = this->m_appear_color;
        this->m_solid = true;
    }

    return true;
}

bool rdisappearingwall::is_solid()
{
    if (Utils::is_trig(m_press_ID))
    {
        this->col = Pixel::BLACK;
        this->m_solid = false;
    }
    if (Utils::is_trig(m_release_ID))
    {
        this->col = this->m_appear_color;
        this->m_solid = true;
    }
    return this->m_solid;
}
