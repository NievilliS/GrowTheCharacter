#pragma once

#include <consoleengine/ConsoleEngine.hpp>
#include "RObj.hpp"
#include "Player.hpp"
#include "EventQueue.hpp"
#include "RoomTransition.hpp"
#include <atomic>
#include <mutex>

class checkpoint
{
public:
    void *m_room_ptr;
    v2 m_room_pos;
    char m_player_char = 0;

    checkpoint(void *_room_ptr, const v2 _room_pos) : m_room_pos(_room_pos),
                                                      m_room_ptr(_room_ptr)
    {
    }
};

class room
{
private:
    pixelstr m_base_dat{};
    Pixel::Color m_background_color;
    Pixel::Color m_foreground_color;
    v2 m_size;
    std::vector<robj *> m_object_storage;
    std::vector<robj *> m_new_objects;
    std::vector<revent *> m_event_queue;
    std::vector<roomtransition *> m_transition_storage;
    std::string m_room_name;
    int m_index;
    unsigned long long m_trigger_map = 0ULL;
    unsigned long long m_next_trigger_map = 0ULL;
    bool m_cancel_phys = false;
    bool m_debug_information = false;
    checkpoint *m_checkpoint = nullptr;
    std::mutex m_objmutex;

public:
    room(const int _index);
    ~room();

    void set_base_dat_str(pixelstr &bdat);
    void add_obj(robj *obj);
    void direct_commit() {for(auto i = m_new_objects.begin(); i != m_new_objects.end(); i++) m_object_storage.push_back(*i); m_new_objects.clear();};
    room &operator+(robj *obj);
    void draw(unsigned long long tick);
    void physics(unsigned long long tick);
    void set_background_color(const Pixel::Color &bgr);
    Pixel::Color &get_background_color() {return m_background_color;}
    void set_foreground_color(const Pixel::Color &fgr);
    Pixel::Color &get_foreground_color() {return m_foreground_color;}
    int collision_with_base(v2 &rpo);
    void register_event(const unsigned long long tick, const std::function<void()> fct);
    void run_trigger(int ID);
    bool is_triggered(int ID);

    template <typename T>
    std::vector<T*> get_list()
    {
        std::vector<T *> vct;
        m_objmutex.lock();
        for (auto i = this->m_object_storage.begin(); i != m_object_storage.end(); i++)
        {
            T *pt = dynamic_cast<T *>(*i);
            if (pt != nullptr)
                vct.push_back(pt);
        }
        m_objmutex.unlock();
        return vct;
    }

    template <typename T>
    void for_each(std::function<void(T &)> fct)
    {
        std::vector<T *> vct = get_list<T>();

        for (auto i = vct.begin(); i != vct.end(); i++)
        {
            fct(**i);
        }
    }

    void for_each_transition(std::function<void(roomtransition &)> fct)
    {
        for (auto i = this->m_transition_storage.begin(); i != this->m_transition_storage.end(); i++)
        {
            fct(**i);
        }
    }

    inline v2 get_size() { return m_size; }
    inline void set_name(const std::string _name) { this->m_room_name = _name; }
    inline std::string get_name() const { return this->m_room_name; }
    inline void set_index(const int _index) { this->m_index = _index; }
    inline int get_index() const { return this->m_index; }

    static v2 get_pixelstr_dim(const pixelstr &bdat);
    inline pixelstr get_base_dat_str() const { return this->m_base_dat; }
    roomtransition *get_if_collide(const v2 &v);
    void remove_obj(const robj *obj, const bool _free = false);
    inline void add_transition(roomtransition *rt) { this->m_transition_storage.push_back(rt); }
    inline void cancel_phys() { this->m_cancel_phys = true; }
    inline void __debug_set_flag() { this->m_debug_information = true; }
    inline void set_checkpoint(v2 v) { this->m_checkpoint = new checkpoint(this, v); }
    inline checkpoint *get_checkpoint() { return this->m_checkpoint; }
    unsigned char get_char_at_base(v2 &v);
    void propagate_trigger_sensitive();
};
