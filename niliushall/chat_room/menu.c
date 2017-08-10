#include "myhead.h"

void menu_login(int conn_fd) {
    int choice;
    char recv_buf[ BUFSIZE];

    do{
        CLEAR; //clear the screen

        printf("----------------------------------------\n");
        printf("---      Welcome to my chatroom      ---\n");
        printf("----------------------------------------\n");
        printf("---                                  ---\n");
        printf("---         1. login                 ---\n");
        printf("---         2. register              ---\n");
        printf("---         0. exit                  ---\n");
        printf("---                                  ---\n");
        printf("----------------------------------------\n\n");
        printf("Please input a number(0~2):\n");
        
        scanf("%d", &choice);
        getchar();
        fflush(stdin);

        switch(choice) {
            case 1: {   //login
                if(send(conn_fd, "1", 2, 0) < 0)
                    err("send", __LINE__);

                if(recv(conn_fd, recv_buf, sizeof(recv_buf), 0) < 0)
                    err("recv", __LINE__);

                if(recv_buf[0] == 'y')
                    login(conn_fd);
                else
                    printf("connect with server error\n\n");

                break;
            }

            case 2: {   //register
                if(send(conn_fd, "2", 2, 0) < 0)
                    err("send", __LINE__);

                if(recv(conn_fd, recv_buf, sizeof(recv_buf), 0) < 0)
                    err("recv", __LINE__);

                if(recv_buf[0] == 'y')
                    my_register(conn_fd);
                else
                    printf("connect with server error\n\n");
                break;
            }

            case 0:    //退出并修改在线状态
                if(send(conn_fd, "0", 2, 0) < 0)
                    err("send", __LINE__);
                break;

            default: {
                printf("Input error. The number should be 0 ~ 2\n");
                printf("Press any key to continue...\n\n");
                getchar();
                break;
            }
        }

    } while(choice);
}


void menu_chat(void) {

    CLEAR;
    printf("----------------------------------------\n");
    printf("---      Welcome to my chatroom      ---\n");
    printf("----------------------------------------\n");
    printf("---                                  ---\n");
    printf("---         1. Private chat          ---\n");
    printf("---         2. Group chat            ---\n");
    printf("---         3. Chat log              ---\n");
    printf("---         4. Friend's state        ---\n");
    printf("---         5. Add friend            ---\n");
    printf("---         6. Delete friend         ---\n");
    printf("---         7. Group chat            ---\n");
    printf("---         2. Group chat            ---\n");
    printf("---         0. Exit                  ---\n");
    printf("---                                  ---\n");
    printf("----------------------------------------\n\n");
    printf("Please input a number(0~2):\n");

}