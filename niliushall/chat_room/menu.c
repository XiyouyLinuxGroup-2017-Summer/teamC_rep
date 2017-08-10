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

                if(recv_buf[0] == 'y'){  //登录成功
                    login(conn_fd);
                    menu_chat(conn_fd);  //进入聊天菜单
                }
                else
                    printf("connect with server error\n\n");

                break;
            }

            case 2: {   //register
                if(send(conn_fd, "2", 2, 0) < 0)
                    err("send", __LINE__);

                if(recv(conn_fd, recv_buf, sizeof(recv_buf), 0) < 0)
                    err("recv", __LINE__);

                if(recv_buf[0] == 'y')  //已连接服务器
                    my_register(conn_fd);  // 开始注册
                else
                    printf(RED"connect to server error\n\n"END);
                break;
            }

            case 0:    //退出并修改在线状态
                if(send(conn_fd, "0", 2, 0) < 0)
                    err("send", __LINE__);
                break;

            default: {
                printf(RED"Input error. The number should be 0 ~ 2\n"END);
                printf(RED"Press any key to continue...\n\n"END);
                getchar();
                break;
            }
        }

    } while(choice);
}


void menu_chat(int conn_fd) {
    int choice;
    char recv_buf[ BUFSIZE ];
    struct message info;

    info.from = conn_fd; 

    do {
        CLEAR;
        printf("------------------------------------------------\n");
        printf("---          Welcome to my chatroom          ---\n");
        printf("------------------------------------------------\n");
        printf("---                                          ---\n");
        printf("---         1. Private chat                  ---\n");  //私聊
        printf("---         2. Group chat                    ---\n");  //群聊
        printf("---         3. Chat log                      ---\n");  //聊天记录
        printf("---         4. Friend's state                ---\n");  //在线状态
        printf("---         5. Add friend                    ---\n");  //添加好友
        printf("---         6. Delete friend                 ---\n");  //删除好友
        printf("---         7. Display friends               ---\n");  //好友列表
        printf("---         8. Quit group                    ---\n");  //退群
        printf("---         9. Add group                     ---\n");  //加群
        printf("---        10. Create group                  ---\n");  //建群
        printf("---        11. Delete group                  ---\n");  //解散群
        printf("---        12. Invite friend into group      ---\n");  //邀请好友加群
        printf("---         0. Exit                          ---\n");
        printf("---                                          ---\n");
        printf("------------------------------------------------\n\n");
        printf("Please input a number(0 ~ 12):\n");

        scanf("%d", &choice);
        getchar();
        fflush(stdin);

        switch(choice) {
            
            case 1: {  //私聊
                printf(GREEN"-1 to display friends state, or input friend's account:\n"END);
                scanf("%d", &info.from);

                if(info.from == -1) {
                    break;
                }

                if(send(conn_fd, &info, sizeof(info), 0) < 0)
                    err("send", __LINE__);

                if(recv(conn_fd, recv_buf, sizeof(recv_buf), 0) < 0)
                    err("recv", __LINE__);

                if(recv_buf[0] != 'y') {
                    printf(RED"connect to server error\n"END);
                    break;
                }

                while(1) {
                    printf(BLUE"me: %s\n"END, my_time());
                    fgets(info.buf, sizeof(info.buf), stdin);

                    if(!strcmp(info.buf, "exit")) {  //输入exit退出聊天
                        printf("GREEN ");
                    }


                    if(send(conn_fd, (char *)&info, sizeof(info), 0) < 0)
                        err("send", __LINE__);

                    if(recv(conn_fd, recv_buf, sizeof(recv_buf), 0) < 0)
                        err("recv", __LINE__);

                    if(recv_buf[0] != 'y') {
                        printf(RED"send message failed.\n"END);
                    }
                }
            }
        }
    } while(choice);
}
