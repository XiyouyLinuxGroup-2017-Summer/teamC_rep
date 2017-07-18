#include<pangda/ls.h>
#include<pwd.h>
#include<grp.h>
#include<queue>
using std::pair;
using std::make_pair;
using std::queue;

static DIR *open_folder(const param_t param) {
    DIR *ret;
    if ((ret = opendir(param.path.c_str())) == NULL) {
        return NULL;
    }
    return ret;
}

static char check_type(const mode_t mode) {
    if (S_ISLNK(mode))
        return 'l';
    if (S_ISDIR(mode))
        return 'd';
    return '-';
}

static filelist_t build_filelist_map(param_t param, DIR *where) {
    filelist_t ret;
    ret.type = false;
    queue<pair<string, DIR *> > q;
    where = opendir(param.path.c_str());
    q.push(make_pair(param.path, where));
    while (!q.empty()) {
        pair<string, DIR *> qt = q.front();
        q.pop();

        files_t temp;
        struct stat stbuf;
        dirent *thisfile;
        bool param_a = check_param(param, PARAM_a);
        bool param_A = check_param(param, PARAM_A);
        
        if (qt.second == NULL) {
            continue;
        }

        while ((thisfile = readdir(qt.second)) != NULL) {
            temp.fst_name = thisfile->d_name;
            if (!(param_A || param_a)) {
                if (temp.fst_name[0] == '.')
                    continue;
            } else if (param_A) {
                if (temp.fst_name == "." || temp.fst_name == "..")
                    continue;
            }
            temp.fst_namelen = thisfile->d_reclen;
            string statp = thisfile->d_name;
            if (qt.first[qt.first.size() - 1] == '/')
                statp = qt.first + statp;
            else
                statp = qt.first + '/' +statp;

            if (lstat(statp.c_str(), &stbuf) == -1) {
                printf("##lstat,continue here.path:%s,errno:%d##", statp.c_str(), errno); getchar();
                continue;
            }
            temp.fst_creatime = stbuf.st_ctime;
            temp.fst_gid = get_groupname(stbuf.st_gid);
            temp.fst_uid = get_username(stbuf.st_uid);
            temp.fst_linknum = stbuf.st_nlink;
            temp.fst_mode = stbuf.st_mode;
            temp.fst_type = check_type(stbuf.st_mode);
            temp.fst_size = stbuf.st_size;
            ret.nmap[qt.first].push_back(temp);

            if (temp.fst_type == 'd' && temp.fst_name != "." && temp.fst_name != "..") {
                string newpath = qt.first + temp.fst_name;
                if (newpath[newpath.size() - 1] != '/')
                    newpath += '/';
                DIR *newdir = opendir(newpath.c_str());
                q.push(make_pair(newpath, newdir));
            }
        }
        closedir(qt.second);
        if (check_param(param, PARAM_f)) {
            continue;
        }
        static auto sort_cmp = [](const files_t a, const files_t b) -> bool {
            return a.fst_name < b.fst_name;
        };
        static auto sort_revcmp = [](const files_t a, const files_t b) -> bool {
            return a.fst_name > b.fst_name;
        };

        if (check_param(param, PARAM_r))
            sort(ret.nmap[qt.first].begin(), ret.nmap[qt.first].end(), sort_revcmp);
        else
            sort(ret.nmap[qt.first].begin(), ret.nmap[qt.first].end(), sort_cmp);
    }

    return ret;
}

static filelist_t build_filelist_list(param_t param, DIR *where) {
    filelist_t ret = { filelist_t::FLT_LIST };
    files_t temp;
    struct stat stbuf;
    dirent *thisfile;
    bool param_a = check_param(param, PARAM_a);
    bool param_A = check_param(param, PARAM_A);

    chdir(param.path.c_str());
    if (where == NULL) {
        printf("Permission Denined.\n");
        exit(-1);
    }
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
    DIR *where = open_folder(param);
    if (check_param(param, PARAM_R))
        return build_filelist_map(param, where);
    return build_filelist_list(param, where);
}

string get_username(uid_t uid) {
    passwd *ret = getpwuid(uid);
    if (ret == NULL)
        return "";
    return string(ret->pw_name);
}

string get_groupname(gid_t gid) {
    group *ret = getgrgid(gid);
    if (ret == NULL)
        return "";
    return string(ret->gr_name);
}