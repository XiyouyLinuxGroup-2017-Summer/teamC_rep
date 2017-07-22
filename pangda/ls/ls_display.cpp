#include<pangda/ls.h>
#include<cstdio>
#include<ctime>
#include<cstring>

//获得终端窗口的大小，用于计算列宽。
static winsize get_winsize() {
    winsize ret;
    ioctl(STDIN_FILENO, TIOCGWINSZ, (char *)&ret);
    return ret;
}

//根据文件的权限属性设定颜色
static int setc(mode_t mode, string filename) {
    if (S_ISDIR(mode))  //若为目录
        return printf("\033[36m");
    if (S_ISLNK(mode))  //若为符号链接
        return printf("\033[33;35m");
    if ((mode & S_IXUSR) || (mode & S_IXOTH) || (mode & S_IXGRP))   //若为可执行文件
        return printf("\033[32m");
    return printf("\033[0m");   //若为其他文件
}

//取消设定的颜色
static int canc() {
    return printf("\033[0m");
}

//test_tier辅助函数
inline static unsigned int find_pos(const int x, const int y, const int tier) {
    return (x + tier * y);
}

//根据宽度表尝试构建宽度表
static list_type test_tier(const int tier, const vint_t widths) {
    vvint_t build;
    list_type ret { int(ceil(widths.size() / double(tier))),
                    tier, 0, vint_t() };

    for (int i = 0; i < tier; i++) {
        build.push_back(vint_t());
        int col_max = 0;
        for (int j = 0; j < ret.rows; j++) {
            if (find_pos(i, j, tier) >= widths.size()) {
                break;
            }
            int t = widths[find_pos(i, j, tier)];

            build[i].push_back(t);
            if (t > col_max)
                col_max = t;
        }

        ret.widths_list.push_back(col_max);
        ret.judge += col_max + 2;
    }

    ret.judge -= 2;
    return ret;
}

//计算在当前文件下，列宽应该为多少
static list_type calc_cols(const vft_t files) {
    int limit = get_winsize().ws_col;   //获取终端的最大列宽
    vint_t widths;
    for (auto i : files) {
        widths.push_back(i.fst_name.size());    //构建文件长度表
    }

    list_type test;
    int t = 1;
    while (true) {  //测试一行中最多能有多少列
        test = test_tier(t + 1, widths);
        if (test.judge <= limit)
            t++;
        else
            break;
    }
    return test_tier(t, widths);
}

//按普通形式输出一个文件列表
static int outone_normal(const vft_t filist) {
    list_type lt = calc_cols(filist);   //计算该文件列表可以有多少列

    int i = 0;
    bool fir = true;
    for (auto it : filist) {
        if (fir)
            fir = false;
        else
            printf("  ");
        setc(it.fst_mode, it.fst_name); //根据权限来指定颜色
        printf("%-*s", lt.widths_list[i], it.fst_name.c_str());
        canc(); //取消掉颜色
        if (++i == lt.cols)
            putchar('\n'), i = 0, fir = true;
    }
    putchar('\n');
    return 0;
}

//输出是否具有指定的权限，若有输出right,反之输出-
static int put_power(const mode_t mode, const int power, const char right) {
    if (mode & power)
        return putchar(right);
    return putchar('-');
}

//计算数字有多少位
static int calc_numbits(int num) {
    char buf[500];
    sprintf(buf, "%d", num);
    return strlen(buf);
}

//根据长整型的时间返回字符串形式的时间
static string calc_time(long times) {
    string ret = "";
    char buf[500];
    strcpy(buf, ctime(&times));
    int sz = strlen(buf);
    for (int i = 4; i < sz - 9; i++) {
        ret += buf[i];
    }
    return ret;
}

//按list形式输出一个list
static int outone_list(const vft_t filist) {
    vint_t wcalc[4];
    //简单的计算中间的列宽
    for (auto it : filist) {
        wcalc[0].push_back(it.fst_uid.size());
        wcalc[1].push_back(it.fst_gid.size());
        wcalc[2].push_back(calc_numbits(int(it.fst_size)));
        wcalc[3].push_back(calc_numbits(int(it.fst_linknum)));
    }
    int widths[4] {
        *max_element(wcalc[0].begin(), wcalc[0].end()),
        *max_element(wcalc[1].begin(), wcalc[1].end()),
        *max_element(wcalc[2].begin(), wcalc[2].end()),
        *max_element(wcalc[3].begin(), wcalc[3].end())
    };

    for (auto it : filist) {
        putchar(it.fst_type);                   //输出文件的类型
        put_power(it.fst_mode, S_IRUSR, 'r');   //输出权限
        put_power(it.fst_mode, S_IWUSR, 'w');
        put_power(it.fst_mode, S_IXUSR, 'x');
        put_power(it.fst_mode, S_IRGRP, 'r');
        put_power(it.fst_mode, S_IWGRP, 'w');
        put_power(it.fst_mode, S_IXGRP, 'x');
        put_power(it.fst_mode, S_IROTH, 'r');
        put_power(it.fst_mode, S_IWOTH, 'w');
        put_power(it.fst_mode, S_IXOTH, 'x');
        putchar(' ');
        printf("%*d ", widths[3], (int)it.fst_linknum);        
        printf("%-*s ", widths[0], it.fst_uid.c_str());
        printf("%-*s ", widths[1], it.fst_gid.c_str());
        printf("%*d ", widths[2], (int)it.fst_size);
        printf("%s ", calc_time(it.fst_creatime).c_str());
        setc(it.fst_mode, it.fst_name);
        printf("%s\n", it.fst_name.c_str());
        canc();
    }
    return 0;
}

//输出list
static int output_list(param_t param, const filelist_t filist) {
    if (check_param(param, PARAM_l))
        return outone_list(filist.nlist);
    return outone_normal(filist.nlist);
}

//输出map
static int output_map(param_t param, const filelist_t filist) {
    for (auto it : filist.nmap) {
        printf("%s:\n", it.first.c_str());
        if (check_param(param, PARAM_l))
            outone_list(it.second);
        else
            outone_normal(it.second);
        printf("\n");
    }
    return 0;
}

//通用output函数
int output(param_t param, const filelist_t flt) {
    if (flt.type == filelist_t::FLT_LIST)
        return output_list(param, flt);
    return output_map(param, flt);
}