/*************************************************************************
	> File Name: myshell.c
	> Author: niliushall
    > Email: sdwllinux@gmail.com
	> Created Time: 2017年07月25日 星期二 14时29分47秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <dirent.h>
#include <readline/readline.h>
#include <readline/history.h>

#define NORMAL        0//一般命令
#define OUT_REDIRECT  1//输出重定向
#define IN_REDIRECT   2//输入重定向
#define HAVE_PIPE     3//管道命令

void print_prompt(void);//打印提示符
void get_input(char *);//输入
int explain_input(char *, char a[][256]);//解析输入命令
void do_cmd(int, char a[][256]);//执行命令
int find_cmd(char *);//查找可执行程序
