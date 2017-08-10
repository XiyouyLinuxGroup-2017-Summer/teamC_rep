#include "myhead.h"

#define INVALID_USERINFO 'n'
#define VALID_USERINFO   'y'


/*函数声明*/
int get_userinfo(char *buf, int len);
void input_userinfo(int conn_fd, char *string);
void menu_login(int conn_fd);
void login(int conn_fd);
void my_register(int conn_fd);



int main(int argc, char **argv) {
    int conn_fd, serv_port;
    struct sockaddr_in serv_addr;
    char recv_buf[BUFSIZE];
    int i, ret;

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

    close(conn_fd);

    return 0;
}

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
                /*chat menu*/

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
int get_userinfo(char *buf, int len) {
    int i;

    if(buf == NULL)
        return -1;

    i = 0;
    while((buf[i++] = getchar()) != '\n' && i < len-1)
        ;
    buf[i-1] = 0;

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
        if(get_userinfo(input_buf, NAMESIZE) < 0)
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
