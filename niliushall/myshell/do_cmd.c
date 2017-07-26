/*************************************************************************
	> File Name: do_cmd.c
	> Author: niliushall
    > Email: sdwllinux@gmail.com
	> Created Time: 2017年07月25日 星期二 15时51分41秒
 ************************************************************************/

#include "myshell.h"
void do_cmd(int argcount, char arglist[][256]){
    int flag = 0;
    int how = 0;
    int background = 0;
    int status, i, fd;
    char * arg[argcount + 1];
    char * argnext[argcount + 1];
    char *file;
    pid_t pid;

    /*将命令取出*/
    for(i = 0; i < argcount; i++){
        arg[i] = (char *)arglist[i];
    }
    arg[argcount] = NULL;

    /*查看命令行是否有后台运算符*/
    for(i = 0; i < argcount; i++){
        if(!strncmp(arg[i], "&", 1)){
            if(i == argcount - 1){
                background = 1;
                arg[argcount - 1] = NULL;
                break;
            }
            else{
                printf("Wrong command\n");
                return;
            }
        }
    }

    for(i = 0; arg[i] != NULL; i++){
        if(!strcmp(arg[i], ">")){ //存在输出重定向
            flag++;
            how = OUT_REDIRECT;
            if(arg[i+1] == NULL)
                flag++;
        }
        else if(!strncmp(arg[i], "<", 1)){
            flag++;
            how = IN_REDIRECT;
            if(!i)
                flag++;
        }
        else if(!strncmp(arg[i], "|", 1)){
            flag++;
            how = HAVE_PIPE;
            if(arg[i+1] == NULL || !i)
                flag++;
        }
    }

    /*存在多个重定向和 | 符号，或格式不对*/
    if(flag > 1){
        printf("Wrong commang\n");
        return;
    }

    if(how == OUT_REDIRECT){ //含输出重定向
        for(i = 0; arg[i] != NULL; i++){
            if(!strncmp(arg[i], ">", 1)){
                file = arg[i + 1];
                arg[i] = NULL;
            }
        }
    } 
    else if(!strncmp(arg[i], ">", 1)){
        for(i = 0; arg[i] != NULL; i++){
            file = arg[i + 1];
            arg[i] = NULL;
        }
    }
    else if(how == HAVE_PIPE){
        if(!strncmp(arg[i], "|", 1)){
            for(i = 0; arg[i] != NULL; i++){
                arg[i] = NULL;
                int j = i+1;
                for( ; arg[i] != NULL; i++ )
                    argnext[j - i - 1] = arg[j];
                break;
            }
        }
    }

    if((pid = fork()) < 0){
        printf("fork error\n");
        return;
    }

    switch(how){
        case 0:
        if(!pid){
            if(!find_cmd(arg[0])){
                printf("%s: command not found.\n", arg[0]);
                exit(0);
            }
            execvp(arg[0], arg);
            exit(0);
        }
        break;

        case 1:
        if(!pid){
            if(!find_cmd(arg[0])){
                printf("%s: command not found\n", arg[0]);
                exit(0);
            }

            if((fd = open(file, O_RDWR|O_CREAT|O_TRUNC, 0644)) < 0){
                printf("%s: file creation failed\n", file);
                exit(0);
            }
            dup2(fd, 1);
            execvp(arg[0], arg);
            exit(0);
        }
        break;

        case 2:
        if(!pid){
            if(!find_cmd(arg[0])){
                printf("%s: command not found\n", arg[0]);
                exit(0);
            }
            if((fd = open(file, O_RDONLY)) < 0){
                printf("%s: can not open\n", file);
                exit(0);
            }
            dup2(fd, 0);
            execvp(arg[0], arg);
            exit(0);
        }
        break;
        
    
    }
}
