#pragma once

#include <DUNE/DControlManager.hpp>
#include "Room.hpp"
#include "RObj.hpp"
#include "GameManagers.hpp"

/** MACRO TOOLS **/

#define KEY_UP_BIT                  (0)
#define KEY_DOWN_BIT                (1)
#define KEY_LEFT_BIT                (2)
#define KEY_RIGHT_BIT               (3)

#define _keypress_press(bit)     (this->m_keymap |= (1 << (bit)))
#define _keypress_pressed(bit)   ((this->m_keymap & (1 << (bit))) > ((this->m_keymap &= ~(1 << (bit))) & 0))

#define _keypress_fcts_make(name, bit)                                                  \
    inline void name ## _kbpress()   {_keypress_press(bit);}                            \
    inline bool name ## _kbpressed() {return _keypress_pressed(bit);}

/**
 * @brief Open environment for the entire game, stored values are meant to be very open and accessible
 * 
 */
class gameenv
{
private:
    /** MANAGERS **/
    DManager::DControlManager m_dcm;
    graphicsmgr m_graphicsmgr;
    physicsmgr m_physicsmgr;
    
    /** ROOM OBJECT STORAGE **/
    std::vector<std::unique_ptr<room>> m_room_storage;
    room *m_active_room;

    /** MISCELLANEOUS **/
    std::thread m_controls_thread;
    std::atomic<int> m_keymap;
    unsigned long long m_tick;

public:
    gameenv();

    /** KEYPRESS MACRO DECLARATIONS **/
    _keypress_fcts_make(up, KEY_UP_BIT);
    _keypress_fcts_make(down, KEY_DOWN_BIT);
    _keypress_fcts_make(left, KEY_LEFT_BIT);
    _keypress_fcts_make(right, KEY_RIGHT_BIT);

    /** PRE GAME **/
    inline void subscribe_new_room(room *&&_room) {this->m_room_storage.push_back(std::unique_ptr<room>(_room));}
    inline room *get_active_room() {return this->m_active_room;}
    inline void set_active_room(const std::size_t index) {this->m_active_room = m_room_storage[index].get();}
    inline unsigned long long get_tick() {return this->m_tick;}
    inline unsigned long long iterate_tick() {return this->m_tick++;}
    void launch();

};
