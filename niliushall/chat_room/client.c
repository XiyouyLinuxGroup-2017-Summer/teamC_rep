#include "myhead.h"

#define INVALID_USERINFO 'n'
#define VALID_USERINFO   'y'


/*函数声明*/
int get_userinfo(char *buf, int len, char *string);
void input_userinfo(int conn_fd, char *string);
// void menu_login(int conn_fd);
void login(int conn_fd);
void my_register(int conn_fd);
void menu_login(int);
void menu_chat(int);
void * recv_thread(void *);

int taccount;
char tname[NAMESIZE];


int main(int argc, char **argv) {
    int conn_fd, serv_port;
    struct sockaddr_in serv_addr;
    char recv_buf[BUFSIZE];
    int i, ret;
    pthread_t tid;

    if(argc != 3)
        err("The number of argc", __LINE__);

    /*初始化服务器端地址结构*/
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);

    /*检查参数*/
    if(!strcmp(argv[1], "-a")) {
        if(inet_aton(argv[2], &serv_addr.sin_addr) < 0) {
            printf("invalid server ip address\n");
            exit(1);
        }
    } else {
        printf("there should be [ -a ] [ server_ip ]\n");
        exit(1);
    }

    /*创建socket*/
    conn_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(conn_fd < 0)
        err("socket", __LINE__);

    /*向服务器发送连接请求*/
    if(connect(conn_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) < 0)
        err("connect", __LINE__);
    
    menu_login(conn_fd);
    pthread_create(&tid, NULL, recv_thread, &conn_fd);

    menu_chat(conn_fd);

    close(conn_fd);

    return 0;
}


/*子线程接收服务器数据*/
void do_recv (struct message info) {
    int n = info.n;
    
    switch(n) {
        case 1: {  //私聊
            printf(BLUE"%s  %s"END, info.name_from, info.time);
            printf(BLUE"%s"END, info.buf);
        }

        /////////////////
    }
}

/*子线程接收服务器数据*/
void *recv_thread(void *arg) {
    struct message info_recv;
    int conn_fd = *(int *)arg;
    int ret;

    while(1) {
        if((ret = recv(conn_fd, &info_recv, sizeof(info_recv), 0)) < 0)
            err("recv", __LINE__);
        if(!ret) {
            printf(RED"server exit."END);
            pthread_exit(NULL);
        }

        do_recv(info_recv);
    }
}

/*登陆函数*/
void login(int conn_fd){
    int i;
    input_userinfo(conn_fd, "account");
    input_userinfo(conn_fd, "password");

    fprintf(stderr, "Login success. Please wait for a while.");
    for(i = 0; i < 3; i++){
        sleep(1);
        fprintf(stderr, ".");
    }
}

/*获取用户输入信息*/
int get_userinfo(char *buf, int len, char *string) {
    int i;

    if(buf == NULL)
        return -1;

    i = 0;
    while((buf[i++] = getchar()) != '\n' && i < len-1)
        ;
    buf[i-1] = 0;

    if(!strcmp(string, "account"))
        taccount = atoi(buf);

    // fflush(stdin);
    
    return 0;
}

/*输入信息，提交到服务器*/
void input_userinfo(int conn_fd, char *string) {
    char recv_buf[ BUFSIZE ];
    char input_buf[ NAMESIZE ] = {0};
    int flag_userinfo = 0;

    do {
        printf("%s : ", string);
        if(get_userinfo(input_buf, NAMESIZE, string) < 0)
            err("error return from get_userinfo", __LINE__);
        if(send(conn_fd, input_buf, sizeof(input_buf), 0) < 0)
            err("send", __LINE__);

        /*读取套接字数据*/
        if(recv(conn_fd, recv_buf, sizeof(recv_buf), 0) < 0)
            err("recv", __LINE__);

        if(recv_buf[0] == VALID_USERINFO) {
            flag_userinfo = VALID_USERINFO;
        } else {

            printf("%s error, input again\n\n", string);
            flag_userinfo = INVALID_USERINFO;
        }
    } while(flag_userinfo == INVALID_USERINFO);
}


/*注册函数*/
void my_register( int conn_fd ) {
    int i;
    char recv_buf[ BUFSIZE ];
    int account = 0, flag = 0, flag_a;
    // char tpasswd[ PASSWDSIZE ];
    char *tpasswd, *passwd;
    struct userinfo info;

    memset(&info, 0, sizeof(info));

    /*帐号*/
    while(1) {
        flag_a = 0;

        printf("Please input your account number:\n(no mroe than 6 digits , input 0 to exit)\n");

        scanf("%s", recv_buf);
        if(recv_buf[0] == '0') {
            printf("The first number should not be 0\n\n");
            continue;
        }

        i = 0;
        while(recv_buf[i++]) {
            if(!(recv_buf[i-1] >= '0' && recv_buf[i-1] <= '9')) {
                printf("The account format is incorrect\n\n");
                flag_a = 1;
                break;
            }
        }

        if(!flag_a) {
            recv_buf[i-1] = 0;
            info.account = atoi(recv_buf);
        } else {
            continue;
        }

        fflush(stdin);

        if(!info.account)
            return;

        if(send(conn_fd, &info.account, sizeof(int), 0) < 0)
            err("send", __LINE__);

        if(recv(conn_fd, recv_buf, sizeof(recv_buf), 0) < 0)
            err("recv", __LINE__);

        if(recv_buf[0] == 'y')
            break;
        else
            printf("This account has existed, input again:\n");
    }

    /*用户名*/
    printf("Please input your nick name (no ' '):\n");
    scanf("%s", info.name);


    /*密码*/
    flag = 0;
    while(1) {
        
        if(!flag){
            passwd = getpass("Please input password:(-1 to exit)\n");
            flag = 1;
        } else {
            passwd = getpass("The password is inconsistent, input again:\n");
        }

        if(!strcmp(passwd, "-1"))
            return ;

        tpasswd = getpass("Please input password again:\n");

        if(!strcmp(passwd, tpasswd)){
            memcpy(info.passwd, passwd, sizeof(passwd));
            if(send(conn_fd, (char *)&info, sizeof(info), 0) < 0)
                err("send", __LINE__);
        }

        if(recv(conn_fd, recv_buf, sizeof(recv_buf), 0) < 0)
            err("recv", __LINE__);
        if(recv_buf[0] == 'y'){
            printf("Congratulations! Register success!\n");
            sleep(1);
            fflush(stdin);
            break;
        }
    }
}


void menu_login(int conn_fd) {

    int choice;
    char recv_buf[ BUFSIZE];

    do{
        CLEAR; //clear the screen
        printf(GREEN);
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
        printf(END);
        
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
                    choice = 0;
                    break;
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
                pthread_exit(NULL);

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

    info.sock_from = conn_fd;
    info.account_from = taccount;

    do {
        CLEAR;
        printf(GREEN);
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
        printf(END);

        scanf("%d", &choice);

        getchar();
        fflush(stdin);
        CLEAR;

        switch(choice) {
            
            case 1: {  //私聊
                printf(GREEN"Input your friend's account:\n"END);
                scanf("%d", &info.account_to);
                fflush(stdin);

                info.n = 1;

                while(1) {
                    fgets(info.buf, sizeof(info.buf), stdin);
printf("input = -%s- account = %d %d\n", info.buf, info.account_from, info.account_to);
                    if(!strcmp(info.buf, "exit\n"))
                        break;

                    if(send(conn_fd, &info, sizeof(info), 0) < 0)
                        err("send",__LINE__);
                }
            }
            break;


            case 2: {

            }
            break;

            
        }
    } while(choice);
}
        //         if(send(conn_fd, "3", 2, 0) < 0)
        //             err("send", __LINE__);

        //         if(recv(conn_fd, recv_buf, sizeof(recv_buf), 0) < 0)
        //             err("recv", __LINE__);

        //         if(recv_buf[0] == 'y') {
        //             CLEAR;
        //         } else {
        //             printf(RED"connect to server error\n"END);
        //             break;
        //         }


        //         printf(GREEN"-1 to display friends state, or input friend's account:\n"END);
        //         scanf("%d", &info.from);

        //         /*if(info.from == -1) {
        //             break;
        //         }*/

        //         if(send(conn_fd, &info, sizeof(info), 0) < 0)
        //             err("send", __LINE__);

        //         if(recv(conn_fd, recv_buf, sizeof(recv_buf), 0) < 0)
        //             err("recv", __LINE__);

        //         if(recv_buf[0] != 'y') {
        //             printf(RED"connect to server error\n"END);
        //             break;
        //         }

        //         while(1) {
        //             printf(BLUE"me: %s"END, my_time());
        //             fgets(recv_buf, sizeof(recv_buf), stdin);

        //             if(!strcmp(recv_buf, "exit")) {  //输入exit退出聊天
        //                 printf("GREEN ");
        //             }


        //             if(send(conn_fd, recv_buf, sizeof(recv_buf), 0) < 0)
        //                 err("send", __LINE__);

        //             if(recv(conn_fd, recv_buf, sizeof(recv_buf), 0) < 0)
        //                 err("recv", __LINE__);

        //             if(recv_buf[0] != 'y') {
        //                 printf(RED"send message failed.\n"END);
        //             }
        //         }
        //     }
        // }

