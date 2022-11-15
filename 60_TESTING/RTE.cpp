#include "RTE.hpp"
#include <chrono>
#include <DUNE/DControlManager.hpp>

void RTE::stop()
{
    ((DManager::DControlManager*)(RTE::dcm_ptr))->stop();
}
