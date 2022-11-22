#pragma once

#include "Room.hpp"

class level
{
private:
    /** ROOM OBJECT STORAGE **/
    std::vector<room*> m_room_storage;
    room *m_active_room = nullptr;
    int index;

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
};
