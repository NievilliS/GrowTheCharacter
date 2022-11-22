#pragma once

#include <regex>
#include "GameEnvironment.hpp"
#include "RoomMaker.hpp"

class gamemaker
{
protected:
    std::string raw_game_str;
    std::string charlevels{};
    gameenv *genv = nullptr;

    const std::regex room_rx{"^room(.): *"};
    const std::regex level_rx{"^level(.): *"};
    const std::regex comment_rx{"^//.+"};
    const std::regex line_rx{"[^\n]+"};
    const std::regex index_rx{"^index: *"};
    const std::regex end_rx{"^end *"};
    const std::regex endroom_rx{"^endroom *"};
    const std::regex endlevel_rx{"^endlevel *"};

public:
    gamemaker(const std::string _raw_game_str): raw_game_str(_raw_game_str) {}
    ~gamemaker()
    {
        if(genv != nullptr)
            delete genv;
    }

    gameenv *createenvironment()
    {
        int status = 0;
        int index_level = 0;
        int index_room = 0;
        auto lines_begin = std::sregex_iterator(raw_game_str.begin(), raw_game_str.end(), line_rx);
        auto lines_end = std::sregex_iterator();
        this->genv = new gameenv();
        std::smatch smat;
        std::string roombuff{};

        for(auto lines_iter = lines_begin; lines_iter != lines_end; ++lines_iter)
        {
            std::smatch match_lines_iter = *lines_iter;
            auto str_line = match_lines_iter.str();

            //Comment voids
            if(std::regex_match(str_line, comment_rx)) continue;

            if(status == 0)
            {                
                if(std::regex_match(str_line, index_rx))
                {
                    status = 1;
                    continue;
                }
                else if(std::regex_search(str_line, smat, level_rx))
                {
                    index_level = (int) smat[1].str()[0];
                    genv->subscribe_new_level(new level(index_level));
                    genv->set_active_level(genv->size()-1);
                    status = 2;
                    continue;
                }
            }
            else if(status == 1)
            {
                if(std::regex_match(str_line, end_rx))
                {
                    status = 0;
                    continue;
                }
                charlevels += str_line;
            }
            else if(status == 2)
            {
                if(std::regex_search(str_line, smat, room_rx))
                {
                    status = 3;
                    index_room = smat[1].str()[0];
                    roombuff = str_line + '\n';
                    continue;
                }
                else if(std::regex_match(str_line, endlevel_rx))
                {
                    status = 0;
                }
            }
            else if(status == 3)
            {
                if(std::regex_match(str_line, endroom_rx))
                {
                    level *lv = genv->get_active_level();
                    roommaker rm(roombuff);
                    lv->subscribe_new_room(rm.createroom(this->genv));
                    lv->set_active_room(lv->size()-1);
                    status = 2;
                    continue;
                }
                roombuff += str_line + '\n';
            }
        }

        return this->genv;
    }

};
