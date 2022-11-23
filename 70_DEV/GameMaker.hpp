#pragma once

#include <regex>
#include "GameEnvironment.hpp"
#include "RoomMaker.hpp"

class gamemaker
{
protected:
    std::string m_raw_game_str;
    std::string m_index_buffer{};
    gameenv *m_genv = nullptr;

    const std::regex c_room_rx{"^room(.): *"};
    const std::regex c_level_rx{"^level(.): *"};
    const std::regex c_comment_rx{"^//.+"};
    const std::regex c_line_rx{"[^\n]+"};
    const std::regex c_index_rx{"^index: *"};
    const std::regex c_end_rx{"^end *"};
    const std::regex c_endroom_rx{"^endroom *"};
    const std::regex c_endlevel_rx{"^endlevel *"};

public:
    gamemaker(const std::string _raw_game_str): m_raw_game_str(_raw_game_str) {}
    ~gamemaker()
    {
        if(m_genv != nullptr)
            delete m_genv;
    }

    gameenv *createenvironment()
    {
        int status = 0;
        int index_level = 0;
        int index_room = 0;
        auto lines_begin = std::sregex_iterator(m_raw_game_str.begin(), m_raw_game_str.end(), c_line_rx);
        auto lines_end = std::sregex_iterator();
        this->m_genv = new gameenv();
        std::smatch smat;
        std::string roombuff{};

        for(auto lines_iter = lines_begin; lines_iter != lines_end; ++lines_iter)
        {
            std::smatch match_lines_iter = *lines_iter;
            auto str_line = match_lines_iter.str();

            //Comment voids
            if(std::regex_match(str_line, c_comment_rx)) continue;

            if(status == 0)
            {                
                if(std::regex_match(str_line, c_index_rx))
                {
                    status = 1;
                    continue;
                }
                else if(std::regex_search(str_line, smat, c_level_rx))
                {
                    index_level = (int) smat[1].str()[0];
                    this->m_genv->subscribe_new_level(new level(index_level));
                    this->m_genv->set_active_level(this->m_genv->size()-1);
                    status = 2;
                    continue;
                }
            }
            else if(status == 1)
            {
                if(std::regex_match(str_line, c_end_rx))
                {
                    status = 0;
                    continue;
                }
                m_index_buffer += str_line;
            }
            else if(status == 2)
            {
                if(std::regex_search(str_line, smat, c_room_rx))
                {
                    status = 3;
                    index_room = smat[1].str()[0];
                    roombuff = str_line + '\n';
                    continue;
                }
                else if(std::regex_match(str_line, c_endlevel_rx))
                {
                    status = 0;
                }
            }
            else if(status == 3)
            {
                if(std::regex_match(str_line, c_endroom_rx))
                {
                    level *lv = this->m_genv->get_active_level();
                    roommaker rm(roombuff);
                    lv->subscribe_new_room(rm.createroom(this->m_genv));
                    lv->set_active_room(lv->size()-1);
                    status = 2;
                    continue;
                }
                roombuff += str_line + '\n';
            }
        }

        return this->m_genv;
    }

};
