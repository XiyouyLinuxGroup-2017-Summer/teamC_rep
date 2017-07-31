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

//得到输入，统计历史命令个数并存储
void get_input(char *buf,int *historycount,char history[100][256]) {
    int count = 0;
    int i;
    int len;
    char  *temp;
    char *msg;
    char str[256];

    msg = (char *)malloc(200);
    getcwd(msg,100);
    temp = (char* )malloc(sizeof(char) * 256);  
    memset(temp,0,sizeof(temp));  
    sprintf(str,"myshell$$ \033[;34m%s/\033[0m",msg);
    //简单处理界面，获得当前目录

    temp = readline(str);        
    strcpy(buf, temp);         
    //readline会打印里面的字符串，然后接受所输入的字符串

    len = strlen(buf);           
    for(i = 0;i < len; i++) {
        if(buf[i] == ' '|| buf[i] == '\n') 
            count++;
    }
    //统计空格和换行

    //如果直接空格或换行不存储命令
    if(count != len) {
        if(*historycount == 0){
                strcpy(history[*historycount],buf);
                *historycount = *historycount + 1;
                add_history(buf);
        } else {
            //如果前后两次命令相同，不再存储
            if(strcmp(history[*historycount-1],buf) != 0){
                strcpy(history[*historycount],buf);
                *historycount = *historycount + 1;
                add_history(buf);//将命令存储在历史列表，可以用上下键来进行操作
            }
        }
    }
    
    free(temp);
}

//解释输入,将输入以空格为界线分开，argcount存储命令个数，arglist存储具体命令
void explain_input(char *buf,int *argcount,char arglist[100][256]) {
	int number = 0 ;
	char *p = buf;
	char *q = buf;

	while(1){
		if(p[0] == '\n' || p[0] == 0)  //空格或者换行结束
			break;
		if(p[0] == ' ') //跳过空格
			p++;
		else{
			q = p;
			number = 0;
			while( (q[0] != ' ') && q[0] != 0  && q[0] != '\n'){
				number++; //代表单个命令的长度
				q++;    //移动到命令结尾
			}
			strncpy(arglist[*argcount],p,number+1);   //把单个命令解析出来
			arglist[*argcount][number] = 0;         //最后加上结尾
			*argcount = *argcount + 1;             //移动到下一个字符串 
			p = q;                                   //将p移动到末尾位置
		}
	}
}


//将命令分类，分别执行
void do_cmd(int argcount,char arglist[100][256],int historycount,char history[100][256]) {
	int flag = 0;
	int how = normal;
	int backgroud = not_background;
	int status;
	int i;
	int fd;
	char *arg[argcount + 4];
	char *argnext[argcount + 4];
	char *file;
	pid_t pid;

	for(i = 0;i < argcount;i++)
		arg[i] = (char *)arglist[i];    

	arg[argcount] = NULL;	

    //解析是否是后台运行
	for(i=0;i < argcount;i++){
		if(strncmp(arg[i],"&",1) == 0){
			if(i == argcount - 1){
				backgroud = 1;
				arg[argcount - 1] =NULL;
				break;
			} else {
				printf("wrong command\n");
				return ;
			}
		}
	}

	
	for(i=0 ;arg[i] != NULL; i++){
		//解析重定向输入
        if(strcmp(arg[i],">") == 0){
			flag++;
			how = out_redirect;
			if (arg[i+1] == NULL)
				flag++;
		}
        //解析追加重定向输入
        if(strcmp(arg[i],">>") == 0) {
            flag++;
            how = double_out_redirect;
            if (arg[i+1] == NULL) {
                flag++;
            }
        }
        //解析重定向输出
		if( strcmp(arg[i],"<") == 0){
			flag++;
			how = in_redirect;
			if(i == 0)
				flag++;
		}
        //解析管道命令
		if( strcmp(arg[i],"|") == 0){
			flag++;
			how = have_pipe;
			if(arg[i+1] == NULL)
				flag++;
			if( i == 0)
				flag++;
		}
	}

    //只允许单个 > < >> |命令
	if(flag > 1){
		printf("wrong command !\n");
		return ;
	}

	if( how == out_redirect){
		for(i = 0;arg[i] != NULL;i++){
			if(strcmp(arg[i],">") == 0){
				file = arg[i+1];
				arg[i] = NULL;
			}
		}
	}

    if(how == double_out_redirect) {
        for(i = 0;arg[i] != NULL;i++ ) {
            if(strcmp(arg[i],">>") == 0) {
                file = arg[i+1];
                arg[i] = NULL;
            }
        } 
    }

	if(how == in_redirect){
		for(i=0; arg[i] != NULL;i++){
			if(strcmp(arg[i],"<") == 0){
				file = arg[i+1];
				arg[i] = NULL;
			}
		}
	}

	if(how == have_pipe){
		for( i = 0; arg[i] != NULL ;i++){
			if(strcmp(arg[i],"|") == 0){
				arg[i] = NULL;
				
				int j;
				for(j = i+1;arg[j] != NULL;j++){
					argnext[j-i-1] = arg[j];
				}
				argnext[j-i-1] = arg[j];

				break;
			}
		}
	}

  // 如果命令为history 则输出历史。
    if(strcmp(arg[0],"history") == 0&& argcount == 1){
        for(i = 0;i < historycount;i++){
            printf("%d\t %s\n",i+1,history[i]);
        }
        return;
    }


 //完成内嵌cd命令
    if(strcmp(arg[0],"cd") == 0 && argcount == 2){
        //处理特殊的cd或cd ~命令
        if(argcount == 1 || argcount == 2 && strcmp(arg[1],"~") == 0) {
		    chdir("/home/yinjianxiang");
		    return;
        }
        if(chdir(arg[1]) == -1){
            perror("chdir");
        }
	    return;
    }

    //fork一个子进程，执行参数
	if( (pid = fork()) < 0){
		printf("fork error\n");
		return ;
	}
	
	switch(how){
	    case normal:
		    if(pid == 0){
			    if( !(find_command(arg[0])) ){
				    printf("%s :command not found !\n",arg[0]);
				    exit(0);
			    }
			    execvp(arg[0],arg);  //加载参数命令
			    exit(0);              
		    }
		    break;
		// >
	    case out_redirect:
		    if(pid == 0){
			    if( !(find_command(arg[0])) ){
				    printf("%s :command not found !\n",arg[0]);
				    exit(0);
			    }
			    fd = open(file,O_RDWR | O_CREAT | O_TRUNC,0644);//
                dup2(fd,1);

			    execvp(arg[0],arg);//加载参数命令
			    exit(0);
		    }
		    break;
        // >
	    case double_out_redirect:
		    if(pid == 0){
			    if( !(find_command(arg[0])) ){
				    printf("%s :command not found !\n",arg[0]);
				    exit(0);
			    }
			    fd = open(file,O_RDWR | O_CREAT | O_TRUNC |O_APPEND,0644);//打开文件
                dup2(fd,1);                 //把输出从屏幕交给了文件
			    execvp(arg[0],arg);
                close(fd);
			    exit(0);
		    }
		    break;
	    
        // <
        case in_redirect:
		    if(pid == 0){
			        if( !(find_command(arg[0])) ){
				    printf("%s :command not found !\n",arg[0]);
				    exit(0);
			    }
			    fd = open(file,O_RDONLY); //打开文件
                dup2(fd,0);             //将输入从键盘交给了文件
			    execvp(arg[0],arg);         
                close(fd);  
			    exit(0);    
		    }
		    break;
	    
        // |
        case have_pipe:
		    if(pid == 0){
			    int pid2;
			    int status2;
			    int fd2;

                //fork执行|前面的命令
			    if( (pid2 = fork()) < 0){
				    printf("fork2 error\n");
				    return ;
			    }

			    else if(pid2 == 0){
				    if(!(find_command)(arg[0])){
					    printf("%s :command not found !\n",arg[0]);
					    exit(0);
				    }
                
                    //将屏幕的输出流入文件之中
                    if((fd2 = open("/tmp/tempfile",O_WRONLY | O_CREAT | O_TRUNC ,0644)) == -1) {
                        printf("open /tmp/tempfile failed!");
                    }
				    dup2(fd2,1);
                    execvp(arg[0],arg);
                    close(fd2);
                    exit(0);
			    }

			    //等待管道前面的命令完成
                if(waitpid(pid2,&status2,0) == -1){
				    printf("wait for child process error\n");
			    }

                //执行管道后面的命令
                if(!(find_command(argnext[0]))) {
					printf("%s :command not found !\n",argnext[0]);
					exit(0);
			    }
            
                if((fd2 = open("/tmp/tempfile",O_RDONLY)) == -1) {
                    perror("open");
                }
                //用文件代替键盘，执行第二条命令
			    dup2(fd2,0);

			    execvp(argnext[0],argnext);
			    if(remove("/tmp/tempfile") == -1) {//移除文件
				    perror("remove");
                }
			    exit(0);
		    }
		    break;

	    default :
		    break;
	}
	
    //杀死父进程,后台进行
    if(backgroud == 1 && pid != 0){
        printf("[process id %d]\n",pid);
        return;
    }

    //父进程等待子进程
    if(waitpid(pid,&status,0) == -1){
        printf("wait for child process error!\n");
    }


}


static int find_command(char *command) {
    DIR *dp;
    struct dirent *dirp;
    char *path[] = {"./","/bin","/usr/bin",NULL};

    //使当前目录下可执行文件能够执行
    if(strncmp(command,"./",2) == 0) {
        command = command + 2;
    }
    

    int i = 0;
    //在当前目录，/bin和/usr/bin中执行
    while(path[i] != NULL) {
        if( (dp = opendir(path[i])) == NULL) {//打开目录
            printf("can not open /bin \n");
        }
        while( (dirp = readdir(dp)) != NULL)  { //读取目录中的文件
            if(strcmp(dirp->d_name,command) == 0) {//找到
                closedir(dp);  
                return 1;    //返回1
            }
        }
        closedir(dp);
        i++;  //去找下一个目录
    }
    return 0; //没找到
}

#endif
