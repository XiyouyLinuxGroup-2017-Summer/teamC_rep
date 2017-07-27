/*************************************************************************
	> File Name: do_cmd.c
	> Author: niliushall
    > Email: sdwllinux@gmail.com
	> Created Time: 2017年07月25日 星期二 15时51分41秒
 ************************************************************************/

#include "myshell.h"

/*若有&， 则后台执行*/
void back_run(int background){
    int pid;

    if(background == 1){
        if((pid = fork()) < 0){
            printf("fork error\n");
            exit(0);
        }
        else if(pid > 0){
            printf("Process id is %d\n", pid);
            return;
        }
    }
}

void do_cmd(int argcount, char arglist[][256]){
    int flag = 0;
    int how = 0;
    int background = 0;
    int status, i, fd;
    char * arg[argcount + 1];
    char * argnext[argcount + 1];// '|'后面的的命令
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
    else if(how == IN_REDIRECT){
        for(i = 0; arg[i] != NULL; i++){
            if(!strncmp(arg[i], "<", 1)){
                file = arg[i + 1];
                arg[i] = NULL;
            }
        }
    }
    else if(how == HAVE_PIPE){
        for(i = 0; arg[i] != NULL; i++){
            if(!strncmp(arg[i], "|", 1)){
                arg[i] = NULL;
                int j = i+1;
                for( ; arg[j] != NULL; j++ )
                    argnext[j - i - 1] = arg[j];
                argnext[j - i - 1] = NULL;
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
        if(pid == 0){
            if(!find_cmd(arg[0])){
                printf("%s: command not found.\n", arg[0]);
                exit(0);
            }
            
            if(background == 1){
                int pid2;
                if((pid2 = fork()) < 0){
                    printf("fork error\n");
                    exit(0);
                }
                else if(pid2 > 0){
                    printf("Process exited with %d\n", pid2);
                    exit(0);
                }    

                sleep(3);
                    
                execvp(arg[0], arg);
            }
            exit(0);
        }
        if(waitpid(pid, NULL, 0) < 0){
            perror("waitpid error");
            exit(1);
        }
        
        exit(0);
        
        break;

        case 1:
        if(pid == 0){
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
        if(pid == 0){
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
        
        case 3:
        if(pid == 0){
            int pid2, status2, fd2;

            /*创建子进程查找执行"|" 前面的命令*/
            if((pid2 = fork()) < 0){
                printf("fork2 failed\n");
                exit(0);
            }
            else if(pid2 == 0){
                if(!find_cmd(arg[0])){
                    printf("%s: command not found\n", arg[0]);
                    exit(0);
                }
                if((fd2 = open("/tmp/iknow", O_CREAT|O_RDONLY|O_TRUNC, 0644)) < 0){
                    printf("file creation failed\n");
                    exit(0);
                }
                dup2(fd2, 1);
                execvp(arg[0], arg);
                exit(0);
            }
            
            /*退出子进程*/
            if(waitpid(pid2, &status, 0) < 0){
                printf("wait for child process error\n");
                exit(0);
            }

            if(!find_cmd(argnext[0])){
                printf("%s: command not found\n", argnext[0]);
                exit(0);
            }

            if((fd2 = open("/tmp/iknow", O_RDONLY)) < 0){
                printf("open temp file error\n");
                exit(0);
            }

            dup2(fd2, 0);
            execvp(argnext[0], argnext);

            if(remove("/tmp/iknow") < 0)
                printf("remove temp file error\n");
            exit(0);
        }
        break;
        
        default:
            break;
    }

   
    /*等待子进程退出*/
    if(waitpid(pid, &status, 0) < 0 ){
        printf("wait for child process error\n");
    }
}
