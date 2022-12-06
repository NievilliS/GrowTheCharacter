#pragma once

#include <regex>
#include "GameEnvironment.hpp"
#include "RoomMaker.hpp"
#include "objects/RoomTransition.hpp"

class gamemaker
{
protected:
    std::string m_raw_game_str;
    std::string m_index_buffer{};
    gameenv *m_genv = nullptr;
    std::vector<roomtransition *> m_transitions;

    const std::regex c_room_rx{"^ *room([0-9]+): *"};
    const std::regex c_level_rx{"^ *level([0-9]+): *"};
    const std::regex c_comment_rx{"^ *//.+"};
    const std::regex c_line_rx{"[^\n]+"};
    const std::regex c_index_rx{"^ *index: *"};
    const std::regex c_end_rx{"^ *end *"};
    const std::regex c_endroom_rx{"^ *endroom *"};
    const std::regex c_endlevel_rx{"^ *endlevel *"};
    const std::regex c_include_rx{"^include \"([^ ]+[.]dat)\""};

public:
    gamemaker(const std::string _raw_game_str) : m_raw_game_str(_raw_game_str) {}
    ~gamemaker()
    {
        if (m_genv != nullptr)
            delete m_genv;
    }

    gameenv *createenvironment();
};

gameenv *gamemaker::createenvironment()
{
    // PREPROCESSOR

    int status = 0;
    int index_level = 0;
    int index_room = 0;
    auto lines_begin = std::sregex_iterator(m_raw_game_str.begin(), m_raw_game_str.end(), c_line_rx);
    auto lines_end = std::sregex_iterator();
    this->m_genv = new gameenv();
    std::smatch smat;
    std::string roombuff{};

    for (auto lines_iter = lines_begin; lines_iter != lines_end; ++lines_iter)
    {
        std::smatch match_lines_iter = *lines_iter;
        auto str_line = match_lines_iter.str();

        // Comment voids
        if (std::regex_match(str_line, c_comment_rx))
            continue;

        if (status == 0)
        {
            if (std::regex_match(str_line, c_index_rx))
            {
                status = 1;
                continue;
            }
            else if (std::regex_search(str_line, smat, c_level_rx))
            {
                index_level = std::atoi(smat[1].str().c_str());
                if (!this->m_genv->set_active_index_level(index_level))
                {
                    this->m_genv->subscribe_new_level(new level(index_level));
                    this->m_genv->set_active_level(this->m_genv->size() - 1);
                }
                status = 2;
                continue;
            }
        }
        else if (status == 1)
        {
            if (std::regex_match(str_line, c_end_rx))
            {
                status = 0;
                continue;
            }
            m_index_buffer += str_line;
        }
        else if (status == 2)
        {
            if (std::regex_search(str_line, smat, c_room_rx))
            {
                status = 3;
                index_room = std::atoi(smat[1].str().c_str());
                roombuff = str_line + '\n';
                continue;
            }
            else if (std::regex_match(str_line, c_endlevel_rx))
            {
                this->m_genv->get_active_level()->set_active_room(0);
                status = 0;
            }
        }
        else if (status == 3)
        {
            if (std::regex_match(str_line, c_endroom_rx))
            {
                level *lv = this->m_genv->get_active_level();
                roommaker rm(index_level, &m_transitions, roombuff);
                lv->subscribe_new_room(rm.createroom(this->m_genv));
                lv->set_active_room(lv->size() - 1);
                status = 2;
                continue;
            }
            roombuff += str_line + '\n';
        }
    }

    //! Linking step
    for (auto i = m_transitions.begin(); i != m_transitions.end(); i++)
    {
        roomtransition *&active = *i;
        if (active->is_linked())
            continue;

        for (auto j = m_transitions.begin(); j != m_transitions.end(); j++)
        {
            roomtransition *&cmp = *j;
            if (active == cmp)
                continue;
            if (cmp->is_linked())
                continue;

            if (active->do_next_level() && cmp->do_prev_level())
            {
                if (active->get_level_index() + 1 == cmp->get_level_index())
                {
                    active->set_link(cmp);
                    cmp->set_link(active);
                    break;
                }
            }

            else if (active->get_level_index() == cmp->get_level_index())
            {
                if (active->get_meta_index() == cmp->get_meta_index() && !active->do_next_level() && !active->do_prev_level() && !cmp->do_next_level() && !cmp->do_prev_level())
                {
                    active->set_link(cmp);
                    cmp->set_link(active);
                    break;
                }
            }
        }
    }

    //!! Checkpoint step
    // for(auto i = 0; i < )

    this->m_transitions.clear();
    this->m_genv->set_active_level(0);

    return this->m_genv;
}
