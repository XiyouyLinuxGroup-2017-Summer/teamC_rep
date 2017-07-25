/*************************************************************************
> File Name: show_file.c
> Author: wangliang
> Email: sdwllinux@gmail.com
> Created Time: 2017年07月18日 星期二 14时45分17秒
************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>

int print_file(const char *path)
{
    DIR *dir;
    struct dirent * ptr;

    if((dir = opendir(path)) == NULL){
        perror("opendir");
        return -1;
    }

        while((ptr = readdir(dir)) != NULL){
            printf("filename: %s\n", ptr->d_name);
    }
    closedir(dir);
    return 0;
}

int main (int argc, char **argv)
{
    if(argc != 2){
        perror("num error\n");
        exit(1);
    }

    if(print_file(argv[1]) < 0){
        printf("errorr\n");
        exit(1);
    }

    return 0;
}
