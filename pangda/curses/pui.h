#include<cstdarg>
#include<curses.h>
#include<unistd.h>

namespace pui {

class Screen {
private:

public:
    Screen() {
        initscr();
    }
    ~Screen() {
        endwin();
    }
    int Refresh() {
        refresh();
    }
    int Move(int y, int x) {
        return move(y, x);
    }
    int Getchar() {
        return getch();
    }
};

class Window {
private:
    WINDOW *self;
public:
    Window(int startx, int starty, int r, int c) {
        self = newwin(startx, starty, r, c);
    }
    ~Window() {
        delwin(self);
    }
    int DrawBorder() {
        wborder(self, '@','@','@','@','@','@','@','@');
    }
    int PutChar(char which);
    int PutCharAtPos(int y, int x, char which);
    int Print(char *format, ...) {
        va_list ap;
        va_start(ap, format);
        int ret = vwprintw(self, format, ap);
        va_end(ap);
        return ret;
    }
    int PrintAt(int y, int x, char *format, ...) {
        wmove(self, y, x);
        va_list ap;
        va_start(ap, format);
        int ret = vwprintw(self, format, ap);
        va_end(ap);
        return ret;
    }
    int MoveTo(int x, int y);
    int Refresh() {
        return wrefresh(self);
    }
    int Getchar() {
        return wgetch(self);
    }
};




};