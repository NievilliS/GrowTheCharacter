#pragma once

#include <regex>
#include "objects/Room.hpp"

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

    const std::regex check_rx{"^room.: *"};
    const std::regex index_rx{"^room(.): *"};
    const std::regex line_rx{"[^\n]+"};
    const std::regex tiles_rx{"^tiles: *"};
    const std::regex name_rx{"^name: ?([^ ]+) *"};
    const std::regex comment_rx{"^#.*"};
    const std::regex end_rx{"^end *"};
    const std::regex bcol_rx{"^bgrcol: ?([^ ]+) *"};
    const std::regex tcol_rx{"^txtcol: ?([^ ]+) *"};
    const std::regex tile_trim_rx{"^.(.+).$"};
    const std::regex tile_getl_rx{"^(.).+$"};
    const std::regex tile_getr_rx{"^.+(.)$"};

public:
    roommaker(const std::string _raw_room_string): raw_room_str(_raw_room_string) {}

    room *createroom()
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
                    continue;
                }
                else if(std::regex_search(str_line, smat, bcol_rx))
                {
                    ret->set_background_color(get_color(smat[1].str()));
                }
                else if(std::regex_search(str_line, smat, tcol_rx))
                {
                    ret->set_foreground_color(get_color(smat[1].str()));
                }
                else if(std::regex_match(str_line, tiles_rx)) {
                    status = 1;
                    continue;
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
                        wh = room::get_pixelstr_dim(ps) - v2{2,2};
                    }
                    std::string rt{};
                    auto ITER = std::sregex_iterator(str_line.begin(), str_line.end(), line_rx);
                    ITER++;

                    pixelstr ps{};
                    Pixel::copy_string_to_pixel_string(ps, rt);
                    std::cout << rt << std::endl;
                    ret->set_base_dat_str(ps);
                    continue;
                }
                tile_str += str_line + '\n';
            }
        }
        return ret;
    }    
};
