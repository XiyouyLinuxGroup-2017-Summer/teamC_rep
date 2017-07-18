#pragma once
#ifndef _HEADER_MYLS_
#define _HEADER_MYLS_
#include<cstdio>
#include<string>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/ioctl.h>
#include<unistd.h>
#include<dirent.h>
#include<algorithm>
#include<vector>
#include<cmath>
#include<map>

#define fclrprint(color, statement, ...) \
        printf(color); printf(statement, ##__VA_ARGS__); printf("\033[0m");

#define clrprint(forecolor, backcolor, statment, ...) \
        printf("%s%s", forecolor, backcolor); \
        printf(statement, ##__VA_ARGS__); printf("\033[0m");



//param
//output method:
//drwxr-xr-x 2 root root 4096 Sat May 12 14:15:46 Name
//mode&type linknum username groupname size creatime filename
const int PARAM_a = 0x01; //-a --all
const int PARAM_A = 0x02; //-A --almost-all
const int PARAM_f = 0x04; //-f do not sort
const int PARAM_r = 0x08; //-r reverse order while sorting
const int PARAM_R = 0x10; //-R list subdirectories recursively
const int PARAM_l = 0x11; //-l 

//typedef
using std::vector;
using std::string;

typedef vector<string> vstr_t;
typedef vector<int> vint_t;
typedef vector<vector<int> > vvint_t;


//structs
struct list_type {
    int rows, cols, judge;
    vint_t widths_list;
};

struct param_t {
    std::string path;
    int param_flag;
};

struct files_t {
    std::string fst_name;
    unsigned short fst_namelen;
    mode_t fst_mode;
    char fst_type;
    long fst_creatime;
    off_t fst_size;
    nlink_t fst_linknum;
    uid_t fst_uid;
    gid_t fst_gid;
};
typedef vector<files_t> vft_t;
struct filelist_t {
    static const bool FLT_LIST = true;
    static const bool FLT_MAP = false;
    bool type = FLT_MAP;
    vft_t nlist;
    std::map<std::string, vft_t> nmap;
};


param_t set_param(const int argc, const char *argv[]);
bool check_param(const param_t msg, const int param);
//inline bool check_param(const param_t msg, const int param);


#endif