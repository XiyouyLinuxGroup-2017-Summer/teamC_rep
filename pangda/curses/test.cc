#include<stdio.h>
#include"pui.h"
#include<stdlib.h>
#include<curses.h>
#include<unistd.h>

int main(){

 WINDOW *new_window_ptr;
 WINDOW *popup_window_ptr;
 int x_loop;
 int y_loop;
 char a_letter = '@';

 initscr();
 cbreak;
 noecho();
 move(5,5);
 printw("%s","Testing multiple windows\n");
 printw("LINES =%d",LINES);
 printw("COLS=%d",COLS);
 refresh();
 getch();

 for(y_loop =0;y_loop<LINES-1;y_loop++){
   for(x_loop=0;x_loop<COLS-1;x_loop++){
         // mvwaddch(stdscr,y_loop,x_loop,a_letter);
         addch(a_letter);
        }
}
 refresh();
 getch();

 new_window_ptr = newwin(10,20,5,5);
 mvwprintw(new_window_ptr,2,2,"%s","hello world");
 mvwprintw(new_window_ptr,5,2,"%s","Notice how very long line wrap inside in window");
 wrefresh(new_window_ptr);
 getch();

a_letter ='@';
 for(y_loop =0;y_loop<LINES-1;y_loop++){
   for(x_loop=0;x_loop<COLS-1;x_loop++){
         mvwaddch(stdscr,y_loop,x_loop,a_letter);
        }
}
  refresh();
  getch();

  touchwin(new_window_ptr);
  //touchwin告诉new_window_ptr指向的窗口已经发生变化，如果不不使用这个，下面语句不能使他重新绘制 
  wrefresh(new_window_ptr);
  getch();

popup_window_ptr = newwin(10,20,8,8);
  box(popup_window_ptr,'|','-');
  mvwprintw(popup_window_ptr,5,2,"%s","pop up window");
  wrefresh(popup_window_ptr);
  wrefresh(new_window_ptr);
  getch();

  touchwin(new_window_ptr);
  wrefresh(new_window_ptr);
 getch();
  wclear(new_window_ptr);
  wrefresh(new_window_ptr);
  getch();

  delwin(new_window_ptr);
  touchwin(popup_window_ptr);
  wrefresh(popup_window_ptr);
  getch();

delwin(popup_window_ptr);
  touchwin(stdscr);
  refresh();
  getch();


 endwin();
 exit(EXIT_SUCCESS);
}