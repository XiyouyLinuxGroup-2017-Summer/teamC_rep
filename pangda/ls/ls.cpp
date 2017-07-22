#include<pangda/ls.h>

int main(int argc, char *argv[]) {
    param_t param = set_param(argc, argv);         //分析带有的参数
    if (param.path[param.path.size() - 1] != '/')
        param.path += '/';  //若要显示的目录不以/结尾，那么手动在之后加上这个属性
    auto fli = build_filelist(param);   //根据参数构建文件列表
    output(param, fli);     //按指定参数形式输出文件列表
    return 0;
}
