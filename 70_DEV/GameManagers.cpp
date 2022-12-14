#include "GameEnvironment.hpp"

/** GRAPHICS MANAGER STUFF **/

graphicsmgr::graphicsmgr(void *env_ptr) :
    m_env_ptr(env_ptr),
    DManager(DManagerFlags{
        .can_skip = true,
        .log_lag = false,
        .target_period = std::chrono::milliseconds((int)(1000.0 / GRAPHICS_PER_SEC)),
        .delay_target_period = std::chrono::milliseconds((int)(1000.0 / PHYSICS_PER_SEC) / 2),
        .log_ela = false,
        .name = "Graphics Manager",
        .adress_millis = &(((gameenv*) env_ptr)->m_g_e)
    })
{
}

void graphicsmgr::init_user()
{
    std::cout << "Starting " << this->flags.name << std::endl;
}

void graphicsmgr::run_user()
{
    static gameenv &ge = *(gameenv *)m_env_ptr;
    level *lv = ge.get_active_level();
    room *rm = lv->get_active_room();
    if (rm != nullptr)
    {
        rm->draw(ge.get_tick());
    }
}

/** PHYSICS MANAGER STUFF **/

physicsmgr::physicsmgr(void *env_ptr) :
    m_env_ptr(env_ptr),
    DManager(DManagerFlags{
        .can_skip = false,
        .log_lag = false,
        .target_period = std::chrono::milliseconds((int)(1000.0 / PHYSICS_PER_SEC)),
        .delay_target_period = std::chrono::milliseconds(0),
        .log_ela = false,
        .name = "Physics Manager",
        .adress_millis = &(((gameenv*) env_ptr)->m_p_e)
    })
{
}

void physicsmgr::init_user()
{
    std::cout << "Starting " << this->flags.name << std::endl;
}

void physicsmgr::run_user()
{
    static gameenv &ge = *(gameenv *)m_env_ptr;
    level *lv = ge.get_active_level();
    room *rm = lv->get_active_room();
    if (rm != nullptr)
    {
        rm->physics(ge.iterate_tick());
    }
}
