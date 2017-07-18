#include<pangda/ls.h>
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

static list_type calc_cols(const filelist_t files) {
    int limit = get_winsize().ws_col;

    vint_t widths;
    for (auto i : files.nlist) {
        widths.push_back(i.fst_namelen);
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

static int output_normal(const filelist_t filist) {
    list_type lt = calc_cols(filist);

    int i = 0;
    bool fir = true;
    for (auto it : filist.nlist) {
        if (fir)
            fir = false;
        else
            printf("  ");
        printf("%-*s", lt.widths_list[i], it.fst_name.c_str());
        if (++i == lt.cols)
            putchar('\n'), i = 0, fir = true;;
    }
    putchar('\n');
}

int output(const filelist_t flt) {
    if (flt.type == filelist_t::FLT_LIST)
        return output_normal(flt);
    //return output_list(flt);
}