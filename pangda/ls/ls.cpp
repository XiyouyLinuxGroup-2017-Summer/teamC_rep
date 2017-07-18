#include<pangda/ls.h>

int main(int argc, char *argv[]) {
    param_t param = set_param(argc, argv);
    if (param.path[param.path.size() - 1] != '/')
        param.path += '/';
    auto fli = build_filelist(param);
    output(param, fli);
    return 0;
}
