/***********************************
|* GTC
|*  Grow the Character by Nievillis
|***********************************
|* TESTING
|***********************************
|* @file room.hpp
|*
|* This file is responsible for:
|*  - Header for Room Class
|* This file depends on:
|*  -
|*
|***********************************/

#ifndef __GTC_ROOM_H_
#define __GTC_ROOM_H_

/************************************
|* INCLUDE
|***********************************/
#include <iostream>
#include <string>
#include <functional>

namespace GTC
{

/************************************
|* DEFINITIONS
|***********************************/
/************************************
|* CLASS room
|************************************
|* @class room
|* @brief Class manages and stores tile data
|***********************************/
class room
{
private:
    unsigned short *tiledata;
    unsigned int width;
    unsigned int height;
    void test() {std::cout << "TEST" << std::endl; std::cout << "This value is equal to " << width << std::endl;}
public:
    ~room() {}
    room(const unsigned int _W, const unsigned int _H): width(_W), height(_H) {}
    static room parseFromString(const std::string);
    
    void (*retadr())(void)
    {
        auto fptr = &GTC::room::test;
        void *out = (void*&) fptr;
        void (GTC::room::*pp) (void) = (void (GTC::room::*&)(void)) fptr;
        auto ret = (void (*)(void)) fptr;
        return ret;
    }
};

};
#endif
