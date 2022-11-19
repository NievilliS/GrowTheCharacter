/***********************************
|* GTC
|*  Grow the Character by Nievillis
|***********************************
|* TESTING
|***********************************
|* @file room.cpp
|* 
|* This file is responsible for:
|*  - C++ file for Room and room transition Class
|* This file depends on:
|*  -
|*
|***********************************/

/************************************
|* INCLUDE
|***********************************/

#include "room.hpp"
#include <regex>

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

const std::regex check_rx("^room.: *");
const std::regex index_rx("^room(.): *");
const std::regex line_rx("[^\n]+");
const std::regex tiles_rx("^tiles: ?([0-9]+ [0-9]+)? *");
const std::regex name_rx("^name: ?[^ ]+ *");

GTC::room GTC::room::parseFromString(const std::string str)
{  
    if(!std::regex_search(str, check_rx))
    {
        throw "Room init misformatting";
    }
    // Line iterator
    auto lines_begin = std::sregex_iterator(str.begin(), str.end(), line_rx);
    auto lines_end = std::sregex_iterator();

    for(auto lines_iter = lines_begin; lines_iter != lines_end; ++lines_iter)
    {
        std::smatch match_lines_iter = *lines_iter;
        auto str_line = match_lines_iter.str();
        
        std::cout << str_line << std::endl;

        if(std::regex_match(str_line, tiles_rx)) {
            //std 
            std::cout << "Detected.....,,.-,.-," << std::endl;
        }
    }
}

GTC::room::transition::transition(
    const unsigned int parent_room,
    const unsigned int to_room,
    const unsigned int x1,
    const unsigned int y1,
    const unsigned int x2,
    const unsigned int y2
) :
    parent_room(parent_room),
    to_room(to_room),
    x1(x1),
    x2(x2),
    y1(y1),
    y2(y2)
{
}

GTC::room::transition::~transition()
{
}

int GTC::room::transition::in_collision(const unsigned int x, const unsigned int y) const
{
    return ((x >= this->x1) && (x <= this->x2) && (y >= this->y1) && (y <= this->y2)) ? this->to_room : -1;
}
