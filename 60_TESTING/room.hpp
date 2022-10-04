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

namespace GTC
{

/************************************
|* INCLUDE
|***********************************/
#include <iostream>
#include <string>

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
    
    public:
        ~room();
        room(const unsigned int, const unsigned int);

        static room parseFromString(const std::String);
};


};
#endif
