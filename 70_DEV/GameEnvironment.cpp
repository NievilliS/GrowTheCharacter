#include "GameEnvironment.hpp"
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>
#include "objects/Utils.hpp"

inline void simKeyPress()
{
    Display *display;
    unsigned int keycode;
    display = XOpenDisplay(NULL);
    keycode = XKeysymToKeycode(display, XK_H);
    XTestFakeKeyEvent(display, keycode, True, 0);
    XTestFakeKeyEvent(display, keycode, False, 0);
    XFlush(display);
}

inline void setBufferedInput(bool enable)
{
    static bool enabled = true;
    static struct termios old;
    struct termios newt;

    if (enable && !enabled)
    {
        // restore the former settings
        tcsetattr(STDIN_FILENO, TCSANOW, &old);
        // set the new state
        enabled = true;
    }
    else if (!enable && enabled)
    {
        // get the terminal settings for standard input
        tcgetattr(STDIN_FILENO, &newt);
        // we want to keep the old setting to restore them at the end
        old = newt;
        // disable canonical mode (buffered i/o) and local echo
        newt.c_lflag &= (~ICANON & ~ECHO);
        // set the new settings immediately
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        // set the new state
        enabled = false;
    }
}

gameenv::gameenv() : m_graphicsmgr{this},
                     m_physicsmgr{this},
                     m_active_level(nullptr),
                     m_controls_thread{[&]
                                       {
                                           setBufferedInput(false);
                                           int c = 0;

                                           while (this->m_running && this->m_dcm.is_running())
                                           {
                                               if (c == 'q')
                                               {
                                                   ConsoleEngine_context << ConsoleEngine::CLEAR << ConsoleEngine::ERASE_SCREEN << Pixel::CYAN << "Ending by ESC press\n"
                                                                         << ConsoleEngine::PRINTOUT;
                                                   this->stop();
                                                   getchar();
                                                   break;
                                               }

                                               switch (c)
                                               {
                                               case 119:
                                               case 65:
                                                   this->up_kbpress();
                                                   break;

                                               case 66:
                                               case 115:
                                                   this->down_kbpress();
                                                   break;

                                               case 67:
                                               case 100:
                                                   this->right_kbpress();
                                                   break;

                                               case 68:
                                               case 97:
                                                   this->left_kbpress();
                                                   break;

                                               case 114:
                                                   this->r_kbpress();
                                                   break;

                                                   /*default:
                                                       ConsoleEngine_context << c << ConsoleEngine::PRINTOUT;
                                                   break;*/
                                               }

                                               c = getchar();
                                           }

                                           setBufferedInput(true);
                                       }}
{
    Utils::init(this);
    m_dcm.subscribe(&m_graphicsmgr);
    m_dcm.subscribe(&m_physicsmgr);
    m_dcm.subscribe_post([&]
                         {simKeyPress(); this->m_controls_thread.join(); });
}

void gameenv::launch()
{
    this->m_dcm.launch();
}

gameenv::~gameenv()
{
    for (std::vector<level *>::iterator i = m_level_storage.begin(); i != m_level_storage.end(); i++)
    {
        delete *i;
    }
    m_level_storage.clear();
}

void gameenv::register_event_current(const unsigned long long delay, const std::function<void()> fct)
{
    this->m_active_level->register_event_current(this->m_tick + delay, fct);
}

void gameenv::stop()
{
    this->m_running = false;
    this->m_dcm.stop();
}

bool gameenv::set_active_index_level(const int index)
{
    for (auto i = this->m_level_storage.begin(); i != this->m_level_storage.end(); i++)
    {
        if ((*i)->get_index() == index)
        {
            this->m_active_level = *i;
            return true;
        }
    }
    return false;
}

void gameenv::set_pcoord(v2 *pcoord)
{
    this->pcoord = pcoord;
}

v2 *gameenv::get_pcoord() const
{
    return this->pcoord;
}

void gameenv::__debug_set_flag()
{
    this->m_debug_information = true;
    for (auto i = this->m_level_storage.begin(); i != this->m_level_storage.end(); i++)
    {
        (*i)->__debug_set_flag();
    }
}

int gameenv::get_damage_index(const unsigned char c)
{
    static int size_index = this->m_damage_index.size();
    if(c == 0) return size_index;

    for(int i = 0; i < size_index; i++)
    {
        if(c == this->m_damage_index[i])
            return i;
    }
    return -1;
}
