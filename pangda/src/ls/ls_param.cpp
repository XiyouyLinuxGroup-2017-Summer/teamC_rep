#include<pangda/ls.h>
#include<cstring>

static const char *PARAMSTR_a = "all";
static const char *PARAMSTR_A = "almost-all";

static int analyse_param(const char *args) {
    if (args[0] != '-')
        return -1;
    if (args[1] == '-') {
        if (strcmp(args + 2, PARAMSTR_a) == 0)
            return PARAM_a;
        if (strcmp(args + 2, PARAMSTR_A) == 0)
            return PARAM_A;
        return 0;
    }

    int ret = 0;
    while (*args++) {
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

bool check_param(const param_t msg, const int param) {
    return (msg.param_flag & param);
}

param_t set_param(int argc, char *argv[]) {
    param_t ret = { "./", 0 };

    for (int i = 1; i < argc; i++) {
        int t = analyse_param(argv[i]);
        if (t == -1)
            ret.path = argv[i];
        else
            ret.param_flag |= t;
    }
    return ret;
}