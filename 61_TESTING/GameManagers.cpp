#include "GameEnvironment.hpp"

/** GRAPHICS MANAGER STUFF **/

graphicsmgr::graphicsmgr(void *env_ptr): env_ptr(env_ptr),
    DManager(DManagerFlags{
        .can_skip = true,
        .log_lag = false,
        .target_period = std::chrono::milliseconds((int) (1000.0 / GRAPHICS_PER_SEC)),
        .delay_target_period = std::chrono::milliseconds((int) (1000.0 / PHYSICS_PER_SEC) / 2),
        .log_ela = false,
        .name = "Graphics Manager",
        .adress_millis = nullptr
    })
{
}

void graphicsmgr::init_user()
{
    std::cout << "Starting " << this->flags.name << std::endl;
}

void graphicsmgr::run_user()
{
    gameenv &ge = *(gameenv*) env_ptr;
    room *rm = ge.get_active_room();
    rm->x = this->milli_last_ela;
    if(rm != nullptr)
    {
        rm->draw(ge.get_tick());
    }
}

/** PHYSICS MANAGER STUFF **/

physicsmgr::physicsmgr(void *env_ptr): env_ptr(env_ptr),
    DManager(DManagerFlags{
        .can_skip = false,
        .log_lag = false,
        .target_period = std::chrono::milliseconds((int) (1000.0 / PHYSICS_PER_SEC)),
        .delay_target_period = std::chrono::milliseconds(0),
        .log_ela = false,
        .name = "Physics Manager",
        .adress_millis = nullptr
    })
{
}

void physicsmgr::init_user()
{
    std::cout << "Starting " << this->flags.name << std::endl;
}

void physicsmgr::run_user()
{
    gameenv &ge = *(gameenv*) env_ptr;
    room *rm = ge.get_active_room();
    rm->y = this->milli_last_ela;
    if(rm != nullptr)
    {
        rm->physics(ge.iterate_tick());
    }
}
