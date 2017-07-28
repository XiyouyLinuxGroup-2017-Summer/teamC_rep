/*************************************************************************
	> File Name: main.c
	> Author: niliushall
    > Email: sdwllinux@gmail.com
	> Created Time: 2017年07月25日 星期二 14时51分03秒
 ************************************************************************/

#include "myshell.h"

void print_prompt(void){
    char buf[256];
    printf("\033[1;32mmyshell\033[0m:\033[1;34m%s\033[0m$ ", getcwd(buf, 256));
}

/*
void get_input(char *line){
    int len = 0, k = 0;
    //char ch, *line;mZ    //char **matches = (char **)NULL;
    //rl_compentry_func_t * command_generator;
    //rl_bind_key ( '\t' , rl_complete );

    line = readline("");
    if(*line)
        add_history(line);

    while((ch = getchar()) != '\n' && len < 256){
        buf[len++] = ch;
        if(ch != ' ')
            k = 1;
    }
    buf[len] = 0;
    
        if(ch == ' '){
            k = 0;
        }
        else if(ch == '\t')
        {
            tmp[k] = 0;
            matches = rl_completion_matches(tmp, command_generator);
            printf("%s", tmp);
        }
        else{
            buf[len++] = ch;
            tmp[k++] = 0;
        }
    }

    len = strlen(line);
    if(len > 255){
        printf("Command is too long.\n");
        exit(1);
    }
}
*/
int main (int argc, char **argv)
{
    int i, len, count_t = 0;
    int argcount = 0;//待执行命令的参数个数
    char arglist[100][256];
    char **arg = NULL;
    char *buf = NULL;//存放输入命令
    char history[1000][256];
    char *line = NULL;
    char start[300] = "\033[1;32mmyshell\033[0m:\033[1;34m";
    char t[256] = {0};

    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGHUP, SIG_IGN);
    signal(SIGINT, SIG_IGN); //ctrl+c


    buf = (char *)malloc(256);
    if(buf == NULL){
        perror("malloc failed");
        exit(1);
    }

    while(1){
        memset(buf, 0, 256);//将buf清零  
        memset(arglist, 0, 25600);  
        memset(start, 0, 300);
        strcpy(start, "\033[1;32mmyshell\033[0m:\033[1;34m");
        
        //print_prompt();
//printf("111\n");
        //get-input
        strcat(start, getcwd(t, 256));
//printf("222\n");
        strcat(start, "\033[0m$ ");
//printf("333\n");
        line = readline(start);
//printf("444\n");
        if(*line)
            add_history(line);
        strncpy(buf, line, 256);
        strncpy(history[count_t++], line, 256);
        free(line);

        len = strlen(line);
        if(len > 255){
            printf("Command is too long.\n");
            exit(1);
        }

        if(!strcmp(buf, "exit") || !strcmp(buf, "logout"))
            break;

        argcount = explain_input(buf, arglist);

        do_cmd(argcount, arglist, count_t, history);
    }

    if(buf != NULL){
        free(buf);
        buf = NULL;
    }

    return 0;
}
