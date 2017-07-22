/*************************************************************************
> File Name: my_own_ls.c
> Author: YinJianxiang
> Mail: YinJianxiang123@gmail.com 
> Created Time: 2017年07月19日 星期三 15时09分16秒
************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<pwd.h>
#include<linux/limits.h>
#include<grp.h>
#include<errno.h>
#include<dirent.h>
#include<time.h>

#define PARAM_NONE 0  //无参数
#define PARAM_A 1     //a
#define PARAM_L 2     //l
#define MAXROWLEN 80  //每行最多显示字符

int g_leave_len = MAXROWLEN;  //每行剩余长度
int g_maxlen;                 //每行最大的文件长度

/*简单错误处理*/
void my_err(const char *err_string,int line) {
    fprintf(stderr,"line:%d ",line);
    perror(err_string);
    exit(0);
}

/*获取文件属性*/
void display_attribute(struct stat buf,char *name) {
    char buf_time[32];
    struct passwd *psd;     //获取文件个人属性结构体
    struct group *grp;      //获取文件组属性的结构体

    /*打印文件类型*/
    if(S_ISLNK(buf.st_mode)) {              //符号链接
        printf("l");
    } else if(S_ISREG(buf.st_mode)) {       //一般文件
        printf("-");
    } else if(S_ISDIR(buf.st_mode)) {       //目录文件
        printf("d");
    } else if(S_ISCHR(buf.st_mode)) {       //字符设备文件  
        printf("c");
    } else if(S_ISBLK(buf.st_mode)) {       //块设备文件
        printf("b");
    } else if(S_ISFIFO(buf.st_mode)) {      //先进先出文件
        printf("f");
    } else if(S_ISSOCK(buf.st_mode)) {      //socket
        printf("s");
    }

    /*打印用户权限*/
    if(buf.st_mode & S_IRUSR) {
        printf("r");
    } else {
        printf("-");
    }
    if(buf.st_mode & S_IWUSR) {
        printf("w");
    } else {
        printf("-");
    }
    if(buf.st_mode & S_IXUSR) {
        printf("x");
    } else {
        printf("-");
    }

    /*打印所属组的权限*/
    if(buf.st_mode & S_IRGRP) {
        printf("r");
    } else {
        printf("-");
    }
    if(buf.st_mode & S_IWGRP) {
        printf("w");
    } else {
        printf("-");
    }
    if(buf.st_mode & S_IXGRP) {
        printf("x");
    } else {
        printf("-");
    }

    /*打印其他人的权限*/
    if(buf.st_mode & S_IROTH) {
        printf("r");
    } else {
        printf("-");
    }
    if(buf.st_mode & S_IWOTH) {
        printf("w");
    } else {
        printf("-");
    }
    if(buf.st_mode & S_IXOTH) {
        printf("x");
    } else {
        printf("-");
    }

    printf("   ");


    psd = getpwuid(buf.st_uid);     //获取个人信息结构体
    grp = getgrgid(buf.st_gid);     //获取组信息结构体
    
    printf("%4ld ",buf.st_nlink);	//打印链接数
    printf("%-8s",psd->pw_name);    //打印用户姓名
    printf("%-8s",grp->gr_name);    //打印组姓名

    printf("%6ld",buf.st_size);     //打印文件大小
    strcpy(buf_time,ctime(&buf.st_mtime));  
    buf_time[strlen(buf_time) - 1] = 0;     
    printf("	%s",buf_time);              //打印文件时间信息
}

/*没有-l参数，打印一个文件名，上下对齐*/
void display_single(char *name) {
    int i;                  
    int len;                

    /*剩余的空间不够，则换下一行*/
    if(g_leave_len < g_maxlen) {        
        printf("\n");                   
        g_leave_len = MAXROWLEN;                    
    }

    len = strlen(name);             
    len = g_maxlen - len;       //统计空格个数        
    printf("%-s",name);

    for(i = 0;i < len;i++) {                
        printf(" ");                
    }
    printf(" ");

    g_leave_len -= (g_maxlen + 2); //每个文件名后空2个格
}

/*flag为ls参数，pathname为文件路径*/
void display(int flag,char *pathname) {
    int i;          
    int j;          
    struct stat buf; 
    char name[MAXROWLEN + 1];       

    for(i = 0,j = 0;i < strlen(pathname);i++) {         
        if(pathname[i] == '/') {   //每次到‘/’就去获取下一个文件的名字     
            j = 0;          
            continue;           
        }                           
        name[j++] = pathname[i];            
    }                               
    name[j] = 0;                        

    /*lstat返回连接符本身的信息 stat返回连接符所指向文件的信息*/
    if(lstat(pathname,&buf) == -1) { //获取文件信息
        my_err("stat",__LINE__); 
    }

    switch(flag) {
        case PARAM_NONE: //无参数
            if(name[0] != '.') {
                display_single(name);	
            }
            break;

        case PARAM_A:    //-a
            display_single(name);
            break;

        case PARAM_L:    //-l
            if(name[0] != '.') {
                display_attribute(buf,name);
                printf("%-s\n",name);
            }
            break;

        case PARAM_A + PARAM_L:     //-la 
            display_attribute(buf,name);
            printf("  %-s\n",name);
            break;

        default:
            break;
    }
}


void display_dir(int flag_param,char *path) {
    DIR *dir;
    struct dirent *ptr;
    int count = 0;
    char filename[256][PATH_MAX];
    char temp[PATH_MAX];
    
    dir = opendir(path);    //打开文件

    if(dir == NULL) {       
        my_err("opendir",__LINE__);     
    }                       
    while((ptr = readdir(dir)) != NULL) {           
        if(g_maxlen < strlen(ptr->d_name)) {   //获取目录下最长文件名                 
            g_maxlen = strlen(ptr->d_name);                 
        }
        count++;        //统计目录下文件个数
    }
    closedir(dir);       

    if(count > 256) {
        my_err("too many files under this dir",__LINE__);
    }
    /*限定了文件的最大*/
    int i;
    int j;
    int len = strlen(path);

    dir = opendir(path);

    /*获取所有文件的名字信息*/
    for(i = 0;i < count;i++) {
        ptr = readdir(dir);  //读取目录，返回目录结构体信息

        if(ptr == NULL) {
            my_err("readdir",__LINE__);
        }
        strncpy(filename[i],path,len);          
        filename[i][len] = 0;           
        strcat(filename[i],ptr->d_name);            
        filename[i][len+strlen(ptr->d_name)] = 0;       
    }
    
    /*冒泡排序排文件名*/
    for(i = 0;i < count - 1;i++) {
        for(j = 0;j < count-1-i;j++) {
            if(strcmp(filename[j],filename[j+1]) > 0) {
                strcpy(temp,filename[j+1]);
                temp[strlen(filename[j+1])] = 0;
                strcmp(filename[j+1],filename[j]);
                filename[j+1][strlen(filename[j])] = 0;
                strcmp(filename[j],temp);
                filename[j][strlen(temp)] = 0;
            }
        }
    }
    
    for(i = 0;i < count;i++) {
        display(flag_param,filename[i]);
    }
        
    if(flag_param & PARAM_L){  //参数没有-l时，换行
        printf("\n");
    }
}

/*相当于打印文件信息的函数，打印目录的函数相当于不停调用*/

int main(int argc,char *argv[]) {
    int i;          
    int j;      
    int k;      
    int num;                    
    char path[PATH_MAX];        //路径
    char param[32];             //命令行参数
    int flag_param = PARAM_NONE;//参数
    struct stat buf;            //文件信息结构体

    j = 0;
    num = 0;
    /*解析参数*/
    for(i = 1;i < argc;i++) { 
        if(argv[i][0] == '-') { 
            for(k = 1;k < strlen(argv[i]);k++,j++) { 
                param[j] = argv[i][k];       
            }       
        num++;   //-的个数       
        }  
    }   

    for(i = 0;i < j;i++) {      
        if(param[i] == 'a') {  //参数为a          
            flag_param |= PARAM_A;                
        }                           
        else if(param[i] == 'l') {  //参数为l        
            flag_param |= PARAM_L;              
        } else {                                            
            printf("my ls:invalid option -%c\n",param[i]);                  
            exit(1);            
        }           
    }

    param[j] = 0;               
        
    /*没有输入文件名，则显示当前目录*/
    if((num + 1) == argc) {                                         
        strcpy(path,"./");                                      
        path[2] = 0;                                        
        display_dir(flag_param,path);                           
        return 0;
    }

    i = 1;           

    do {                                  
        if(argv[i][0] == '-') {     //如果不是文件名或目录，解析下一个命令行参数
            i++;         
            continue;       
        } else {        
            strcpy(path,argv[i]);       
             
            if(stat(path,&buf) == -1) {
                my_err("stat",__LINE__);        //获取文件的信息结构体
            }
            
            if(S_ISDIR(buf.st_mode)) {   //如果是一个目录
                if(path[strlen(argv[i])-1] != '/') {
                    //最后没有/时加上/
                    path[strlen(argv[i])] = '/';    
                    path[strlen(argv[i])+1] = 0;    
                } else {
                    path[strlen(argv[i])] = 0;
                }
                display_dir(flag_param,path);
                i++;
            } else { //文件
                display(flag_param,path);
                i++;
            }
        }
    } while(i < argc);
    
    return 0;
}



