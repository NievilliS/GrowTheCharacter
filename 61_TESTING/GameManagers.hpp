#pragma once

#include <DUNE/DManager.hpp>
#include "config.hpp"

class graphicsmgr : public DManager::DManager
{
private:
    void *env_ptr;

public:
    graphicsmgr(void *env_ptr);

    virtual void init_user() override;
    virtual void run_user() override;
};

class physicsmgr : public DManager::DManager
{
private:
    void *env_ptr;

public:
    physicsmgr(void *env_ptr);

    virtual void init_user() override;
    virtual void run_user() override;
};
