#include<stdio.h>
#include"pui.h"
#include<stdlib.h>
#include<curses.h>
#include<unistd.h>
#include<pthread.h>
pui::Screen scr;
pui::Window w1 = pui::Window(10,10,10,10),
        w2 = pui::Window(10,10,10,20);

int main() {
    scr.Refresh();
    pthread_t t1,t2;
    pthread_create(&t1, NULL, th1, NULL);
    pthread_create(&t2, NULL, th2, NULL);

    sleep(30);

}