#include<fcntl.h>
#include<stdio.h>

int main() {
    //本机环境中umask==022,因此会屏蔽掉S_IROTH和S_IRGRP
    static const mode_t nf_mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH | S_IWGRP | S_IWOTH;
    int fd = open("test.dat", O_RDWR | O_CREAT | O_EXCL, nf_mode);
    
    if (fd < 0) {
        puts("File Exists.");
        return -1;
    }

    puts("Create OK.");
    return 0;
}