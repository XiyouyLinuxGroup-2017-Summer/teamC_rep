#include<pangda/ls.h>
#include<ctime>
#include<cstring>
//forecolor
const char *FCLR_BLACK = "\033[30m";
const char *FCLR_RED = "\033[31m";
const char *FCLR_GREEN = "\033[32m";
const char *FCLR_YELLOW = "\033[33m";
const char *FCLR_BLUE = "\033[34m";
const char *FCLR_PURPLE = "\033[35m";
const char *FCLR_SKY = "\033[36m";
const char *FCLR_WHITE = "\033[37m";

//backcolor
const char *BCLR_BLACK = "\033[40m";
const char *BCLR_RED = "\033[41m";
const char *BCLR_GREEN = "\033[42m";
const char *BCLR_YELLOW = "\033[43m";
const char *BCLR_BLUE = "\033[44m";
const char *BCLR_PURPLE = "\033[45m";
const char *BCLR_SKY = "\033[46m";
const char *BCLR_WHITE = "\033[47m";

static winsize get_winsize() {
    winsize ret;
    ioctl(STDIN_FILENO, TIOCGWINSZ, (char *)&ret);
    return ret;
}

static int setc(mode_t mode, string filename) {
    if (S_ISDIR(mode))
        return printf("\033[36m");
    if (S_ISLNK(mode))
        return printf("\033[33;35m");
    if ((mode & S_IXUSR) || (mode & S_IXOTH) || (mode & S_IXGRP))
        return printf("\033[32m");
    return printf("\033[0m");
}

static int canc() {
    return printf("\033[0m");
}

inline static unsigned int find_pos(const int x, const int y, const int tier) {
    return (x + tier * y);
}

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

static list_type calc_cols(const vft_t files) {
    int limit = get_winsize().ws_col;

    vint_t widths;
    for (auto i : files) {
        widths.push_back(i.fst_name.size());
    }

    list_type test;
    int t = 1;
    while (true) {
        test = test_tier(t + 1, widths);
        if (test.judge <= limit)
            t++;
        else
            break;
    }
    return test_tier(t, widths);
}

static int outone_normal(const vft_t filist) {
    list_type lt = calc_cols(filist);

    int i = 0;
    bool fir = true;
    for (auto it : filist) {
        if (fir)
            fir = false;
        else
            printf("  ");
        setc(it.fst_mode, it.fst_name);
        printf("%-*s", lt.widths_list[i], it.fst_name.c_str());
        canc();
        if (++i == lt.cols)
            putchar('\n'), i = 0, fir = true;;
    }
    putchar('\n');

    return 0;
}

static int put_power(const mode_t mode, const int power, const char right) {
    if (mode & power)
        return putchar(right);
    return putchar('-');
}

static int calc_numbits(int num) {
    char buf[500];
    sprintf(buf, "%d", num);
    return strlen(buf);
}

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

static int outone_list(const vft_t filist) {
    vint_t wcalc[4];
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
        putchar(it.fst_type);
        put_power(it.fst_mode, S_IRUSR, 'r');
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

static int output_list(param_t param, const filelist_t filist) {
    if (check_param(param, PARAM_l))
        return outone_list(filist.nlist);
    return outone_normal(filist.nlist);
}

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

int output(param_t param, const filelist_t flt) {
    if (flt.type == filelist_t::FLT_LIST)
        return output_list(param, flt);
    return output_map(param, flt);
}