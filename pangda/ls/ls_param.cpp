#include<pangda/ls.h>
#include<cstring>

static const char *PARAMSTR_a = "all";
static const char *PARAMSTR_A = "almost-all";

//分析并返回是否具有参数，返回值是参数做|运算后的数值
static int analyse_param(const char *args) {
    if (args[0] != '-')     //若参数不以-开头，说明他不是一个参数
        return -1;
    if (args[1] == '-') {   //若以--开头，说明需要以字符串来判断
        if (strcmp(args + 2, PARAMSTR_a) == 0)
            return PARAM_a;
        if (strcmp(args + 2, PARAMSTR_A) == 0)
            return PARAM_A;
        return 0;
    }

    int ret = 0;
    while (*args++) {   //依次判断，因为可能出现-alRf的形式
        switch (*args) {
        case 'a':
            ret |= PARAM_a;
            break;
        case 'A':
            ret |= PARAM_A;
            break;
        case 'f':
            ret |= PARAM_f;
            break;
        case 'r':
            ret |= PARAM_r;
            break;
        case 'R':
            ret |= PARAM_R;
            break;
        case 'l':
            ret |= PARAM_l;
        }
    }
    return ret;
}

//检查msg是否具有param所指定的参数
bool check_param(const param_t msg, const int param) {
    return (msg.param_flag & param);
}

//设置参数
param_t set_param(int argc, char *argv[]) {
    param_t ret = { "./", 0 };  //若没有默认路径，则默认以./作为路径

    for (int i = 1; i < argc; i++) {
        int t = analyse_param(argv[i]);
        if (t == -1)
            ret.path = argv[i];     //若返回值是-1，说明这个参数是要显示的路径
        else
            ret.param_flag |= t;    //反之则说明应当设定参数
    }
    return ret;
}