#pragma once

#include "Room.hpp"
#include "Utils.hpp"

class level
{
private:
    /** ROOM OBJECT STORAGE **/
    std::vector<room*> m_room_storage;
    room *m_active_room = nullptr;
    int index;
    unsigned long long ltriggers = 0ULL;

public:
    level(const int index): index(index) {}
    ~level()
    {
        for(std::vector<room*>::iterator i = m_room_storage.begin(); i != m_room_storage.end(); i++)
        {
            delete *i;
        }
        m_room_storage.clear();
    }

    inline void subscribe_new_room(room *_room) {this->m_room_storage.push_back(_room);}
    inline room *get_active_room() {return this->m_active_room;}
    inline void set_active_room(const std::size_t index) {this->m_active_room = m_room_storage[index];}
    inline int get_index() const {return this->index;}
    inline void register_event_current(const unsigned long long tick, const std::function<void()> fct) {this->m_active_room->register_event(tick, fct);}
    inline int size() const {return this->m_room_storage.size();}
    inline void propagate_ltriggers()
    {
        for(auto i = m_room_storage.begin(); i != m_room_storage.end(); i++)
        {
            if(*i != m_active_room)
                (*i)->physics(0);
        }

        Utils::do_event(1, [&]{
            ltriggers = 0ULL;
        });
    }
    inline void do_ltri(const __UINT8_TYPE__ _ID)
    {
        __UINT8_TYPE__ ID = _ID - 65;
        ltriggers |= (1 << ID);
    }

    inline bool is_ltrig(const __UINT8_TYPE__ _ID)
    {
        __UINT8_TYPE__ ID = _ID - 65;
        return (ltriggers & (1 << ID)) > 0;
    }
};
