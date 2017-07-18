#include<pangda/ls.h>

int main(int argc, char *argv[]) {
    param_t param = set_param(argc, argv);
    auto fli = build_filelist(param);
    output(param, fli);
    return 0;
}
