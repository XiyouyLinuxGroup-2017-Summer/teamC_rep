#include<unistd.h>
#include<string.h>

int main(int argc, char *argv[]) {
    for (int i = 0; i <= argc - 2; i++) {
        if (i)
            write(STDOUT_FILENO, " ", 1);
        write(STDOUT_FILENO, argv[i + 1], strlen(argv[i + 1]));
    }
    return 0;
}