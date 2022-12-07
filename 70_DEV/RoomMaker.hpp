#pragma once

#include <regex>
#include "GameEnvironment.hpp"
#include "objects/Player.hpp"
#include "objects/SpeedBump.hpp"
#include "objects/Button.hpp"
#include "objects/DisappearingWall.hpp"
#include "objects/Spinny.hpp"
#include "objects/TeleportObj.hpp"
#include "objects/Roller.hpp"
#include "objects/RollerSpawner.hpp"

const Pixel::Color &get_color(std::string st)
{
    if (st.compare("BLACK") == 0)
    {
        return Pixel::BLACK;
    }
    else if (st.compare("RED") == 0)
    {
        return Pixel::RED;
    }
    else if (st.compare("GREEN") == 0)
    {
        return Pixel::GREEN;
    }
    else if (st.compare("YELLOW") == 0)
    {
        return Pixel::YELLOW;
    }
    else if (st.compare("BLUE") == 0)
    {
        return Pixel::BLUE;
    }
    else if (st.compare("MAGENTA") == 0)
    {
        return Pixel::MAGENTA;
    }
    else if (st.compare("CYAN") == 0)
    {
        return Pixel::CYAN;
    }
    else if (st.compare("WHITE") == 0)
    {
        return Pixel::WHITE;
    }
    return Pixel::DEFAULT;
}

class roommaker
{
protected:
    const int m_level_index;
    std::vector<roomtransition *> *m_transitions;
    std::string m_raw_room_str;

    const std::regex
        c_check_rx{"^ *room.: *"},
        c_index_rx{"^ *room(.): *"},
        c_line_rx{"[^\n]+"},
        c_tiles_rx{"^ *tiles: *"},
        c_name_rx{"^ *name: ?([^ ]+) *"},
        c_comment_rx{"^ *//.*"},
        c_end_rx{"^ *end *"},
        c_bcol_rx{"^ *bgrcol: ?([^ ]+) *"},
        c_tcol_rx{"^ *txtcol: ?([^ ]+) *"},
        c_tcol2_rx{"^ *txtcol: ?([^ ]+) ?% ?([^ ]+) *"},
        c_gsub_rx{"^ *gsub: *"},
        c_class_rx{"^ *class: *"},
        c_transition_rx{"^ *transitions: *"},
        c_transition_gsub_rx{"^ *(.)=([0-9]+) *"},
        c_checkpoint_rx{"^ *checkpoint: ([0-9]+) ([0-9]+) *"},

        c_cl_start_rx{"^ *start ([0-9x]+) ([0-9y]+) (.) *"},
        c_cl_area_rx{"^ *area ([0-9]+) ([0-9]+) ([0-9]+) ([0-9]+) +.+"},
        c_cl_speed_rx{" *(.)speed ([0-9x]+) ([0-9y]+) *"},
        c_cl_roller_rx{" *(.)roller ([0-9x]+) ([0-9y]+) (.) ([0-9.]+) *"},
        c_cl_rollerspawner_rx{" *(.)rollerspawner ([0-9x]+) ([0-9y]+) (.) (.) ([0-9]+) ([0-9.]+) *"},
        c_cl_button_rx{" *button ([0-9x]+) ([0-9y]+) (-?[0-9]+) (l?)([0-9]+) (l?)([0-9]+) *"},
        c_cl_toggwall_rx{" *(t|h)oggwall ([0-9x]+) ([0-9y]+) (l?)([0-9]+) (l?)([0-9]+) *"},
        c_cl_spinner_rx{" *spinner ([0-9x]+) ([0-9y]+) (.) ([-]?[0-9.]+) ([0-9.]+) ([0-9]+) *"},
        c_cl_teleporter_rx{"^ *teleporter ([0-9]+) ([0-9]+) ([0-9]+) ([0-9]+) ([yn]) *"};

public:
    roommaker(const int _level_index, std::vector<roomtransition *> *_transitions, const std::string _raw_room_string) : m_raw_room_str(_raw_room_string), m_transitions(_transitions), m_level_index(_level_index) {}

    room *createroom(void *env_ptr);
};

room *roommaker::createroom(void *env_ptr)
{
    std::smatch smat;

    if (!std::regex_search(m_raw_room_str, smat, c_index_rx))
    {
        throw "Room init misformatting";
    }

    room *ret = new room(smat[1].str()[0]);
    int status = 0;
    auto lines_begin = std::sregex_iterator(m_raw_room_str.begin(), m_raw_room_str.end(), c_line_rx);
    auto lines_end = std::sregex_iterator();
    std::string tile_str{};
    Pixel::Color tcol2 = Pixel::DEFAULT;
    v2 wh{};

    for (auto lines_iter = lines_begin; lines_iter != lines_end; ++lines_iter)
    {
        std::smatch match_lines_iter = *lines_iter;
        auto str_line = match_lines_iter.str();

        if (std::regex_match(str_line, c_comment_rx))
        {
            continue;
        }

        if (status == 0)
        {
            if (std::regex_search(str_line, smat, c_name_rx))
            {
                std::string name = smat[1].str();
                ret->set_name(name);
            }
            else if (std::regex_search(str_line, smat, c_bcol_rx))
            {
                ret->set_background_color(get_color(smat[1].str()));
            }
            else if (std::regex_search(str_line, smat, c_tcol2_rx))
            {
                ret->set_foreground_color(get_color(smat[1].str()));
                tcol2 = get_color(smat[2].str());
            }
            else if (std::regex_search(str_line, smat, c_tcol_rx))
            {
                ret->set_foreground_color(get_color(smat[1].str()));
            }
            else if (std::regex_match(str_line, c_tiles_rx))
            {
                tile_str.clear();
                status = 1;
            }
            else if (std::regex_match(str_line, c_gsub_rx))
            {
                tile_str.clear();
                status = 2;
            }
            else if (std::regex_match(str_line, c_class_rx))
            {
                status = 3;
            }
            else if (std::regex_match(str_line, c_transition_rx))
            {
                status = 4;
            }
            else if (std::regex_search(str_line, smat, c_checkpoint_rx))
            {
                ret->set_checkpoint(v2{std::atoi(smat[2].str().c_str()), std::atoi(smat[1].str().c_str())});
            }
        }
        else if (status == 1)
        {
            if (std::regex_match(str_line, c_end_rx))
            {
                status = 0;
                {
                    pixelstr ps{};
                    Pixel::copy_string_to_pixel_string(ps, tile_str);
                    wh = room::get_pixelstr_dim(ps);
                }
                std::string rt{};

                for (int y = 1; y < wh.vert - 2; y++)
                {
                    for (int x = 1; x < wh.hori - 1; x++)
                    {
                        rt += tile_str[x + y * (wh.hori + 1)];
                    }
                    if (y < wh.vert - 3)
                        rt += '\n';
                }

                // Upper transition
                int _in_active = -3;
                int _len = 0;

                for (int x = 1; x < wh.hori - 1; x++)
                {
                    if (tile_str[x] != '#' && _len <= 0)
                    {
                        _in_active = (int)tile_str[x];
                        _len++;
                    }
                    else if (tile_str[x] == _in_active)
                    {
                        _len++;
                    }
                    else
                    {
                        if (_len > 0)
                        {
                            roomtransition *rt = new roomtransition(x - _len - 1, -1, _len, 1, ret->get_index(), m_level_index, 1, -_in_active);
                            ret->add_transition(rt);
                            m_transitions->push_back(rt);
                            _len = 0;
                            _in_active = -3;
                        }

                        if (tile_str[x] != '#')
                        {
                            _in_active = (int)tile_str[x];
                            _len++;
                        }
                    }
                }

                if (_len > 0)
                {
                    roomtransition *rt = new roomtransition(wh.hori - _len - 3, -1, _len, 1, ret->get_index(), m_level_index, 1, -_in_active);
                    ret->add_transition(rt);
                    m_transitions->push_back(rt);
                    _len = 0;
                    _in_active = -3;
                }

                // Lower Transition
                for (int x = 1; x < wh.hori - 1; x++)
                {
                    if (tile_str[x] != '#' && _len <= 0)
                    {
                        _in_active = (int)tile_str[x + (wh.vert - 2) * (wh.hori + 1)];
                        _len++;
                    }
                    else if (tile_str[x + (wh.vert - 2) * (wh.hori + 1)] == _in_active)
                    {
                        _len++;
                    }
                    else
                    {
                        if (_len > 0)
                        {
                            roomtransition *rt = new roomtransition(x - _len - 1, (wh.vert - 3), _len, 1, ret->get_index(), m_level_index, 0, -_in_active);
                            ret->add_transition(rt);
                            m_transitions->push_back(rt);
                            _len = 0;
                            _in_active = -3;
                        }

                        if (tile_str[x + (wh.vert - 2) * (wh.hori + 1)] != '#')
                        {
                            _in_active = tile_str[x + (wh.vert - 2) * (wh.hori + 1)];
                            _len++;
                        }
                    }
                }

                if (_len > 0)
                {
                    roomtransition *rt = new roomtransition(wh.hori - _len - 3, (wh.vert - 3), _len, 1, ret->get_index(), m_level_index, 0, -_in_active);
                    ret->add_transition(rt);
                    m_transitions->push_back(rt);
                    _len = 0;
                    _in_active = -3;
                }

                // Left transitions
                for (int y = 1; y < wh.vert - 2; y++)
                {
                    if (tile_str[y * (wh.hori + 1)] != '#' && _len <= 0)
                    {
                        _in_active = (int)tile_str[y * (wh.hori + 1)];
                        _len++;
                    }
                    else if (tile_str[y * (wh.hori + 1)] == _in_active)
                    {
                        _len++;
                    }
                    else
                    {
                        if (_len > 0)
                        {
                            roomtransition *rt = new roomtransition(-1, y - _len - 1, 1, _len, ret->get_index(), m_level_index, 3, -_in_active);
                            ret->add_transition(rt);
                            m_transitions->push_back(rt);
                            _len = 0;
                            _in_active = -3;
                        }

                        if (tile_str[y * (wh.hori + 1)] != '#')
                        {
                            _in_active = tile_str[y * (wh.hori + 1)];
                            _len++;
                        }
                    }
                }

                if (_len > 0)
                {
                    roomtransition *rt = new roomtransition(-1, wh.vert - _len - 3, 1, _len, ret->get_index(), m_level_index, 3, -_in_active);
                    ret->add_transition(rt);
                    m_transitions->push_back(rt);
                    _len = 0;
                    _in_active = -3;
                }

                // Right stuff
                for (int y = 1; y < wh.vert - 2; y++)
                {
                    if (tile_str[y * (wh.hori + 1) + wh.hori - 1] != '#' && _len <= 0)
                    {
                        _in_active = (int)tile_str[y * (wh.hori + 1) + wh.hori - 1];
                        _len++;
                    }
                    else if (tile_str[y * (wh.hori + 1) + wh.hori - 1] == _in_active)
                    {
                        _len++;
                    }
                    else
                    {
                        if (_len > 0)
                        {
                            roomtransition *rt = new roomtransition(wh.hori - 2, y - _len - 1, 1, _len, ret->get_index(), m_level_index, 2, -_in_active);
                            ret->add_transition(rt);
                            m_transitions->push_back(rt);
                            _len = 0;
                            _in_active = -3;
                        }

                        if (tile_str[y * (wh.hori + 1) + wh.hori - 1] != '#')
                        {
                            _in_active = tile_str[y * (wh.hori + 1) + wh.hori - 1];
                            _len++;
                        }
                    }
                }

                if (_len > 0)
                {
                    roomtransition *rt = new roomtransition(wh.hori, wh.vert - _len - 3, 1, _len, ret->get_index(), m_level_index, 2, -_in_active);
                    ret->add_transition(rt);
                    m_transitions->push_back(rt);
                    _len = 0;
                    _in_active = -3;
                }

                pixelstr ps{};
                Pixel::copy_string_to_pixel_string(ps, rt, [&](const size_t i, char &c, Pixel::ColorType &t, Pixel::Color &col, Pixel::Font &f)
                                                   {
                    if(c == '#')
                        col = ((i % (wh.hori+1)) + (i/(wh.hori+1))) % 2 > 0 ? tcol2 : Pixel::DEFAULT;
                    else
                        col = Pixel::WHITE; });
                ret->set_base_dat_str(ps);

                continue;
            }
            tile_str += str_line + '\n';
        }
        else if (status == 2)
        {
            if (std::regex_match(str_line, c_end_rx))
            {
                status = 0;
                pixelstr ps = ret->get_base_dat_str();
                Pixel::for_each(ps, [&](const size_t i, char &c, Pixel::ColorType &t, Pixel::Color &col, Pixel::Font &f)
                                {
                    for(int x = 0; x < tile_str.size(); x += 2)
                    {
                        if(c == tile_str[x])
                            c = tile_str[x+1];

                        // Color
                        if(c == '#')
                            col = ((i % (wh.hori+1)) + (i/(wh.hori+1))) % 2 > 0 ? tcol2 : Pixel::DEFAULT;
                        else
                            col = Pixel::WHITE;
                    } });
                ret->set_base_dat_str(ps);
            }
            tile_str += str_line;
        }
        else if (status == 4)
        {
            if (std::regex_match(str_line, c_end_rx))
            {
                status = 0;
            }
            else if (std::regex_search(str_line, smat, c_transition_gsub_rx))
            {
                int c = smat[1].str()[0];
                int r = std::atoi(smat[2].str().c_str());

                if(c == '+' || c == '-' || r == '+' || r == '-')
                    throw std::runtime_error{"You can't override or gsub reserved transition IDs!"};

                ret->for_each_transition([&c, &r](roomtransition &rt)
                                         {
                    if(rt.get_meta_index() == -c)
                        rt.set_meta_index(r); });
            }
        }
        else if (status == 3)
        {
            bool area_flag = false;
            int a = 0, b = 0, c = 0, d = 0;

            if (std::regex_match(str_line, c_end_rx))
            {
                status = 0;
                continue;
            }

            if (std::regex_search(str_line, smat, c_cl_area_rx))
            {
                area_flag = true;
                a = std::atoi(smat[1].str().c_str());
                b = std::atoi(smat[2].str().c_str());
                c = std::atoi(smat[3].str().c_str());
                d = std::atoi(smat[4].str().c_str());
            }

            if (std::regex_search(str_line, smat, c_cl_start_rx))
            {
                rplayerobj *ro = new rplayerobj(env_ptr, std::atoi(smat[1].str().c_str()), std::atoi(smat[2].str().c_str()), smat[3].str()[0]);
                ret->add_obj(ro);
                ret->set_checkpoint(v2{std::atoi(smat[2].str().c_str()), std::atoi(smat[1].str().c_str())});
                ro->set_checkpoint(ret->get_checkpoint());
            }

            else if (std::regex_search(str_line, smat, c_cl_speed_rx))
            {
                rspeedbump::direction_e _d;
                switch (smat[1].str()[0])
                {
                case 'l':
                    _d = rspeedbump::LEFT;
                    break;
                case 'r':
                    _d = rspeedbump::RIGHT;
                    break;
                case 'd':
                    _d = rspeedbump::DOWN;
                    break;
                case 'u':
                default:
                    _d = rspeedbump::UP;
                    break;
                }
                if (area_flag)
                {
                    for (int x = a; x <= c; x++)
                        for (int y = b; y <= d; y++)
                        {
                            ret->add_obj(new rspeedbump(_d, x, y));
                        }
                }
                else
                    ret->add_obj(new rspeedbump(_d, std::atoi(smat[2].str().c_str()), std::atoi(smat[3].str().c_str())));
            }

            else if (std::regex_search(str_line, smat, c_cl_button_rx))
            {
                int del = std::atoi(smat[3].str().c_str());
                int prID = std::atoi(smat[5].str().c_str()) + (smat[4].str()[0] == 'l' ? 65 : 0);
                int reID = std::atoi(smat[7].str().c_str()) + (smat[6].str()[0] == 'l' ? 65 : 0);

                if (area_flag)
                {
                    for (int x = a; x <= c; x++)
                        for (int y = b; y <= d; y++)
                        {

                            ret->add_obj(new rbuttonobj(x, y, del, prID, reID));
                        }
                }
                else
                    ret->add_obj(new rbuttonobj(std::atoi(smat[1].str().c_str()), std::atoi(smat[2].str().c_str()), del, prID, reID));
            }

            else if (std::regex_search(str_line, smat, c_cl_toggwall_rx))
            {
                int prID = std::atoi(smat[5].str().c_str()) + (smat[4].str()[0] == 'l' ? 65 : 0);
                int reID = std::atoi(smat[7].str().c_str()) + (smat[6].str()[0] == 'l' ? 65 : 0);
                bool ish = smat[1].str()[0] == 'h';

                if (area_flag)
                {
                    for (int x = a; x <= c; x++)
                        for (int y = b; y <= d; y++)
                        {

                            ret->add_obj(new rdisappearingwall(x, y, robj::F2, ish ? ret->get_foreground_color() : Pixel::DEFAULT, prID, reID));
                        }
                }
                else
                    ret->add_obj(new rdisappearingwall(std::atoi(smat[2].str().c_str()), std::atoi(smat[3].str().c_str()), robj::F2, ish ? ret->get_foreground_color() : Pixel::DEFAULT, prID, reID));
            }

            else if (std::regex_search(str_line, smat, c_cl_spinner_rx))
            {
                double period = std::atof(smat[4].str().c_str());
                double offset = std::atof(smat[5].str().c_str());
                int radius = std::atoi(smat[6].str().c_str());
                char ch = smat[3].str()[0];

                if (area_flag)
                {
                    for (int x = a; x <= c; x++)
                        for (int y = b; y <= d; y++)
                        {
                            ret->add_obj(new rspinnyobj(x, y, ch, period, offset, radius));
                        }
                }
                else
                    ret->add_obj(new rspinnyobj(std::atoi(smat[1].str().c_str()), std::atoi(smat[2].str().c_str()), ch, period, offset, radius));
            }

            else if (std::regex_search(str_line, smat, c_cl_teleporter_rx))
            {
                int xs = std::atoi(smat[1].str().c_str());
                int ys = std::atoi(smat[2].str().c_str());
                int xd = std::atoi(smat[3].str().c_str());
                int yd = std::atoi(smat[4].str().c_str());
                int ow = smat[5].str()[0] == 'y' ? 1 : 0;

                ret->add_obj(new rteleportobj(xs, ys, xd, yd, ow));
            }

            else if(std::regex_search(str_line, smat, c_cl_roller_rx))
            {
                //rx = " *(.)roller ([0-9x]+) ([0-9y]+) (.) ([0-9.]+)"
                double period = std::atof(smat[5].str().c_str());
                char ch = smat[4].str()[0];

                robj::direction_e _d;
                switch (smat[1].str()[0])
                {
                case 'l':
                    _d = robj::LEFT;
                    break;
                case 'r':
                    _d = robj::RIGHT;
                    break;
                case 'd':
                    _d = robj::DOWN;
                    break;
                case 'u':
                default:
                    _d = robj::UP;
                    break;
                }

                if (area_flag)
                {
                    for (int x = a; x <= c; x++)
                        for (int y = b; y <= d; y++)
                        {
                            ret->add_obj(new rrollerobj(env_ptr, x, y, ch, _d, period));
                        }
                }
                else
                    ret->add_obj(new rrollerobj(env_ptr,
                        std::atoi(smat[2].str().c_str()),
                        std::atoi(smat[3].str().c_str()),
                        ch, _d, period
                    ));
            }

            else if(std::regex_search(str_line, smat, c_cl_rollerspawner_rx))
            {
                //rx = " *(.)rollerspawner ([0-9x]+) ([0-9y]+) (.) (.) ([0-9]+) ([0-9.]+) *"
                robj::direction_e _d;
                switch (smat[1].str()[0])
                {
                case 'l':
                    _d = robj::LEFT;
                    break;
                case 'r':
                    _d = robj::RIGHT;
                    break;
                case 'd':
                    _d = robj::DOWN;
                    break;
                case 'u':
                default:
                    _d = robj::UP;
                    break;
                }

                char sp_ch = smat[4].str()[0];
                char ro_ch = smat[5].str()[0];
                int sp_per = std::atoi(smat[6].str().c_str());
                int ro_per = std::atoi(smat[7].str().c_str());

                if (area_flag)
                {
                    for (int x = a; x <= c; x++)
                        for (int y = b; y <= d; y++)
                        {
                            ret->add_obj(new rrollerspawnerobj(env_ptr, x, y, sp_ch, ro_ch, _d, sp_per, ro_per));
                        }
                }
                else
                    ret->add_obj(new rrollerspawnerobj(env_ptr,
                        std::atoi(smat[2].str().c_str()),
                        std::atoi(smat[3].str().c_str()),
                        sp_ch, ro_ch, _d, sp_per, ro_per
                    ));
            }
        }
    }
    ret->direct_commit();
    return ret;
}
