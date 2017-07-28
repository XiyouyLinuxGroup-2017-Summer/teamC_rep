/*************************************************************************
	> File Name: ls_new.c
	> Author: wangliang
    > Email: sdwllinux@gmail.com
	> Created Time: 2017年07月20日 星期四 16时24分18秒
 ************************************************************************/

#include <stdio.h>
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

#define PARAM_NONE 0    //无参数
#define PARAM_A 1       // -a
#define PARAM_L 2       // -l
#define PARAM_R 4       // -R
#define PARAM_r 8       // -r
#define MAXROWLEN 160    //一行显示的最多字符数

int len_leave = MAXROWLEN;//该行剩余长度
int len_max;//最长文件名的长度

typedef struct dirname{
    char name[NAME_MAX + 1];
    struct dirname *next;
}DIRNAME;

void err(const char *err_string, int line){
    fprintf(stderr, "Line %d: ", line);
    perror(err_string);
    exit(1);
}

void sort_dir(DIRNAME *pHead){
    DIRNAME *p, *f = pHead, *x, *y;
    //判断是否只有一个元素或者没有元素
    if(pHead->next == NULL || pHead -> next -> next == NULL)
    {
        return;
    }
    while(f != pHead->next->next)
    {
        //外层是N - 1次循环,升序
        for(p = pHead ; p -> next -> next != f; p = p -> next)
        {
            if(strcmp(p -> next -> name, p -> next -> next -> name) > 0)
            {
                x = p -> next;
                y = p -> next -> next;
                p -> next = y;
                x -> next = y -> next;
                y -> next = x;
            }
        }
        f = p -> next;
    }
}

void display(int param_mode, DIRNAME *pHead){
    DIRNAME *p = pHead->next;
    int i, j;
    struct stat buf;
    char name[NAME_MAX + 1];

    for(i = 0; i < strlen(); i++)
    
}

void display_dir(int param_mode, char *path, DIRNAME *pEnd){
    DIR *dir;
    struct dirent *ptr;
    char temp[NAME_MAX+1];
    DIRNAME *pHead = NULL, *pNew, *pEnd = pHead, *p;
    
    if((dir = opendir(path)) < 0){
        err("opendir", __LINE__);
    }

    //获取最长文件名长度并存储文件名
    while((ptr = readdir(dir)) != NULL){
        if(len_max < strlen(ptr->d_name)){
            len_max = strlen(ptr->d_name);
        }
        pNew = (DIRNAME *)malloc(sizeof(DIRNAME));
        strcpy(pNew->name, path);
        pNew->name[strlen(path)] = 0;
        strcat(pNew->name, ptr->d_name);
        pNew->name[strlen(path) + strlen(ptr->d_name)] = 0;
        pEnd->next = pNew;
    }
    closedir(dir);
    
    //文件名排序
    sort_dir(pHead);

    p = pHead->next;
    while(p != NULL){
        display(param_mode, p->name, pEnd);
        p = p->next;
    }
    close(dir);

    if(param_mode & PARAM_L == 0)
        printf("\n");
}

int main (int argc, char **argv)
{
    DIRNAME *pHead = NULL, *pNew;
    int i, j, k = 0, sum = 0;
    int param_mode = PARAM_NONE;
    char param[32];
    char path[PATH_MAX+1];
    struct stat buf;

    for(i = 1;i < argc; i++){
        if(argv[i][0] == '-'){
            for(j = 1; j < strlen(argv[i]); j++){
                    param[k++] = argv[i][j]; 
            }
            sum++;
        }
    }
                
    for(i = 0; i < k; i++){
        if(argv[i][j] = 'a'){
            param_mode |= PARAM_A;
        }
        else if(argv[i][j] == 'l'){
            param_mode |= PARAM_L;
        }
        else if(argv[i][j] == 'R'){
            param_mode |= PARAM_R;
        }
        else{
            printf("invalid option -%c\n", param[i]);
            exit(1);
        }
    }
    param[k] = 0;

    //没有文件名，显示当前目录
    if(sum + 1 == argc){
        strcpy(path, "./");
        path[2] = 0;
        diaplay_dir(param_mode, path);
        return 0;
    }

    return 0;
}
