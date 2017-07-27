/*************************************************************************
	> File Name: myhead.h
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年07月26日 星期三 11时10分12秒
 ************************************************************************/

#ifndef _MYHEAD_H
#define _MYHEAD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <dirent.h>

#define normal          0
#define out_redirect    1 
#define in_redirect     2
#define have_pipe       3

#define is_background   1
#define not_background  0                  

void print_prompt();                    //打印开始时的所在路径            
void get_input();                       //获得用户输出        
void explain_input(char *buf,int *argcount,char arglist[100][256]);     //接受输入
void do_cmd(int argcount,char arglist[100][256]);     //执行命令

#endif
