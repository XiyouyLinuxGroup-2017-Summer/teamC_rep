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

//表示参数常量
const int PARAM_a = 0x01; //-a --all        显示所有文件
const int PARAM_A = 0x02; //-A --almost-all 显示除过.和..之外的所有文件
const int PARAM_f = 0x04; //-f              不进行排序
const int PARAM_r = 0x08; //-r              逆序排序显示
const int PARAM_R = 0x10; //-R              递归地显示目录下所有目录的文件
const int PARAM_l = 0x20; //-l              以列表的形式显示

struct list_type {
    int rows, cols, judge;          //几行几列
    std::vector<int> widths_list;   //宽度表，指每一列应该是多少宽度
};

struct param_t {
    std::string path;   //ls哪一个路径
    int param_flag;     //都有哪些参数
};

struct files_t {
    std::string fst_name;           //文件名
    unsigned short fst_namelen;     //文件名长度，用来计算列宽
    mode_t fst_mode;                //文件权限
    char fst_type;                  //文件类型，l表示链接，d表示目录，-表示其他
    long fst_creatime;              //文件的更改时间
    off_t fst_size;                 //文件大小
    nlink_t fst_linknum;            //文件被硬链接的次数
    std::string fst_uid;            //文件所属的用户的uid
    std::string fst_gid;            //文件所属的用户组的gid
};

struct filelist_t {
    static const bool FLT_LIST = true;
    static const bool FLT_MAP = false;
    bool type = FLT_MAP;                                //文件列表的类型，true表示nlist可用，false代表nmap可用
    std::vector<files_t> nlist;
    std::map<std::string,  std::vector<files_t> > nmap;
};

typedef std::vector<std::string> vstr_t;
typedef std::vector<int> vint_t;
typedef std::vector<std::vector<int> > vvint_t;
typedef std::vector<files_t> vft_t;

//函数定义
int output(param_t param, const filelist_t flt);
param_t set_param(const int argc, char *argv[]);
bool check_param(const param_t msg, const int param);
filelist_t build_filelist(const param_t param);
string get_username(uid_t uid);
string get_groupname(gid_t gid);

#endif