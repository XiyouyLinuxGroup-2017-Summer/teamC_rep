#pragma once
#ifndef _HEADER_MYLS_
#define _HEADER_MYLS_
#include<string>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/ioctl.h>
#include<unistd.h>
#include<dirent.h>
#include<algorithm>
#include<vector>
#include<cmath>
#include<map>

using std::string;

//param
const int PARAM_a = 0x01; //-a --all do not ignore entries starting with .
const int PARAM_A = 0x02; //-A --almost-all do not list implied . and ..
const int PARAM_f = 0x04; //-f do not sort
const int PARAM_r = 0x08; //-r reverse order while sorting
const int PARAM_R = 0x10; //-R list subdirectories recursively
const int PARAM_l = 0x20; //-l use a long listing format

//structs
struct list_type {
    int rows, cols, judge;
    std::vector<int> widths_list;
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
    std::string fst_uid;
    std::string fst_gid;
};

struct filelist_t {
    static const bool FLT_LIST = true;
    static const bool FLT_MAP = false;
    bool type = FLT_MAP;
    std::vector<files_t> nlist;
    std::map<std::string,  std::vector<files_t> > nmap;
};

//typedef
typedef std::vector<std::string> vstr_t;
typedef std::vector<int> vint_t;
typedef std::vector<std::vector<int> > vvint_t;
typedef std::vector<files_t> vft_t;

//Function: ls_display.cpp
int output(param_t param, const filelist_t flt);
//Functions: ls_param.cpp
param_t set_param(const int argc, char *argv[]);
bool check_param(const param_t msg, const int param);
//Functions: ls_common.cpp
filelist_t build_filelist(const param_t param);
string get_username(uid_t uid);
string get_groupname(gid_t gid);

#endif