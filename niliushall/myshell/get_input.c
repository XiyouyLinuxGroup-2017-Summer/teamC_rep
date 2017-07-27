/*************************************************************************
	> File Name: get_input.c
	> Author: niliushall
    > Email: sdwllinux@gmail.com
	> Created Time: 2017年07月25日 星期二 15时12分50秒
 ************************************************************************/

#include "myshell.h"

/*获取用户输入*/
void get_input(char *line){
    int len = 0, k = 0;
    //char ch, *line;
    //char **matches = (char **)NULL;
    //rl_compentry_func_t * command_generator;
    //rl_bind_key ( '\t' , rl_complete );

    line = readline("");
    if(*line)
        add_history(line);

    /*while((ch = getchar()) != '\n' && len < 256){
        buf[len++] = ch;
        if(ch != ' ')
            k = 1;
    }
    buf[len] = 0;
    */
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

    len = strlen(line);
    if(len > 255){
        printf("Command is too long.\n");
        exit(1);
    }
}
