#include<pangda/psh.h>

int psh_error(int error) {
    switch (error) {
    //fixme： add error
    }
    return -1;
}

int shellfunc_exit() {
    exit(0);
    return 0;
}

int shellfunc_logout() {
    shellfunc_exit();
    return 0;
}