/*************************************************************************
	> File Name: func.h
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年07月26日 星期三 11时48分40秒
 ************************************************************************/


#ifndef _FUNC_H
#define _FUNC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <dirent.h>
#include "./myhead.h"

static int find_command(char *command);            //查找命令是否存在

/*打印刚开始的shell开头*/
void print_prompt() {
    char *msg;
    msg = (char *)malloc(200);
    getcwd(msg,100);
    printf("myshell$$ %s$",msg);
    free(msg);
}

/*获得用户输入的信息*/
void get_input(char *buf) {
    int len = 0;
    char ch;

    ch = getchar();
    while(len < 256 && ch != '\n') {
        buf[len++] = ch;
        ch = getchar();
    }
    
    if(len == 256) {
        printf("command is too long \n");
        exit(-1);
    }

    buf[len] = '\n';
    len++;
    buf[len] = 0;
}

/*解析命令，统计命令个数，并且以空格为分界放入arglist中*/
void explain_input(char *buf,int *argcount,char arglist[100][256]) {
    char *p = buf;
    char *q = buf;
    
    int number = 0;

    while(1) {
        if(p[0] == '\n') {  //遇到\n时结束
            break; 
        }
        if(p[0] == ' ') {
            p++;            //跳过空格
        } else {
            q = p;          //把q变成开始的有效位置
            number = 0;         
            while( (q[0] != '\n') && (q[0] != ' ') ) {
                number++;
                q++;
            }
            //统计命令长度和到结束时的位置
            strncpy(arglist[*argcount],p,number+1);     //复制到arglist中
            arglist[*argcount][number] = 0;             //末尾赋0
            *argcount = *argcount + 1;                  //移动到下一个字符串
            p = q;                                      //将p移动到末尾位置
        }
    }
}

/*运行命令*/
void do_cmd(int argcount,char arglist[100][256]) {
    int how = normal;
    int background = not_background;
    int flag = 0;
    int fd;
    char *arg[argcount+1];
    char *argnext[argcount+1];
    char *file;
    pid_t pid;
    int i;
    int status;

    for(i = 0;i < argcount;i++) {
        arg[i] = (char *)arglist[i];
    }
    arg[argcount] = NULL;
    
    for(i = 0;i < argcount;i++) {
        if(strncmp(arg[i],"&",1) == 0) {
            if(i == argcount - 1) {
                background = is_background;
                arg[argcount - 1] = NULL;
                break;
            } else{
                printf("wrong command\n");
                return;
            }
        }
    }
    for(i = 0;arg[i] != NULL;i++) {
        if(strcmp(arg[i],">") == 0) {
            flag++;
            how = out_redirect;
            if(arg[i] == NULL) {
                flag++;
            }
        }
        if(strcmp(arg[i],"<") == 0) {
            flag++;
            how = in_redirect;
            if(i == 0) {
                flag++;
            } 
        }
        if(strcmp(arg[i],"|") == 0) {
            flag++;
            how = have_pipe;
            if(strcmp(arg[i],"|") == 0) {
                flag++;
            }
            if(i == 0) {
                flag++;
            }
        }
    }
    
    if(flag > 1) {
        printf("wrong command\n");
        return;
    }
    
    if(how == out_redirect) {
        for(i = 0;arg[i] != NULL;i++) {
            if(strcmp(arg[i],">") == 0) {
                file = arg[i+1];
                arg[i] = NULL;
            }
        }
    }

    if(how == in_redirect) {
        for(i = 0;arg[i] != NULL;i++) {
            if(strcmp(arg[i],"<") == 0) {
                file = arg[i+1];
                arg[i] = NULL;
            }
        }
    }

    if(how == have_pipe) {
        for(i = 0;arg[i] != NULL;i++) {
            if(strcmp(arg[i],"|") == 0) {
                arg[i] = NULL;
                int j;
                for(j = i+1;arg[j+1] != NULL;) {
                    argnext[j-i-1] = arg[j];
                }
                argnext[j-i-1] = arg[j];
                break;
            }
        }
    }
    
    if((pid = fork()) < 0) {
        printf("fork error\n");
        return;
    }
    
    if(strcmp(arg[0],"cd") == 0) {
        if(argcount == 1 || (argcount == 2 && strcmp(arg[1],"~") == 0)) {
            chdir("home/yinjianxiang");
            return;
        }
        if(chdir(arg[1]) == -1) {
            perror("chdir fauled\n");
        }
        return;
    }
   /* 
    if(strcmp(arg[0],"history") == 0) {
        
    }
    
    */
    switch(how) {
        case normal:
            if(pid == 0) {
                if(!(find_command(arg[0]))) {
                    printf("%s : the command is not found\n",arg[0]);
                    exit(0);
                }
                execvp(arg[0],arg);
                exit(0);
            }
            break;
        
        case out_redirect:
            if(pid == 0) {
                if(!(find_command(arg[0]))) {
                    printf("%s : the command is not found\n",arg[0]);
                    exit(0);
                }
                fd = open(file,O_WRONLY|O_CREAT|O_TRUNC);
                dup2(fd,1);
                execvp(arg[0],arg);
                exit(0);
            }    
            break;
        case in_redirect:
            if(pid == 0) {
                if(!find_command(arg[0])) {
                    printf("%s : the command is not found\n",arg[0]);
                    exit(0);
                }
                fd = open(file,O_RDONLY);
                dup2(fd,0);
                execvp(arg[0],arg);
                exit(0);
            }    
            break;
        case have_pipe:
        if(pid == 0) {
            pid_t pid2;
            int status2;
            int fd2;

            if((pid2 == fork()) < 0) {
                printf("fork failed");
                return;
            }
            else if(pid2 == 0) {
                if(!command(arg[0])) {
                    printf("%s the command is not found\n",arg[0]);
                    exit(0);
                }
                fd2 = open("/tmp/youdonotknowfile",O_WRONLY|O_CREAT|O_TRUNC);
                dup2(fd2,1);
                execvp(arg[0],arg);
                exit(0);
            }
            if(waitpid(pid2,status2,0) == -1) {
                printf("wait for child process error\n");
            }

            if(!(find_command(argnext[0]))) {
                printf("%s the command is not found\n",argnext[0]);
                exit(0);
            }
            
            fd2 = open("/tep/youdonotknowfile",O_RDONLY);
            dup2(fd2,0);
            execvp(argnext[0],argnext);
            if(remove("/tep/youdonotknowfil:e")) {
                printf("remove failed\n");
            }
        }

        break;
        default:
            break;
    }
    if(background == is_background) {
        printf("[process id %d]\n",pid);
        return;
    }
    
    if(waitpid(pid,&status,1) == -1) {
        printf("wait for child process error\n");
    }
}

/*查找命令是否存在*/
static int find_command(char *command) {
    DIR *dp;
    struct dirent *dirp;
    char *path[] = {"./","/bin","/usr/bin",NULL};

    if(strncmp(command,"./",2) == 0) {
        command = command + 2;
    }
    
    int i = 0;
    while(path[i] != NULL) {
        if( (dp = opendir(path[i])) == NULL) {
            printf("can not open /bin \n");
        }
        while( (dirp = readdir(dp)) != NULL)  {
            if(strcmp(dirp->d_name,command) == 0) {
                closedir(dp);
                return 1;
            }
        }
        closedir(dp);
        i++;
    }
    return 0;
}




#endif
