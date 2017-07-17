#include<sys/stat.h>
#include<sys/types.h>
#include<sys/ioctl.h>
#include<unistd.h>
#include<dirent.h>
#include<string>
#include<algorithm>
#include<vector>
//#include<pangda/ls.h>
#include"../../include/pangda/ls.h"
using std::vector;
using std::string;

typedef vector<string> vstr;
struct winsize TERMINALSIZE;

struct winsize get_winsize() {
    struct winsize ret;
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

vstr build(DIR *where) {
    vstr ret;
    struct dirent *t;
    while ((t = readdir(where)) != NULL) {
        ret.push_back(t->d_name);
    }
    sort(ret.begin(), ret.end());
    closedir(where);
    return ret;
}

void output_normal(vstr filenames) {
    int length = 0;
    for (auto i : filenames) {
        if (i.size() > length)
            length = i.size();
    }

    int oneline_nums = TERMINALSIZE.ws_col / (length + 2);
    if (!length)
        length = 1;

    int i = 0;
    for (auto it : filenames) {
        printf("  %-*s", length, it.c_str());
        if (++i == length)
            putchar('\n'), i = 0;
    }
}

int main() {
    TERMINALSIZE = get_winsize();
    DIR *t = openfolder("/");
    vstr t2 = build(t);
    output_normal(t2);
    return 0;
}
