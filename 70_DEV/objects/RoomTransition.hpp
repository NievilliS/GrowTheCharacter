#pragma once

#include "../vec2.hpp"

class roomtransition
{
protected:
    v2  m_coords,
        m_size;
    int m_room_index;
    int m_level_index;
    roomtransition *m_link = nullptr;
    int m_meta_index;
    int m_direction;
    
public:
    roomtransition(v2 _coords, v2 _size, int _room_index, int _level_index, int _direction, int _meta_index): m_coords(_coords), m_size(_size), m_room_index(_room_index), m_level_index(_level_index), m_direction(_direction), m_meta_index(_meta_index)
    {}

    roomtransition(int x, int y, int w, int h, int _room_index, int _level_index, int _direction, int _meta_index): m_coords{y,x}, m_size{h,w}, m_room_index(_room_index), m_level_index(_level_index), m_direction(_direction), m_meta_index(_meta_index)
    {}

    inline bool collides(const v2 &v) const
    {
        return (v >= m_coords && v < (m_coords + m_size));
    }

    inline bool do_next_level() const
    {
        return m_meta_index == '+';
    }

    inline bool do_prev_level() const
    {
        return m_meta_index == '-';
    }

    inline int get_room_index() const
    {
        return m_room_index;
    }

    inline int get_level_index() const
    {
        return m_level_index;
    }

    inline int get_meta_index() const
    {
        return m_meta_index;
    }

    inline void set_link(roomtransition *rt)
    {
        m_link = rt;
    }

    inline bool is_linked()
    {
        return m_link != nullptr;
    }

    inline v2 get_coord_of_player()
    {
        v2 ret = m_coords + m_size / 2;

        switch(m_direction)
        {
            case 0:
                ret -= v2{1,0};
            break;
            case 1:
                ret += v2{1,0};
            break;
            case 2:
                ret -= v2{0,1};
            break;
            case 3:
                ret += v2{0,1};
        }
        return ret;
    }

    inline roomtransition *get_link()
    {
        return m_link;
    }

    inline void set_meta_index(int _id)
    {
        this->m_meta_index = _id;
    }
};
