#include "myhead.h"

struct online_user *pHead = NULL;
struct online_user *pEnd, *p, *pNew;
pthread_once_t once = PTHREAD_ONCE_INIT;
pthread_mutex_t mutex;
int flag_init;

struct userinfo1 {
    char username[32];
    char password[32];
};

int find_name(int account, char *passwd) {
    int i, flag = 0;
    FILE *fp = NULL;
    struct userinfo tmp;

    if((fp = fopen(USER_INFO, "r")) == NULL)
        err("fopen", __LINE__);

    while((fscanf(fp, "%s%d%s", tmp.name, &tmp.account, tmp.passwd) != EOF) && (tmp.account != account))
        ;

    if(tmp.account != account)
        return -2;
    strcpy(passwd, tmp.passwd);

    fclose(fp);

    return 0;
}

void send_data(int conn_fd, char *string) {
    if(send(conn_fd, string, strlen(string), 0) < 0)
    err("send", __LINE__);
}

/*void init(void) {
    pHead = (struct online_user *)malloc(sizeof(struct online_user));
    pEnd = pHead;
    pHead -> next = NULL;    
}*/

void *service(void *arg) {
    int ret, n, account = 0;
    int conn_fd = *(int *) arg, to_fd;
    int choice;
    int flag = 0, flag_online = 0;;
    char recv_buf[ BUFSIZE ];
    char filename[256];
    struct userinfo tmp;
    struct message info;
    FILE *fp = NULL;

    if(!flag_init) {
        pHead = (struct online_user *)malloc(sizeof(struct online_user));
        pEnd = pHead;
        pEnd -> next = NULL;
        flag_init = 1;
    }


    /*登陆注册*/
    do {
        if((ret = recv(conn_fd, recv_buf, sizeof(recv_buf), 0)) < 0){  //接收choice

            err("recv", __LINE__);
        }

        recv_buf[ret-1] = 0;
        choice = atoi(recv_buf);

        if(send(conn_fd, "y", 2, 0) < 0)
            err("send", __LINE__);

        switch(choice) {

            case 1: {     //登录
                char passwd[21];
                while(1){
                    if((ret = recv(conn_fd, recv_buf, sizeof(recv_buf), 0)) < 0)
                        err("recv", __LINE__);
                    recv_buf[ret-1] = 0;

                    if(flag == USERNAME) {
                        account = atoi(recv_buf);

                        n = find_name(account, passwd);

                        if(n == -2)
                            send_data(conn_fd, "n");
                        else {
                            send_data(conn_fd, "y\n");
                            flag = PASSWORD;
                        }
                    } else if (flag == PASSWORD) {
                        if(!strcmp(passwd, recv_buf)) {
                            send_data(conn_fd, "y\n");
                            printf("%d login, %s\n", account, my_time());

                            fp = fopen(SERVER_LOG, "at+");
                            fprintf(fp, "%d login, %s\n", account, my_time());  //写入服务器日志
                            fclose(fp);

                            choice = 0;

                            /*添加为在线状态*/
                            pthread_mutex_lock(&mutex);
                            pNew = (struct online_user *)malloc(sizeof(struct online_user));
                            pNew -> user_fd = conn_fd;
                            pNew -> account = account;
                            pEnd -> next = pNew;
                            pEnd = pNew;
                            pNew -> next = NULL;
                            pthread_mutex_unlock(&mutex);
                            break;
                        }
                        else
                            send_data(conn_fd, "n");
                    }
                }
            }
            break;


            case 2: {   //注册
                while(1) {
                    if((ret = recv(conn_fd, &account, sizeof(account), 0)) < 0)
                        err("recv", __LINE__);
                    if(!account)
                        break;

                    if((fp = fopen(USER_INFO, "r")) == NULL)
                        err("fopen", __LINE__);
                    flag = 0;
                    while(fscanf(fp, "%s%d%s", tmp.name, &tmp.account, tmp.passwd) != EOF)
                        if(account == tmp.account)
                            flag = 1;

                    fclose(fp);

                    if(flag)
                        send(conn_fd, "n", 2, 0);
                    else{
                        send(conn_fd, "y", 2, 0);
                        break;
                    }
                }

                while(1) {
                    if((fp = fopen(USER_INFO, "ab+")) == NULL)
                        err("fopen", __LINE__);

                    if(recv(conn_fd, recv_buf, sizeof(recv_buf), 0) < 0)
                        err("recv", __LINE__);
                    memcpy(&tmp, recv_buf, sizeof(recv_buf));
                    fprintf(fp, "\n%s %d %s", tmp.name, tmp.account, tmp.passwd);

                    fclose(fp);

                    if(send(conn_fd, "y", 2, 0) < 0) {
                        err("conn_fd", __LINE__);
                    } else {
                        break;
                    }
                }

                sprintf(recv_buf, "%d", tmp.account);

                if(chdir(DIR_USER) < 0)
                    err("chdir", __LINE__);
                if(mkdir(recv_buf, 0777) < 0)
                    err("mkdir", __LINE__);
                chdir(recv_buf);
                
                if(open("friends", O_CREAT|O_TRUNC|O_RDWR, 0777) < 0) {
                    err("open", __LINE__);
                }
                if(open("groups", O_CREAT|O_TRUNC|O_RDWR, 0777) < 0) {
                    err("open", __LINE__);
                }
                if(open("off-online", O_CREAT|O_TRUNC|O_RDWR, 0777) < 0) {
                    err("open", __LINE__);
                }
                if(open("invitation", O_CREAT|O_TRUNC|O_RDWR, 0777) < 0) {
                    err("open", __LINE__);
                }
                if(mkdir("chat_log", 0777) < 0)
                    err("mkdir", __LINE__);

                printf("%d register success, %s\n", tmp.account, my_time());
            }
            break;

            case 0: {   //退出，离线状态

                pthread_mutex_lock(&mutex);
                p = pHead -> next;
                while(p != NULL) {
                    if(p -> account == account) {
                        pEnd -> next = p -> next;
                        break;
                    }
                    pEnd -> next = p;
                    p = p -> next;
                }
                pthread_mutex_unlock(&mutex);
            }
        }
    } while(choice);

        /*case 3: {
            flag = 0;
            if(recv(conn_fd, recv_buf, sizeof(recv_buf), 0) < 0)
                err("recv", __LINE__);
            memcpy(&info, recv_buf, sizeof(recv_buf));

            pthread_mutex_lock(&mutex);
            p = pHead -> next;
            while(p != NULL) {
                if(p -> account == info.account) {
                    info.to = p -> user_fd;
                    to_fd = user_fd;
                    flag = 1;
                    break;
                }
            }
            pthread_mutex_unlock(&mutex);

            fp = fopen(USER_INFO, "r");
            while(fscanf("%s%d%s", tmp.name, &tmp.account, tmp.passwd) != EOF) {
                if(tmp.account == info.from) {
                    strcpy(info.name, tmp.name);
                    break;
                }
            }
            fclose(fp);

            if(send(info.to, "y", 2, 0) < 0)
                err(send,__LINE__);

            while(1) {
                if(recv(conn_fd, recv_buf, sizeof(recv_buf), 0) < 0)
                    err("recv", __LINE__);
                if(send(to_fd, recv_buf, sizeof(recv_buf), 0) < 0) {
                    if(send(conn_fd, "n", 2, 0) < 0)
                        err("send_return", __LINE__);
                } else {
                    if(send(conn_fd, "y", 2, 0) < 0)
                        err("send", __LINE__); 
                }

                

        }
        break;*/

   

    /*执行客户端指令*/
    while(1) {

        int ret = 0;  //存储recv返回值

        if((ret = recv(conn_fd, &info, sizeof(info), 0)) < 0)
            err("recv", __LINE__);
        if(!ret) {  //返回值为0代表客户端退出
            printf("%d exit,  %s\n", info.account_from, my_time());

            fp = fopen(SERVER_LOG, "at+");
            fprintf(fp, "%d exit,  %s\n", info.account_from, my_time());  //写入服务器日志
            fclose(fp);

            pthread_exit(NULL);
        }

         /*获取发送者用户名*/
        fp = fopen(USER_INFO, "r");
        while(fscanf(fp, "%s%d%s", tmp.name, &tmp.account, tmp.passwd) != EOF)
            if(tmp.account ==  info.account_from) {
                strcpy(info.name_from, tmp.name);
                break;
            }
        fclose(fp);


        /*写入时间*/
        strcpy(info.time, my_time());
        
        n = info.n;
        flag_online = 0;  //标记是否在线

        switch(n)
        {
            case 1: {  //私聊
                char filename_t[32];

                /*获取接收者信息*/ 
                fp = fopen(USER_INFO, "r");
                while(fscanf(fp, "%s%d%s", tmp.name, &tmp.account, tmp.passwd) != EOF)
                    if(tmp.account == info.account_to) {
                        strcpy(info.name_to, tmp.name);
                        break;
                    }
                fclose(fp);
                
                pthread_mutex_lock(&mutex);
                p = pHead -> next;
                while(p != NULL) {
                    if(p -> account == info.account_to) {
                        info.sock_to = p -> user_fd;
                        flag_online = 1;  //在线
                        break;
                    }
                    p = p -> next;
                }

                sprintf(recv_buf, "%d", info.account_to);  //将account转换为字符串, 接收者文件名
                sprintf(filename_t, "%d", info.account_from);  //发送者文件名
                strcpy(filename, DIR_USER);
                strcat(filename, recv_buf);
                chdir(filename);     

                if(flag_online) {  //在线
                    if(send(info.sock_to, &info, sizeof(info), 0) < 0)
                        err("send", __LINE__);
                } else {   //离线状态

                    /*写入离线文件*/
                    fp = fopen("off-online", "at+");
                    fprintf(fp, "%d %s\n%s%s", info.account_from, info.name_from, info.time, info.buf);
                    fclose(fp);
                }
                pthread_mutex_unlock(&mutex);


                /*写入接收者聊天记录*/
                if(chdir("chat_log") < 0)
                    err("chdir", __LINE__);
                fp = fopen(filename_t, "at+");
                fprintf(fp, "%d  %s  %s%s", info.account_from, info.name_from, info.time, info.buf);
                fclose(fp);

                /*写入发出者聊天记录*/
                strcpy(filename, DIR_USER);
                strcat(filename, filename_t);
                strcat(filename, "/chat_log/");
                chdir(filename);
                fp = fopen(recv_buf, "at+");
                fprintf(fp, "me :  %s%s", info.time, info.buf);
                fclose(fp);
            }
            break;


            case 2: {  //群聊

            }
            break;


            case 3: {//查看聊天记录
                chdir(DIR_USER);
                sprintf(filename, "%d", info.account_from);
                strcat(filename, "/chat_log/");
                sprintf(recv_buf, "%d", info.account_to);
                strcat(filename, recv_buf);
                fp = fopen(filename, "r");
                while(fscanf(fp, "%d%s%s%s", &info.account_from, info.name_from, info.time, info.buf) != EOF)
                    if(send(conn_fd, &info, sizeof(info), 0) < 0){
                        printf("send to %d error,   line: %d\n", conn_fd, __LINE__);
                    }
                fclose(fp);
            }
            break;


            case 4: {  //好友在线状态
                int i = 0;
                p = pHead -> next;
                while(p != NULL) {
                    info.state[i++][0] = p -> account;
                    p = p -> next;
                }
                info.num = i;

                if(send(conn_fd, &info, sizeof(info), 0) < 0)
                    err("send", __LINE__);
            }
            break;


            case 5: {  //添加好友
                char filename_t[32];

                /*strcpy(filename, DIR_USER);
                sprintf(filename_t, "%d", info.account_from);
                strcat(filename, filename_t);
                strcat(filename, "/invitation");

                pthread_mutex_lock(&mutex);

                fp = fopen(filename, "r");
                while(fscanf(fp, "%d %s ", info.account_to, info.name_to) != EOF){
                    fgets(info.time, sizeof(info.time), fp);

                    if(send(conn_fd, &info, sizeof(info), 0) < 0)
                        err("senf",__LINE__);
                }
                fclose(fp);*/

                /*获取接收者信息*/ 
                fp = fopen(USER_INFO, "r");
                while(fscanf(fp, "%s%d%s", tmp.name, &tmp.account, tmp.passwd) != EOF)
                    if(tmp.account == info.account_to) {
                        strcpy(info.name_to, tmp.name);
                        break;
                    }
                fclose(fp);
                
                p = pHead -> next;
                while(p != NULL) {      
                    if(p -> account == info.account_to) {
                        info.sock_to = p -> user_fd;
                        flag_online = 1;  //在线
                        break;
                    }
                    p = p -> next;
                }

                if(flag_online) {  //在线
                    if(send(info.sock_to, &info, sizeof(info), 0) < 0)
                        err("send", __LINE__);
                }

                strcpy(filename, DIR_USER);
                sprintf(filename_t, "%d", info.account_to);
                strcat(filename, filename_t);
                strcat(filename, "/invitation");

                /*写入离线invitation文件*/
                fp = fopen(filename, "at+");
                fprintf(fp, "%d\n", info.account_from);
                fclose(fp);

                pthread_mutex_unlock(&mutex);

            }
            break;


            case 6: {  //删除好友
                int a[100], i, j = 0, t;
                
                /*删除自己文件信息*/
                strcpy(filename, DIR_USER);
                sprintf(recv_buf, "%d", info.account_from);
                strcat(filename, recv_buf);
                strcat(filename, "/friends");

                pthread_mutex_lock(&mutex);
                fp = fopen(filename, "r");
                while(fscanf(fp, "%d", &t) !=EOF)
                    if(t != info.account_to)
                        a[j++] = t;
                fclose(fp);

                fp = fopen(filename, "w");
                for(i = 0; i < j; i++)
                    fprintf(fp, "%d\n", a[i]);
                fclose(fp);

                /*删除对方文件信息*/
                strcpy(filename, DIR_USER);
                sprintf(recv_buf, "%d", info.account_to);
                strcat(filename, recv_buf);
                strcat(filename, "/friends");

                pthread_mutex_lock(&mutex);
                fp = fopen(filename, "r");
                if(t != info.account_to)
                        a[j++] = t;
                fclose(fp);

                fp = fopen(filename, "w");
                for(i = 0; i < j; i++)
                    fprintf(fp, "%d\n", a[i]);
                fclose(fp);
            }
            break;


            case 131: {  //处理加好友请求
                strcpy(filename, DIR_USER);
                sprintf(recv_buf, "%d", info.account_from);
                strcat(filename, recv_buf);
                strcat(filename, "/friends");

                pthread_mutex_lock(&mutex);
                fp = fopen(filename, "r");
                while(fscanf(fp, "%d", &info.account_to) !=EOF)
                    if(send(conn_fd, &info, sizeof(info), 0) < 0)
                        err("send", __LINE__);
                fclose(fp);

                info.n = 1;

                fp = fopen(filename, "w");
                fclose(fp);

                info.flag = 3;
                if(send(conn_fd, &info, sizeof(info), 0) < 0)
                    err("send", __LINE__);

                pthread_mutex_unlock(&mutex);
            }
            break;

            case 1311: { //同意添加

                pthread_mutex_lock(&mutex);
                strcpy(filename, DIR_USER);
                sprintf(recv_buf, "%d", info.account_from);
                strcat(filename, recv_buf);
                strcat(filename, "/friends");

                fp = fopen(filename, "at+");
                fprintf(fp, "%d\n", info.account_to);
                fclose(fp);

                strcpy(filename, DIR_USER);
                sprintf(recv_buf, "%d", info.account_to);
                strcat(filename, recv_buf);
                strcat(filename, "/friends");

                fp = fopen(filename, "at+");
                fprintf(fp, "%d\n", info.account_from);
                fclose(fp);
                pthread_mutex_unlock(&mutex);
            }
            break;

            case 1310: {  //不同意
                
                ///////////////////  

            }
            break;

            case 132: {

            }
            break;
        }
    }
}


int main(void) {
    int fd, ret;
    int sock_fd, conn_fd;
    int optval;
    int flag_recv = USERNAME;
    pthread_t tid;
    socklen_t cli_len;
    struct sockaddr_in cli_addr, serv_addr;
    char recv_buf[5]; //接收choice

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_fd < 0)
        err("socket", __LINE__);

    optval = 1;
    if(setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&optval, sizeof(int)) < 0)
        err("setsockopt", __LINE__);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(sock_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in)) < 0)
        err("bind", __LINE__);

    if(listen(sock_fd, LISTENSIZE))

    cli_len = sizeof(struct sockaddr_in);
    while(1) {
        conn_fd = accept(sock_fd, (struct sockaddr *)&cli_addr, &cli_len);
        if(conn_fd < 0)
            err("accept", __LINE__);

        printf("accept a new client, ip: %s\n", inet_ntoa(cli_addr.sin_addr));
        
        if(pthread_create(&tid, NULL, service, (void *)&conn_fd) < 0) {
            err("pthread_create", __LINE__);
        }
    }

    return 0;
}
