#include<cstdio>


#define fclrprint(color, statement, ...) \
        printf(color); printf(statement, ##__VA_ARGS__); printf("\033[0m");
#define clrprintf(forecolor, backcolor, statment, ...) \
        printf("%s%s", forecolor, backcolor); \
        printf(statement, ##__VA_ARGS__); printf("\033[0m");

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

//param
const int PARAM_a = 0x01; //-a --all
const int PARAM_A = 0x02; //-A --almost-all
const int PARAM_f = 0x04; //-f do not sort
const int PARAM_r = 0x08; //-r reverse order while sorting
const int PARAM_R = 0x10; //-R list subdirectories recursively

//functions
struct winsize get_winsize();