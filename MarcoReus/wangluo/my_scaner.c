/*************************************************************************
	> File Name: my_scaner.c
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年08月08日 星期二 19时23分14秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct port_segment {
    struct in_addr dest_ip;           //目标IP
    unsigned short int min_port;      
    unsigned short int max_port;
} port_segment;

void my_err(const char *err_string,int line) {
    fprintf(stderr,"line: %d ",line);
    perror(err_string);
    exit(1);
}

int do_scan(struct sockaddr_in serv_addr) {
    int conn_fd;
    int ret;

    conn_fd = socket(AF_INET,SOCK_STREAM,0);
    if(conn_fd < 0) {
        my_err("socket",__LINE__);
    }

    if((ret = connect(conn_fd,(struct sockaddr *)&serv_addr,sizeof(struct sockaddr) )) < 0) {
        if(errno == ECONNREFUSED) {
            close(conn_fd);
            return 0;
        } else {
            close(conn_fd);
            return -1;
        }
    } else if (ret == 0) {
        printf("port %d found in %s\n",ntohs(serv_addr.sin_port),inet_ntoa(serv_addr.sin_addr));
        close(conn_fd);
        return 1;
    }

    return -1;
}


void *scaner(void *arg) {
    unsigned short int i;
    struct sockaddr_in serv_addr;
    port_segment portinfo;

    memcpy(&portinfo,arg,sizeof(port_segment));

    memset(&serv_addr,0,sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = portinfo.dest_ip.s_addr;
    
    for(i = portinfo.min_port;i <= portinfo.max_port;i++) {
        serv_addr.sin_port = htons(i);
        if(do_scan(serv_addr) < 0) {
            continue;
        }
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t *thread;
    int max_port;
    int thread_num;
    int seg_len;
    struct in_addr dest_ip;
    int i;

    if(argc != 7) {
        printf("Usage: [-m] [max_port] [-a] [serv_address] [-n] [thread_number]\n");
        exit(0);
    }

    for(i = 0;i < argc;i++) {
        if(strcmp("-m",argv[i]) == 0) {
            max_port = atoi(argv[i+1]);
            if(max_port < 0 || max_port > 65535) {
                printf("Usage: invalid max dest port\n");
                exit(1);
            }
            continue;
        }
        
        if(strcmp("-a",argv[i]) == 0) {
            if(inet_aton(argv[i+1],&dest_ip) == 0) {
                printf("Usage:invalid dest ip adderss\n");
                exit(1);
            }
            continue;
        }

        if(strcmp("-n",argv[i]) == 0) {
            thread_num = atoi(argv[i+1]);
            if(thread_num <= 0) {
                printf("Usage:invalid thread_number\n");
                exit(1);
            }
            continue;
        }
    }
    
    if(max_port < thread_num) {
        thread_num = max_port;
    }
    
    seg_len = max_port / thread_num;
    if((max_port%thread_num) != 0) {
        thread_num += 1;
    }

    thread = (pthread_t *)malloc(thread_num*sizeof(pthread_t));

    for(i = 0;i < thread_num;i++) {
        port_segment portinfo;
        portinfo.dest_ip = dest_ip;
        portinfo.min_port = i * seg_len + 1;
        
        if(i == thread_num - 1) {
            portinfo.max_port = max_port;
        } else {
            portinfo.max_port = portinfo.min_port + seg_len - 1;
        }

        if(pthread_create(&thread[i],NULL,scaner,(void *)&portinfo) != 0) {
            my_err("pthread_create",__LINE__);
        }

        pthread_join(thread[i],NULL);
    }
    
    return 0;
}

