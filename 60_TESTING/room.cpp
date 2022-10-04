/***********************************
|* GTC
|*  Grow the Character by Nievillis
|***********************************
|* TESTING
|***********************************
|* @file room.cpp
|* 
|* This file is responsible for:
|*  - C++ file for Room Class
|* This file depends on:
|*  -
|*
|***********************************/

/************************************
|* INCLUDE
|***********************************/

#include "room.hpp"

/************************************
|* CLASS room definition 
|************************************
|* @class room
|* @brief Class manages and stores tile data
\***********************************/

/**
 * @fn GTC::room::room(const unsigned int width, const unsigned int height)
 * @brief Constructor that allocates memory for the room data
 */
GTC::room::room(const unsigned int width, const unsigned int height) :
    width(width),
    height(height),
    tiledata(GTC::smart_ptr<unsigned int> (new unsigned int[width * height]))
{
}

GTC::room::~room()
{
}

GTC::room GTC::room::parseFromString(const std::string str)
{   
}
