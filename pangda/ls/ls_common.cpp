#include<pangda/ls.h>
#include<pwd.h>
#include<grp.h>
#include<errno.h>
#include<queue>
using std::pair;
using std::make_pair;
using std::queue;

//打开一个目录并返回DIR流
static DIR *open_folder(const param_t param) {
    DIR *ret;
    if ((ret = opendir(param.path.c_str())) == NULL) {  //尝试打开
        return NULL;    //出错则返回NULL
    }
    return ret;
}

//返回一个文件的属性字，即目录返回d，链接返回l等等
static char check_type(const mode_t mode) {
    if (S_ISLNK(mode))
        return 'l';
    if (S_ISDIR(mode))
        return 'd';
    if (S_ISCHR(mode))
        return 'c';
    if (S_ISBLK(mode))
        return 'b';
    return '-';
}

//以map形式构建输出文件列表
static filelist_t build_filelist_map(param_t param, DIR *where) {
    filelist_t ret;
    ret.type = false;
    queue<pair<string, DIR *> > q;
    where = opendir(param.path.c_str());
    q.push(make_pair(param.path, where));   //将第一个目录放入队列中
    while (!q.empty()) {
        pair<string, DIR *> qt = q.front();
        q.pop();

        files_t temp;
        struct stat stbuf;
        dirent *thisfile;
        bool param_a = check_param(param, PARAM_a);     //检查是否具有a参数
        bool param_A = check_param(param, PARAM_A);     //检查是否具有A参数
        
        if (qt.second == NULL) {    //若没能打开DIR流，则继续构建下一个
            continue;
        }

        while ((thisfile = readdir(qt.second)) != NULL) {
            temp.fst_name = thisfile->d_name;   //设置名字
            if (!(param_A || param_a)) {        //判断是否存在aA参数，若无这两个参数，直接跳过.开头的文件
                if (temp.fst_name[0] == '.')
                    continue;
            } else if (param_A) {
                if (temp.fst_name == "." || temp.fst_name == "..")
                    continue;
            }
            temp.fst_namelen = thisfile->d_reclen;
            string statp = thisfile->d_name;        //构建绝对路径的文件名
            if (qt.first[qt.first.size() - 1] == '/')
                statp = qt.first + statp;
            else
                statp = qt.first + '/' +statp;

            if (lstat(statp.c_str(), &stbuf) == -1) {   //尝试获得文件属性
                continue;
            }
            temp.fst_creatime = stbuf.st_ctime;
            temp.fst_gid = get_groupname(stbuf.st_gid);
            temp.fst_uid = get_username(stbuf.st_uid);
            temp.fst_linknum = stbuf.st_nlink;
            temp.fst_mode = stbuf.st_mode;
            temp.fst_type = check_type(stbuf.st_mode);
            temp.fst_size = stbuf.st_size;
            ret.nmap[qt.first].push_back(temp);     //将获得的文件输行放入列表中

            //若是一个目录，而且不是.和..目录，将之放入队列中，构建他的列表
            if (temp.fst_type == 'd' && temp.fst_name != "." && temp.fst_name != "..") {
                string newpath = qt.first + temp.fst_name;
                if (newpath[newpath.size() - 1] != '/')
                    newpath += '/';
                DIR *newdir = opendir(newpath.c_str());
                q.push(make_pair(newpath, newdir));
            }
        }
        closedir(qt.second);    //关闭文件流

        if (check_param(param, PARAM_f)) {  //若设定了f参数，那么就不进行排序
            continue;
        }

        //lambda：排序的比较函数 -> bool
        static auto sort_cmp = [](const files_t a, const files_t b) -> bool {
            return a.fst_name < b.fst_name;
        };
        static auto sort_revcmp = [](const files_t a, const files_t b) -> bool {
            return a.fst_name > b.fst_name;
        };

        if (check_param(param, PARAM_r))    //若设定了r参数，则使用逆序排序函数
            sort(ret.nmap[qt.first].begin(), ret.nmap[qt.first].end(), sort_revcmp);
        else    //反之使用正常的排序函数
            sort(ret.nmap[qt.first].begin(), ret.nmap[qt.first].end(), sort_cmp);
    }

    return ret;
}

//以list形式构建输出文件列表
static filelist_t build_filelist_list(param_t param, DIR *where) {
    filelist_t ret = { filelist_t::FLT_LIST };
    files_t temp;
    struct stat stbuf;
    dirent *thisfile;
    //检查是否具有指定参数
    bool param_a = check_param(param, PARAM_a);
    bool param_A = check_param(param, PARAM_A);

    chdir(param.path.c_str());
    if (where == NULL) {    //若没有打开文件流，则报错
        perror("ls");
        exit(-1);
    }

    //以下内容与map形式类似
    while ((thisfile = readdir(where)) != NULL) {
        temp.fst_name = thisfile->d_name;
        if (!(param_A || param_a)) {
            if (temp.fst_name[0] == '.')
                continue;
        } else if (param_A) {
            if (temp.fst_name == "." || temp.fst_name == "..")
                continue;
        }
        temp.fst_namelen = thisfile->d_reclen;

        if (lstat(thisfile->d_name, &stbuf) == -1) {
            return filelist_t();
        }
        temp.fst_creatime = stbuf.st_ctime;
        temp.fst_gid = get_groupname(stbuf.st_gid);
        temp.fst_uid = get_username(stbuf.st_uid);
        temp.fst_linknum = stbuf.st_nlink;
        temp.fst_mode = stbuf.st_mode;
        temp.fst_type = check_type(stbuf.st_mode);
        temp.fst_size = stbuf.st_size;
        ret.nlist.push_back(temp);
    }

    closedir(where);
    if (check_param(param, PARAM_f)) {
        return ret;
    }
    auto sort_cmp = [](const files_t a, const files_t b) -> bool {
        return a.fst_name < b.fst_name;
    };
    auto sort_revcmp = [](const files_t a, const files_t b) -> bool {
        return a.fst_name > b.fst_name;
    };

    if (check_param(param, PARAM_r))
        sort(ret.nlist.begin(), ret.nlist.end(), sort_revcmp);
    else
        sort(ret.nlist.begin(), ret.nlist.end(), sort_cmp);

    return ret;
}

filelist_t build_filelist(const param_t param) {
    DIR *where = open_folder(param);        //打开目录
    if (check_param(param, PARAM_R))        //若设定了R参数，则以map形式构建
        return build_filelist_map(param, where);
    return build_filelist_list(param, where);   //反之以list形式构建
}

//获得用户名
string get_username(uid_t uid) {
    passwd *ret = getpwuid(uid);
    if (ret == NULL)
        return "";
    return string(ret->pw_name);
}

//获得用户组名
string get_groupname(gid_t gid) {
    group *ret = getgrgid(gid);
    if (ret == NULL)
        return "";
    return string(ret->gr_name);
}