#include "GameEnvironment.hpp"
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>

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

gameenv::gameenv():
    m_graphicsmgr{this},
    m_physicsmgr{this},
    m_active_room(nullptr),
    m_controls_thread{ [&] {
        setBufferedInput(false);
        int c = 0;

        while(this->m_dcm.is_running())
        {
            if(c == 'q')
            {
                ConsoleEngine_context << ConsoleEngine::CLEAR << ConsoleEngine::ERASE_SCREEN << "Ending by ESC press" << ConsoleEngine::PRINTOUT;
                this->m_dcm.stop();
                getchar();
                break;
            }

            switch(c)
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
            }

            c = getchar();
        }

        setBufferedInput(true);
    }}
{
    m_dcm.subscribe(&m_graphicsmgr);
    m_dcm.subscribe(&m_physicsmgr);
    m_dcm.subscribe_post([&]{simKeyPress(); this->m_controls_thread.join();});
}

void gameenv::launch()
{
    this->m_dcm.launch();
}
