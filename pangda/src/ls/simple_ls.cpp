#include<sys/stat.h>
#include<sys/types.h>
#include<sys/ioctl.h>
#include<unistd.h>
#include<dirent.h>
#include<string>
#include<algorithm>
#include<vector>
#include<pangda/ls.h>
#include<cmath>
using std::vector;
using std::string;

typedef vector<string> vstr_t;
typedef vector<int> vint_t;
typedef vector<vector<int> > vvint_t;

winsize get_winsize() {
    winsize ret;
    ioctl(STDIN_FILENO, TIOCGWINSZ, (char *)&ret);
    return ret;
}

DIR *openfolder(string pathname) {
    DIR *ret;
    if ((ret = opendir(pathname.c_str())) == NULL) {
        return NULL;
    }
    return ret;
}

vstr_t build(DIR *where) {
    vstr_t ret;
    dirent *t;
    while ((t = readdir(where)) != NULL) {
        ret.push_back(t->d_name);
    }
    sort(ret.begin(), ret.end());
    closedir(where);
    return ret;
}

struct list_type {
    int rows, cols, judge;
    vint_t widths_list;
};

list_type calc_cols(vstr_t filenames) {
    int limit = get_winsize().ws_col;
    auto find_pos = [](const int x, const int y, const int tier) -> int {
        return x + tier * y;
    };
    auto test_tier = [=](const int tier, const vint_t widths) -> list_type {
        vvint_t build;
        list_type ret { ceil(widths.size() / double(tier)),
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
    };

    vint_t widths;
    for (auto i : filenames) {
        widths.push_back(i.size());
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

void output_normal(vstr_t filenames) {
    int length = 0;
    for (auto i : filenames) {
        if (i.size() > length)
            length = i.size();
    }

    list_type lt = calc_cols(filenames);
    if (length == 0)
        length = 1;

    int i = 0;
    bool fir = true;
    for (auto it : filenames) {
        if (fir)
            fir = false;
        else
            printf("  ");
        printf("%-*s", lt.widths_list[i], it.c_str());
        if (++i == lt.cols)
            putchar('\n'), i = 0, fir = true;;
    }
    putchar('\n');
}

int main(int argc, char *argv[]) {
    DIR *t = openfolder("/");
    vstr_t t2 = build(t);
    output_normal(t2);
    return 0;
}
