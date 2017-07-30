/*************************************************************************
	> File Name: main.c
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年07月26日 星期三 11时48分54秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "./myhead.h"
#include "./func.h"



int main(int argc, char *argv[]) {
    int i; 
    int argcount = 0;                   
    char arglist[100][256];
    char **arg = NULL;
    char *buf = NULL;
    char  histroy[100][256];
    int his_count;
    char temp[256];

    buf = (char *)malloc(256);
    if(buf == NULL) {
        perror("malloc faid");
        exit(-1);
    }
    
    while(1) {
        signal(SIGINT,SIG_IGN);
        signal(SIGQUIT,SIG_IGN);
        signal(SIGSTOP,SIG_IGN);
        signal(SIGTSTP,SIG_IGN);
        memset(buf,0,256);
        //print_prompt();                 //显示当前路径
        
        get_input(buf,&his_count,histroy);                 //接受命令
        
        /*exit或者logout退出*/
        if(strcmp(buf,"exit") == 0 || strcmp(buf,"logout") == 0) {
            break;                  
        }
        for(i = 0;i < 100;i++) {
            arglist[i][0] = 0;          //初始化命令
        }
        argcount = 0;
         
        explain_input(buf,&argcount,arglist);  //解析命令
        do_cmd(argcount,arglist,his_count,histroy);              //执行命令
    }


    getcwd(temp,100);
    strcmp(last_dir,temp);
    if(buf != NULL) {                          //释放空间
        free(buf);
        buf = NULL;
    }
    
    return 0;
}

