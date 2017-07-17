#include<sys/stat.h>
#include<unistd.h>
#include<stdio.h>

int main() {
    struct stat buf;
    if (stat("test.dat", &buf) == -1) {
        puts("Error.");
        return -1;
    }

    printf("device:%d\n", buf.st_dev);
    printf("inode:%d\n", buf.st_ino);
    printf("blocksize:%d\n", buf.st_blksize);
    printf("gid:%d", buf.st_gid);
    return 0;
}