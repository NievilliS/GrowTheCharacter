/***********************************
|* GTC
|*  Grow the Character by Nievillis
|***********************************
|* TESTING
|***********************************
|* @file room.hpp
|* 
|* This file is responsible for:
|*  - IO
|* This file depends on:
|*  - @ref room.hpp
|*
|***********************************/

/************************************
|* INCLUDE
|***********************************/

#include "room.hpp"

/************************************
|* MAIN fct
|************************************
|* @fn 
|*
\***********************************/
int main(int argc, char **argv)
{
    std::string str = "room0:\n";
    str += argv[1];
    try {
        (void) GTC::room::parseFromString(str);
    }
    catch(const char *e) {
        std::cerr << e << std::endl;
    }
}

