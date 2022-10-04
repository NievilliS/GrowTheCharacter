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
#include "smartptr.hpp"

/************************************
|* NAMESPACE GTC
|***********************************/
namespace GTC
{

/************************************
|* CLASS room
|************************************
|* @class room
|* @brief Class manages and stores tile data
\***********************************/
class room
{
    private:
        GTC::smart_ptr<unsigned int> tiledata;
        unsigned int width;
        unsigned int height;
    
    public:
        ~room();
        room(const unsigned int, const unsigned int);

        static room parseFromString(const std::string);
};


};
#endif
