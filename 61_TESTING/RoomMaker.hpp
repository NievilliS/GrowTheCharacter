#pragma once

#include <regex>
#include "GameEnvironment.hpp"
#include "objects/Player.hpp"
#include "objects/SpeedBump.hpp"
#include "objects/Button.hpp"
#include "objects/DisappearingWall.hpp"

const Pixel::Color &get_color(std::string st)
{
    if(st.compare("BLACK") == 0)
    {
        return Pixel::BLACK;
    }
    else if(st.compare("RED") == 0)
    {
        return Pixel::RED;
    }
    else if(st.compare("GREEN") == 0)
    {
        return Pixel::GREEN;
    }
    else if(st.compare("YELLOW") == 0)
    {
        return Pixel::YELLOW;
    }
    else if(st.compare("BLUE") == 0)
    {
        return Pixel::BLUE;
    }
    else if(st.compare("MAGENTA") == 0)
    {
        return Pixel::MAGENTA;
    }
    else if(st.compare("CYAN") == 0)
    {
        return Pixel::CYAN;
    }
    else if(st.compare("WHITE") == 0)
    {
        return Pixel::WHITE;
    }
    return Pixel::DEFAULT;
}

class roommaker
{
protected:
    std::string raw_room_str;

    const std::regex
        check_rx{"^room.: *"},
        index_rx{"^room(.): *"},
        line_rx{"[^\n]+"},
        tiles_rx{"^tiles: *"},
        name_rx{"^name: ?([^ ]+) *"},
        comment_rx{"^//.*"},
        end_rx{"^end *"},
        bcol_rx{"^bgrcol: ?([^ ]+) *"},
        tcol_rx{"^txtcol: ?([^ ]+) *"},
        tcol2_rx{"^txtcol: ?([^ ]+) ?% ?([^ ]+) *"},
        gsub_rx{"^gsub: *"},
        class_rx{"^class: *"},

        cl_start_rx{"start ([0-9x]+) ([0-9y]+) (.) *"},
        cl_area_rx{"^area ([0-9]+) ([0-9]+) ([0-9]+) ([0-9]+) "},
        cl_speed_rx{"(.)speed ([0-9x]+) ([0-9y]+) *"},
        cl_button_rx{"button ([0-9x]+) ([0-9y]+) (-?[0-9]+) (l?)([0-9]+) (l?)([0-9]+) *"},
        cl_toggwall_rx{"toggwall ([0-9x]+) ([0-9y]+) (l?)([0-9]+) (l?)([0-9]+) *"};

public:
    roommaker(const std::string _raw_room_string): raw_room_str(_raw_room_string) {}

    room *createroom(void *env_ptr)
    {
        std::smatch smat;

        std::cout << raw_room_str << std::endl;

        if(!std::regex_search(raw_room_str, smat, index_rx))
        {
            throw "Room init misformatting";
        }

        room *ret = new room(smat[1].str()[0]);
        int status = 0;
        auto lines_begin = std::sregex_iterator(raw_room_str.begin(), raw_room_str.end(), line_rx);
        auto lines_end = std::sregex_iterator();
        std::string tile_str{};
        Pixel::Color tcol2 = Pixel::DEFAULT;
        v2 wh{};

        for(auto lines_iter = lines_begin; lines_iter != lines_end; ++lines_iter)
        {
            std::smatch match_lines_iter = *lines_iter;
            auto str_line = match_lines_iter.str();

            if(std::regex_match(str_line, comment_rx))
            {
                continue;
            }

            if(status == 0)
            {
                if(std::regex_search(str_line, smat, name_rx))
                {
                    std::string name = smat[1].str();
                    ret->setname(name);
                }
                else if(std::regex_search(str_line, smat, bcol_rx))
                {
                    ret->set_background_color(get_color(smat[1].str()));
                }
                else if(std::regex_search(str_line, smat, tcol2_rx))
                {
                    ret->set_foreground_color(get_color(smat[1].str()));
                    tcol2 = get_color(smat[2].str());
                }
                else if(std::regex_search(str_line, smat, tcol_rx))
                {
                    ret->set_foreground_color(get_color(smat[1].str()));
                }
                else if(std::regex_match(str_line, tiles_rx))
                {
                    tile_str.clear();
                    status = 1;
                }
                else if(std::regex_match(str_line, gsub_rx))
                {
                    tile_str.clear();
                    status = 2;
                }
                else if(std::regex_match(str_line, class_rx))
                {
                    status = 3;
                }
            }
            else if(status == 1)
            {
                if(std::regex_match(str_line, end_rx))
                {
                    status = 0;
                    {
                        pixelstr ps{};
                        Pixel::copy_string_to_pixel_string(ps, tile_str);
                        wh = room::get_pixelstr_dim(ps);
                    }
                    std::string rt{};

                    for(int y = 1; y < wh.vert - 2; y++)
                    {
                        for(int x = 1; x < wh.hori - 1; x++)
                        {
                            rt += tile_str[x + y * (wh.hori+1)];
                        }
                        if(y < wh.vert - 3) rt += '\n';
                    }

                    pixelstr ps{};
                    Pixel::copy_string_to_pixel_string(ps, rt, [&](const size_t i, char &c, Pixel::ColorType &t, Pixel::Color &col, Pixel::Font &f)
                    {
                        if(c == '#')
                            col = ((i % (wh.hori+1)) + (i/(wh.hori+1))) % 2 > 0 ? tcol2 : Pixel::DEFAULT;
                        else
                            col = Pixel::WHITE;
                    });
                    ret->set_base_dat_str(ps);
                    
                    continue;
                }
                tile_str += str_line + '\n';
            }
            else if(status == 2)
            {
                if(std::regex_match(str_line, end_rx))
                {
                    status = 0;
                    pixelstr ps = ret->get_base_dat_str();
                    Pixel::for_each(ps, [&](const size_t i, char &c, Pixel::ColorType &t, Pixel::Color &col, Pixel::Font &f){
                        for(int x = 0; x < tile_str.size(); x += 2)
                        {
                            if(c == tile_str[x])
                                c = tile_str[x+1];
                        }
                    });
                    ret->set_base_dat_str(ps);
                    ret->draw(0);
                }
                tile_str += str_line;
            }
            else if(status == 3)
            {
                bool area_flag = false;
                int a = 0,b = 0,c = 0,d = 0;

                if(std::regex_match(str_line, end_rx))
                {
                    status = 0;
                    continue;
                }

                if(std::regex_search(str_line, smat, cl_area_rx))
                {
                    area_flag = true;
                    a = std::atoi(smat[1].str().c_str());
                    b = std::atoi(smat[2].str().c_str());
                    c = std::atoi(smat[3].str().c_str());
                    d = std::atoi(smat[4].str().c_str());
                }






                if(std::regex_search(str_line, smat, cl_start_rx))
                {
                    if(area_flag)
                    {
                        for(int x = a; x <= c; x++) for(int y = b; y <= d; y++)
                        {
                            ret->add_obj(new rplayerobj(env_ptr, x, y, smat[3].str()[0]));
                        }
                    }
                    else
                        ret->add_obj(new rplayerobj(env_ptr, std::atoi(smat[1].str().c_str()), std::atoi(smat[2].str().c_str()), smat[3].str()[0]));
                }





                else if(std::regex_search(str_line, smat, cl_speed_rx))
                {
                    rspeedbump::direction _d;
                    switch(smat[1].str()[0])
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
                    if(area_flag)
                    {
                        for(int x = a; x <= c; x++) for(int y = b; y <= d; y++)
                        {
                            ret->add_obj(new rspeedbump(_d, x, y));
                        }
                    }
                    else
                        ret->add_obj(new rspeedbump(_d, std::atoi(smat[2].str().c_str()), std::atoi(smat[3].str().c_str())));
                }



                else if(std::regex_search(str_line, smat, cl_button_rx))
                {
                    int del = std::atoi(smat[3].str().c_str());
                    int prID = std::atoi(smat[5].str().c_str()) + (smat[4].str()[0] == 'l' ? 65 : 0);
                    int reID = std::atoi(smat[7].str().c_str()) + (smat[6].str()[0] == 'l' ? 65 : 0);

                    if(area_flag)
                    {
                        for(int x = a; x <= c; x++) for(int y = b; y <= d; y++)
                        {
                            
                            ret->add_obj(new rbuttonobj(x, y, del, prID, reID));
                        }
                    }
                    else
                        ret->add_obj(new rbuttonobj(std::atoi(smat[1].str().c_str()), std::atoi(smat[2].str().c_str()), del, prID, reID));
                }




                else if(std::regex_search(str_line, smat, cl_toggwall_rx))
                {
                    int prID = std::atoi(smat[4].str().c_str()) + (smat[3].str()[0] == 'l' ? 65 : 0);
                    int reID = std::atoi(smat[6].str().c_str()) + (smat[5].str()[0] == 'l' ? 65 : 0);

                    if(area_flag)
                    {
                        for(int x = a; x <= c; x++) for(int y = b; y <= d; y++)
                        {
                            
                            ret->add_obj(new rdisappearingwall(x, y, robj::F2, prID, reID));
                        }
                    }
                    else
                        ret->add_obj(new rdisappearingwall(std::atoi(smat[1].str().c_str()), std::atoi(smat[2].str().c_str()), robj::F2, prID, reID));
                }


            }
        }
        return ret;
    }    
};
