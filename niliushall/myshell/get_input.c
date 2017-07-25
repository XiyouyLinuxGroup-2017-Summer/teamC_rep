/*************************************************************************
	> File Name: get_input.c
	> Author: niliushall
    > Email: sdwllinux@gmail.com
	> Created Time: 2017年07月25日 星期二 15时12分50秒
 ************************************************************************/

#include "myshell.h"

/*获取用户输入*/
void get_input(char *buf){
    int len = 0, k = 0;
    char ch, tmp[100];
    char **matches = (char **)NULL;
    rl_compentry_func_t * command_generator;

    while((ch = getchar()) != '\n' && len < 256){
        buf[len++] = ch;
    }
        /*if(ch == ' '){
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
    }*/

    //add_history(buf);

    if(len > 255){
        printf("Command is too long.\n");
        exit(1);
    }

}
