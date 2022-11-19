#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>
#include "cutils.hpp"

void setBufferedInput(bool enable)
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

int getChar()
{
    return getchar();
}

void simKeyPress()
{
	Display *display;
	unsigned int keycode;
	display = XOpenDisplay(NULL);
	keycode = XKeysymToKeycode(display, XK_H);
	XTestFakeKeyEvent(display, keycode, True, 0);
	XTestFakeKeyEvent(display, keycode, False, 0);
	XFlush(display);
}
