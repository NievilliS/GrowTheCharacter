#pragma once

#include <DUNE/DControlManager.hpp>
#include "objects/Room.hpp"
#include "objects/RObj.hpp"
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
    std::vector<room*> m_room_storage;
    room *m_active_room = nullptr;

    /** MISCELLANEOUS **/
    std::thread m_controls_thread;
    std::atomic<int> m_keymap = 0;
    unsigned long long m_tick = 0ULL;
    bool m_running = true;

public:
    gameenv();
    ~gameenv();

    /** KEYPRESS MACRO DECLARATIONS **/
    _keypress_fcts_make(up, KEY_UP_BIT);
    _keypress_fcts_make(down, KEY_DOWN_BIT);
    _keypress_fcts_make(left, KEY_LEFT_BIT);
    _keypress_fcts_make(right, KEY_RIGHT_BIT);

    /** PRE GAME **/
    inline void subscribe_new_room(room *&&_room) {this->m_room_storage.push_back(_room);}
    inline room *get_active_room() {return this->m_active_room;}
    inline void set_active_room(const std::size_t index) {this->m_active_room = m_room_storage[index];}
    inline unsigned long long get_tick() {return this->m_tick;}
    inline unsigned long long iterate_tick() {return this->m_tick++;}
    void stop();
    void register_event_current(const unsigned long long delay, const std::function<void()> fct);
    void launch();
    void debug_join() {this->m_controls_thread.join();}

};
