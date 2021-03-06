/*************************************************************************
	> File Name: my_ls.c
	> Author: wangliang
    > Email: sdwllinux@gmail.com
	> Created Time: 2017年07月19日 星期三 08时10分00秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <linux/limits.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <errno.h>

#define PARAM_NONE 0    //无参数
#define PARAM_A 1       // -a
#define PARAM_L 2       // -l
#define PARAM_R 4       // -R
#define MAXROWLEN 80    //一行显示的最多字符数

int len_leave = MAXROWLEN;//该行剩余长度
int len_max;//最长文件名的长度

void err(const char *err_string, int line){
    fprintf(stderr, "line: %d  ", line);
    perror(err_string);
    exit(1);
}

/*获取并打印文件属性*/
void display_attribute(struct stat buf, char *name){
    char buf_time[32];
    struct passwd *psd;//结构体中获取文件所有者的用户名
    struct group *grp;//结构体中获取用户组名
    
    /*文件类型*/
    if(S_ISLNK(buf.st_mode))//符号链接文件
        printf("l");
    else if(S_ISREG(buf.st_mode))//一般文件
        printf("-");
    else if(S_ISDIR(buf.st_mode))//目录文件
        printf("d");
    else if(S_ISCHR(buf.st_mode))//设备文件
        printf("c");
    else if(S_ISBLK(buf.st_mode))//接口设备文件
        printf("b");
    else if(S_ISFIFO(buf.st_mode))//FIFO文件
        printf("f");
    else if(S_ISSOCK(buf.st_mode))//socket文件
        printf("s");

    /*文件所有者权限*/
    if(buf.st_mode & S_IRUSR)
        printf("r");
    else if(buf.st_mode & S_IWUSR)
        printf("w");
    else if(buf.st_mode & S_IXUSR)
        printf("w");
    else 
        printf("-");

    /*文件所属用户组权限*/
    if(buf.st_mode & S_IRGRP)
        printf("r");
    else if(buf.st_mode & S_IWGRP)
        printf("w");
    else if(buf.st_mode & S_IXGRP)
        printf("x");
    else 
        printf("-");

    /*其他用户权限*/
    if(buf.st_mode & S_IROTH)
        printf("r");
    else if(buf.st_mode & S_IWOTH)
        printf("w");
    else if(buf.st_mode & S_IXOTH)
        printf("x");
    else 
        printf("-");

    printf("  ");

    printf("%4ld ", buf.st_nlink);//文件连接数

    /*根据uid与gid获取用户所有者与用户组名称*/
    psd = getpwuid(buf.st_uid);
    grp = getgrgid(buf.st_gid);
    printf("%-8s", psd->pw_name);
    printf("%-8s", grp->gr_name);

    printf("%6ld", buf.st_size);//文件大小
    strcpy(buf_time, ctime(&buf.st_mtime));
    buf_time[strlen(buf_time) - 1] = 0;//去掉换行符
    printf("  %s", buf_time);
}

/*无参数，打印文件名，上下对齐*/
void display_single(char *name){
    int i, len;

    if(len_leave < len_max){
        printf("\n");
        len_leave = len_max;
    }

    printf("%-s", name);
    len = strlen(name);
    len = len_max - len;

    /*输出空格以对齐*/
    for(i = 0; i < len; i++){
        printf(" ");
    }
    printf("  ");
    len_leave -= (len_max + 2);//加2为中间多输出的空格

}

void display(int flag, char *pathname){
    int i, j;
    struct stat buf;
    char name[NAME_MAX + 1];

    /*从路径中解析文件名*/
    for(i = 0, j = 0 ; i < strlen(pathname); i++){
        if(pathname[i] == '/'){
            j = 0;
            continue;
        }
        else{
            name[j++] = pathname[i];
        }
    }
    name[j] = 0;

    /*用lstat以方便解析链接文件*/
    if(lstat(pathname, &buf) == -1){
        err("lstat", __LINE__);
    }

    switch(flag){
        case PARAM_NONE:
            if(name[0] != '.'){
                display_single(name);
            }
            break;

        case PARAM_A:
            display_single(name);
            break;

        case PARAM_L:
            if(name[0] != '.'){
                display_attribute(buf, name);
                printf("  %-s\n", name);            
            }
            break;

        case PARAM_A + PARAM_L:
            display_attribute(buf, name);
            printf("  %-s\n", name);
            break;
    }
}

void display_dir(int flag_param, char *path){
    int i, j, len = strlen(path);
    int count = 0;
    DIR *dir;
    struct dirent *ptr;
    char filename[256][PATH_MAX + 1], temp[PATH_MAX + 1];

    if((dir = opendir(path)) < 0){
        err("open", __LINE__);
    }

    /*获取最长文件名和文件个数*/
    while((ptr = readdir(dir)) != NULL){
        if(len_max < strlen(ptr->d_name))
            len_max = strlen(ptr->d_name);
        count++;
    }
    closedir(dir);

    if(count > 256){
        err("too many files under this dir", __LINE__);
    }

    /*获取该目录下所有文件名*/
    dir = opendir(path);
    for(i = 0; i < count; i++){
        if((ptr = readdir(dir)) == NULL){
            err("readdir", __LINE__);
        }
        strncpy(filename[i], path, len);
        filename[i][len] = 0;
        strcat(filename[i], ptr->d_name);
        filename[i][len + strlen(ptr->d_name)] = 0;
    }

    /*对文件名排序*/
    for(i = 0; i < count - 1; i++){
        for(j = 0; j < count - 1 - i; j++){
            if(strcmp(filename[i], filename[i + 1]) > 0){
                strcpy(temp, filename[j]);
                //temp[strlen(filename[i])] = 0;
                strcpy(filename[j], filename[j + 1]);
                //filename[strlen(filename[i + 1])] = 0;
                strcpy(filename[j + 1], temp);

            }
        }
    }

    for(i = 0; i < count; i++){
        display(flag_param, filename[i]);
    }
    closedir(dir);

    if(flag_param & PARAM_L == 0){
        printf("\n");
    }
}

int main (int argc, char **argv)
{
    int i, j = 0, k, num = 0;
    char path[PATH_MAX + 1];
    char param[32];
    int flag_param = PARAM_NONE;
    struct stat buf;
    
    /*解析命令行参数*/
    for(i = 1; i < argc; i++){
        if(argv[i][0] == '-'){
            for(k = 1; k < strlen(argv[i]); k++){
                param[j++] = argv[i][k];
            }
            num++;
        }
    }

    for(i = 0; i < j; i++){
        if(param[i] == 'a')
            flag_param |= PARAM_A;
        else if(param[i] == 'l')
            flag_param |= PARAM_L;
        else if(param[i] == 'R')
            flag_param |= PARAM_R;
        else{
            printf("my_ls: invalid option -%c\n", param[i]);
            exit(1);
        }
    }
    param[j] = 0;

    if((num + 1) == argc){
        strcpy(path, "./");
        path[2] = 0;
        display_dir(flag_param, path);
        return 0;
    }

    i = 1;
    do{
        if(argv[i][0] == '-'){
            i++;
        }
        else{
            strcpy(path, argv[i]);

            if(stat(path, &buf) == -1){
                err("stat", __LINE__);
            }
            if(S_ISDIR(buf.st_mode)){
                if(path[strlen(argv[i]) - 1] != '/'){
                    path[ strlen(argv[i]) ] = '/';
                    path[ strlen(argv[i]) + 1 ] = 0;
                }
                else
                    display_dir(flag_param, path);
            }
            else{
                display(flag_param, path);
            }
            i++;
        }
    }while(i < argc);

    return 0;
}
