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

/*是否为好友*/
int isFriend(struct message *info) {
    char filename[256], t[256];
    int a;
    FILE *fp;
    strcpy(filename, DIR_USER);
    sprintf(t, "%d", info->account_from);
    strcat(filename, t);
    strcat(filename, "/friends");

    pthread_mutex_lock(&mutex);
    fp = fopen(filename, "r");
    while(fscanf(fp, "%d", &a) != EOF) {
        if(a == info->account_to) {
            pthread_mutex_unlock(&mutex);
            return 1;
        }
    }
    fclose(fp);
    pthread_mutex_unlock(&mutex);

    return 0;
}

/*是否被注册*/
int isRegister(struct message *info) {
    char filename[256], t[256];
    int a, b;
    FILE *fp;
    strcpy(filename, DIR_USER);
    strcat(filename, "/userinfo");

    pthread_mutex_lock(&mutex);
    fp = fopen(filename, "r");
    while(fscanf(fp, "%s %d %d", t, &a, &b) != EOF) {
        if(a == info->account_to) {
            pthread_mutex_unlock(&mutex);
            return 1;
        }
    }
    fclose(fp);
    pthread_mutex_unlock(&mutex);

    return 0;
}

/*是否为群成员*/
int isGroup(struct message *info) {
    char filename[256], t[256];
    int a, x;
    FILE *fp;
    strcpy(filename, DIR_GROUP);
    sprintf(t, "%d", info->group);
    strcat(filename, t);
    strcat(filename, "/member");

    pthread_mutex_lock(&mutex);
    fp = fopen(filename, "r");
    while(fscanf(fp, "%d %d", &a, &x) != EOF) {
        if(a == info->account_from) {
            pthread_mutex_unlock(&mutex);
            return 1;
        }
    }
    fclose(fp);
    pthread_mutex_unlock(&mutex);

    return 0;
}

/*群是否已被创建*/
int inGroup(struct message *info) {
    char filename[256], t[256];
    FILE *fp;
    int x;

    strcpy(filename, DIR_GROUP);
    strcat(filename, "groupinfo");
    pthread_mutex_lock(&mutex);
    fp = fopen(filename, "r");
    while(fscanf(fp, "%d", &x) != EOF)
        if(x == info->group) {
            pthread_mutex_unlock(&mutex);
            return 1;
        }
    fclose(fp);
    pthread_mutex_unlock(&mutex);

    return 0;
}

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
                if(open("group_invitation", O_CREAT|O_TRUNC|O_RDWR, 0777) < 0) {
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
// printf("n = %d\n", n);
        switch(n)
        {
            case 1: {  //私聊
                char filename_t[32];

                if(!isFriend(&info)) {
                    info.n = 101;
                    if(send(conn_fd, &info, sizeof(info), 0) < 0)
                        err("send", __LINE__);
                    break;
                }

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
                    fprintf(fp, "%s (%d)%s%s", info.name_from, info.account_from, info.time, info.buf);
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
                int member[50][2] = {0}, i, j = 0, a;

                if(!inGroup(&info)) {
                    info.n = 81;
                    if(send(conn_fd, &info, sizeof(info), 0) < 0)
                        err("send", __LINE__);
                    break;
                }


                if(!isGroup(&info)) {
                    info.n = 30;
                    if(send(conn_fd, &info, sizeof(info), 0) < 0)
                        err("send", __LINE__);
                    break;
                }

                strcpy(info.time, my_time());
                chdir(DIR_GROUP);
                sprintf(filename, "%d", info.group);
                strcat(filename, "/member");
                pthread_mutex_lock(&mutex);
                fp = fopen(filename, "r");

                while(fscanf(fp, "%d %d", &member[j][0], &a) != EOF) { //获取群成员帐号
                    if(member[j][0] == info.account_from)
                        ;
                    else
                        j++;
                }
                fclose(fp);
                if(!member[j-1][0])
                    j--;

                for(i = 0; i < j; i++) {
                    p = pHead -> next;
                    while(p != NULL) {
                        if(p -> account == member[i][0]) {
                            member[i][1] = 1;  //标记已经发过的成员
                            if(send(p->user_fd, &info, sizeof(info), 0) < 0)
                                err("send", __LINE__);
                            break;
                        }
                        p = p -> next;
                    }

                    if(!member[i][1]) {  //用户不在线
                        
                        /*写入离线记录*/
                        strcpy(filename, DIR_USER);
                        sprintf(recv_buf, "%d", member[i][0]);
                        strcat(filename, recv_buf);
                        strcat(filename, "/off-online");

                        fp = fopen(filename, "at+");
                        fprintf(fp, "%d %s%s", info.account_from, info.time, info.buf);
                        fclose(fp);
                    }
                }

                /*写入群聊天记录*/
                strcpy(filename, DIR_GROUP);
                sprintf(recv_buf, "%d", info.group);
                strcat(filename, recv_buf);
                strcat(filename, "/chat_log");

                fp = fopen(filename, "at+");
                fprintf(fp, "%d %s%s", info.account_from, info.time, info.buf);
                fclose(fp);
                pthread_mutex_unlock(&mutex);
            }
            break;


            case 3: {//查看好友聊天记录
                chdir(DIR_USER);

                if(!isFriend(&info)) {
                    info.n = 101;
                    if(send(conn_fd, &info, sizeof(info), 0) < 0)
                        err("send", __LINE__);
                    break;
                }

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

                getchar();
                getchar();
            }
            break;


            case 31: {  //查看群聊记录

                if(isGroup(&info)) {
                    chdir(DIR_GROUP);
                    sprintf(filename, "%d", info.group);
                    strcat(filename, "/chat_log");

                    pthread_mutex_lock(&mutex);
                    fp = fopen(filename, "r");
                    while(fgets(info.buf, sizeof(info.buf), fp) != NULL)
                        if(send(conn_fd, &info, sizeof(info), 0) < 0)
                            err("send",__LINE__);
                    fclose(fp);
                    pthread_mutex_unlock(&mutex);
                } else {
                    info.n = 30;  //未加该群
                    if(send(conn_fd, &info, sizeof(info), 0) < 0)
                        err("send",__LINE__);
                }
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

                if(!isRegister(&info)) {
                    info.n = 102;
                    if(send(conn_fd, &info, sizeof(info), 0) < 0)
                        err("send", __LINE__);
                    break;
                }

                if(isFriend(&info)) {
                    info.n = 103;
                    if(send(conn_fd, &info, sizeof(info), 0) < 0)
                        err("send", __LINE__);
                    break;
                }

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

                getchar();
                getchar();
            }
            break;


            case 6: {  //删除好友
                int a[100], i, j = 0, t;

                if(!isFriend(&info)) {
                    info.n = 101;
                    if(send(conn_fd, &info, sizeof(info), 0) < 0)
                        err("send", __LINE__);
                    break;
                }
                
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

                if(send(conn_fd, &info, sizeof(info), 0) < 0)
                    err("send", __LINE__);

                getchar();
                getchar();
            }
            break;


            case 7: {  //查看好友列表
                strcpy(filename, DIR_USER);
                sprintf(recv_buf, "%d", info.account_from);
                strcat(filename, recv_buf);
                strcat(filename, "/friends");
                pthread_mutex_lock(&mutex);
                fp = fopen(filename, "r");
                while(fscanf(fp, "%d", &info.account_to) != EOF){
                    if(send(conn_fd, &info, sizeof(info), 0) < 0)
                        err("send", __LINE__);
                }
                pthread_mutex_unlock(&mutex);
            }
            break;


            case 8: {  //加群
                int flag_exist1 = 0, flag_exist2 = 0, t;
                strcpy(info.time, my_time());

                if(!inGroup(&info)) {
                    info.n = 81;
                    if(send(conn_fd, &info, sizeof(info), 0) < 0)
                        err("send", __LINE__);
                    break;
                }

                if(isGroup(&info)) {
                    info.n = 80;
                    if(send(p->user_fd, &info, sizeof(info), 0) < 0)
                        err("send", __LINE__);
                    break;
                }

                /*获取群主信息*/ 
                strcpy(filename, DIR_GROUP);
                sprintf(recv_buf, "%d", info.group);
                strcat(filename, recv_buf);
                strcat(filename, "/member");

                /*群主帐号*/
                fp = fopen(filename, "r");
                fscanf(fp, "%d", &info.account_to);
                fclose(fp);

                fp = fopen(USER_INFO, "r");
                while(fscanf(fp, "%s%d%s", tmp.name, &tmp.account, tmp.passwd) != EOF)
                    if(tmp.account == info.account_to) {
                        strcpy(info.name_to, tmp.name);
                        break;
                    }
                fclose(fp);

                /*群主是否在线*/
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

                /*写入群主加群申请文件*/
                strcpy(filename, DIR_USER);
                sprintf(recv_buf, "%d", info.account_to);
                strcat(filename, recv_buf);
                strcat(filename, "/group_invitation");

                fp = fopen(filename, "at+");
                fprintf(fp, "%d %d 1\n", info.account_from, info.group);
                fclose(fp);

                pthread_mutex_unlock(&mutex);

            }
            break;


            case 81: {  //同意好友加群邀请
                /*添加至群成员文件*/
                strcpy(filename, DIR_GROUP);
                sprintf(recv_buf, "%d", info.group);
                strcat(filename, recv_buf);
                strcat(filename, "/member");

                pthread_mutex_lock(&mutex);
                fp = fopen(filename, "at+");
                fprintf(fp, "%d 2\n", info.account_from);
                fclose(fp);

                /*将群添加到自己的文件*/
                strcpy(filename, DIR_USER);
                sprintf(recv_buf, "%d", info.account_from);
                strcat(filename, recv_buf);
                strcat(filename, "/groups");

                fp = fopen(filename, "at+");
                fprintf(fp, "%d\n", info.group);
                fclose(fp);

                pthread_mutex_unlock(&mutex);
            }
            break;


            case 9: {  //退群
                int t[30][2], i = 0, j = 0, flag_header = 0;

                if(!inGroup(&info)) {
                    info.n = 81;
                    if(send(conn_fd, &info, sizeof(info), 0) < 0)
                        err("send", __LINE__);
                    break;
                }

                if(!isGroup(&info)) {
                    info.n = 80;
                    if(send(conn_fd, &info, sizeof(info), 0) < 0)
                        err("send", __LINE__);
                    break;
                }

                chdir(DIR_GROUP);
                sprintf(recv_buf, "%d", info.group);
                strcat(filename, recv_buf);
                strcat(filename, "/member");

                /*删除群文件内信息*/
                pthread_mutex_lock(&mutex);
                fp = fopen(filename, "r");

                while(fscanf(fp, "%d %d", &t[i][0], &t[i][1]) != EOF){
                    if(t[0][0] == info.account_from) {
                        info.n = 90;
                        if(send(conn_fd, &info, sizeof(info), 0) < 0)
                            err("send", __LINE__);
                        flag_header = 1;
                        break;
                    }
                    if(t[i][0] != info.account_from)
                        i++;
                }
                fclose(fp);

                if(!flag_header) {
                    fp = fopen(filename, "w");
                    for(j = 0; j < i; j++)
                        fprintf(fp, "%d %d\n", t[j][0], t[j][1]);
                    fclose(fp);

                    /*删除自己的群文件信息*/
                    strcpy(filename, DIR_USER);
                    sprintf(recv_buf, "%d", info.account_from);
                    strcat(filename, recv_buf);
                    strcat(filename, "/groups");

                    i = 0;
                    j = 0;
                    fp = fopen(filename, "r");
                    while(fscanf(fp, "%d", &t[i][0]) != EOF) {

                        if(t[i][0] != info.group)
                            i++;
                    }
                    fclose(fp);

                    /*重写文件信息*/
                    fp = fopen(filename, "w");
                    for(j = 0; j < i; j++)
                        fprintf(fp, "%d\n", t[j][0]);
                    fclose(fp);

                    pthread_mutex_unlock(&mutex);
                } else {
                    break;
                }
                
                if(send(conn_fd, &info, sizeof(info), 0) < 0)
                    err("send", __LINE__);
            }
            break;


            case 10: {  //建群
                int a, flag_exist = 0;
                pthread_mutex_lock(&mutex);
                chdir(DIR_GROUP);

                /*查看群号是否已存在*/
                fp = fopen("groupinfo", "r");
                while(fscanf(fp, "%d", &a) != EOF)
                    if(a == info.group) {
                        flag_exist = 1;
                        break;
                    }
                fclose(fp);

                if(!flag_exist) {//群号不存在
                    fp = fopen("groupinfo", "at+");
                    fprintf(fp, "%d\n", info.group);
                    fclose(fp);

                    sprintf(recv_buf, "%d", info.group);
                    mkdir(recv_buf, 0777);
                    chdir(recv_buf);
                    
                    if(open("member", O_CREAT|O_TRUNC|O_RDWR, 0777) < 0)
                        err("open", __LINE__);
                    if(open("chat_log", O_CREAT|O_TRUNC|O_RDWR, 0777) < 0)
                        err("open", __LINE__);

                    fp = fopen("member", "w");
                    fprintf(fp, "%d 1\n", info.account_from);//写入群成员文件
                    fclose(fp);

                    strcpy(filename, DIR_USER);
                    sprintf(recv_buf, "%d", info.account_from);
                    strcat(filename, recv_buf);
                    strcat(filename, "/groups");

                    fp = fopen(filename, "at+");
                    fprintf(fp, "%d\n", info.group);//写入文件：用户已加入的群
                    fclose(fp);

                    if(send(conn_fd, &info, sizeof(info), 0) < 0)
                        err("send", __LINE__);
                } else {
                    info.n = 100;
                    if(send(conn_fd, &info, sizeof(info), 0) < 0)
                        err("send", __LINE__);
                }

                pthread_mutex_unlock(&mutex);
            }
            break;


            case 11: {  //解散群
                /*判断是否为群主*/
                int member[50], t[50], i, j = 0, k = 0, n, a;

                if(!inGroup(&info)) {
                    info.n = 81;
                    if(send(conn_fd, &info, sizeof(info), 0) < 0)
                        err("send", __LINE__);
                    break;
                }

                strcpy(filename, DIR_GROUP);
                sprintf(recv_buf, "%d", info.group);
                strcat(filename, recv_buf);
                strcat(filename, "/member");

                pthread_mutex_lock(&mutex);
                fp = fopen(filename, "r");
                fscanf(fp, "%d %d", &member[j], &a); //获取文件第一个，即群主帐号
                if(member[j] != info.account_from) {
                    info.n = 110;  //不是群主
                    if(send(conn_fd, &info, sizeof(info), 0) < 0)
                        err("send", __LINE__);
                    break;
                }

                j++;
                while(fscanf(fp, "%d %d", &member[j++], &a) !=EOF)  //获取文件剩余群成员帐号
                    ;
                fclose(fp);

                if(!member[j-1]) //最后不是0
                    j--;

                /*删除群目录、群成员group信息*/
                for(i = 0; i < j; i++) {
                    k = 0;
                    strcpy(filename, DIR_USER);
                    sprintf(recv_buf, "%d", member[i]);
                    strcat(filename, recv_buf);
                    strcat(filename, "/groups");

                    /*获取群成员加群信息*/
                    fp = fopen(filename, "r");
                    while(fscanf(fp, "%d", &t[k]) != EOF) {
                        if(t[k] != info.group)
                            k++;
                    }
                    fclose(fp);

                    /*重写文件，删除要解散的群*/
                    fp = fopen(filename, "w");
                    for(n = 0; n < k; n++) {

                        fprintf(fp, "%d\n", t[n]);
                    }
                    fclose(fp);
                }

                /*删除群目录*/
                strcpy(filename, DIR_GROUP);
                sprintf(recv_buf, "%d", info.group);
                strcat(filename, recv_buf);
                chdir(filename);
                remove("chat_log");
                remove("member");

                remove(filename);

                /*删除groupinfo中相关群信息*/
                j = 0;
                strcpy(filename, DIR_GROUP);
                strcat(filename, "groupinfo");

                fp = fopen(filename, "r");
                while(fscanf(fp, "%d", &member[j]) != EOF)
                    if(member[j] != info.group)
                        j++;
                fclose(fp);

                if(!member[j-1])  //保证没有多余的0
                    j--;

                fp = fopen(filename, "w");
                for(i = 0; i < j; i++)
                    fprintf(fp, "%d\n", member[i]);
                fclose(fp);
                pthread_mutex_unlock(&mutex);

                if(send(conn_fd, &info, sizeof(info), 0) < 0)
                    err("send", __LINE__);
            }
            break;


            case 12: {  //邀请好友加群
                int flag_exist = 0, t;
                pthread_mutex_lock(&mutex);

                /*群是否存在*/
                chdir(DIR_GROUP);
                fp = fopen("groupinfo", "r");
                while(fscanf(fp, "%d", &t) != EOF)
                    if(t == info.group) {
                        flag_exist = 1;
                        break;
                    }
                fclose(fp);
                if(!flag_exist) { //群未创建
                    info.n = 81;
                    if(send(conn_fd, &info, sizeof(info), 0) < 0)
                        err("send", __LINE__);
                    break;
                }

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
                sprintf(recv_buf, "%d", info.account_to);
                strcat(filename, recv_buf);
                strcat(filename, "/group_invitation");

                /*写入离线group_invitation文件*/
                fp = fopen(filename, "at+");
                fprintf(fp, "%d %d 2\n", info.account_from, info.group);
                fclose(fp);

                pthread_mutex_unlock(&mutex);
            }
            break;


            case 131: {  //处理加好友请求
                strcpy(filename, DIR_USER);
                sprintf(recv_buf, "%d", info.account_from);
                strcat(filename, recv_buf);
                strcat(filename, "/invitation");

                pthread_mutex_lock(&mutex);
                fp = fopen(filename, "r");
                while(fscanf(fp, "%d", &info.account_to) !=EOF)
                    if(send(conn_fd, &info, sizeof(info), 0) < 0)
                        err("send", __LINE__);
                fclose(fp);

                /*清空文件*/
                fp = fopen(filename, "w");
                fclose(fp);

                info.n = 1;
                info.flag = 3;
                if(send(conn_fd, &info, sizeof(info), 0) < 0)
                    err("send", __LINE__);

                pthread_mutex_unlock(&mutex);
            }
            break;



            case 1311: { //同意添加好友

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

            case 1310: {  //不同意添加好友
                
                ///////////////////  

            }
            break;

            case 132: {  //处理加群请求
                int t;
                info.n = 132;
                strcpy(filename, DIR_USER);
                sprintf(recv_buf, "%d", info.account_from);
                strcat(filename, recv_buf);
                strcat(filename, "/group_invitation");

                pthread_mutex_lock(&mutex);
                fp = fopen(filename, "r");
                while(fscanf(fp, "%d %d %d", &info.account_to, &info.group, &t) !=EOF) {
                    if(t == 2){
                        info.n = 8;
                    }
                    if(send(conn_fd, &info, sizeof(info), 0) < 0)
                        err("send", __LINE__);
                }

                fclose(fp);

                /*清空文件*/
                fp = fopen(filename, "w");
                fclose(fp);

                info.n = 1;
                info.flag = 3;
                if(send(conn_fd, &info, sizeof(info), 0) < 0)
                    err("send", __LINE__);

                pthread_mutex_unlock(&mutex);
            }
            break;


            case 1321: {  //同意加群请求

                pthread_mutex_lock(&mutex);
                strcpy(filename, DIR_USER);
                sprintf(recv_buf, "%d", info.account_to);
                strcat(filename, recv_buf);
                strcat(filename, "/groups");

                fp = fopen(filename, "at+");
                fprintf(fp, "%d\n", info.group);
                fclose(fp);

                strcpy(filename, DIR_GROUP);
                sprintf(recv_buf, "%d", info.group);
                strcat(filename, recv_buf);
                strcat(filename, "/member");

                fp = fopen(filename, "at+");
                fprintf(fp, "%d 2\n", info.account_to);
                fclose(fp);
                pthread_mutex_unlock(&mutex);
            }
            break;


            case 1320: {  //拒绝加群请求
                ///////
            }
            break;


            case 14: {
                int i = 0;
                if(!inGroup(&info)) {
                    info.n = 81;
                    if(send(conn_fd, &info, sizeof(info), 0) < 0)
                        err("send", __LINE__);
                    break;
                }

                if(!isGroup(&info)) {
                    info.n = 30;
                    if(send(conn_fd, &info, sizeof(info), 0) < 0)
                        err("send", __LINE__);
                    break;
                }

                chdir(DIR_GROUP);
                sprintf(filename, "%d", info.group);
                strcat(filename, "/member");

                pthread_mutex_lock(&mutex);
                fp = fopen(filename, "r");
                while(fscanf(fp, "%d %d", &info.state[i][0], &info.state[i++][1]) != EOF)
                    ;
                fclose(fp);

                pthread_mutex_unlock(&mutex);

                if(!info.state[i-1][0])
                    i--;

                info.num = i;

                if(send(conn_fd, &info, sizeof(info), 0) < 0)
                    err("send", __LINE__);
            }
            break;

            case 999: {
                info.n = 31;
                strcpy(filename, DIR_USER);
                sprintf(recv_buf, "%d", info.account_from);
                strcat(filename, recv_buf);
                strcat(filename, "/off-online");

                pthread_mutex_lock(&mutex);
                fp = fopen(filename, "r");
                while(fgets(info.buf, sizeof(info.buf), fp) != NULL) {
                    if(send(conn_fd, &info, sizeof(info), 0) < 0)
                        err("send", __LINE__);
                }
                fclose(fp);

                fp = fopen(filename, "w");
                fclose(fp);
                pthread_mutex_unlock(&mutex);
            }
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
