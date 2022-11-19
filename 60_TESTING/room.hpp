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

/************************************
|* NAMESPACE GTC
|***********************************/


/************************************
|* CLASS room
|************************************
|* @class room
|* @brief Class manages and stores tile data
\***********************************/
class room
{
    public:
        class transition
        {
            private:
                unsigned int x1, x2, y1, y2;
                unsigned int parent_room, to_room;

            public:
                ~transition();
                transition(const unsigned int, const unsigned int, const unsigned int, const unsigned int, const unsigned int, const unsigned int);

                int in_collision(const unsigned int, const unsigned int) const;
        };

    private:
        smart_ptr<unsigned int> tiledata;
        std::vector<smart_ptr<gameobj>> objects;
        std::vector<smart_ptr<transition>> transitiondata;
        std::string name;
        unsigned int width;
        unsigned int height;
    
    public:
        ~room();
        room(const unsigned int, const unsigned int);

        static room parseFromString(const std::string);
};

#endif
