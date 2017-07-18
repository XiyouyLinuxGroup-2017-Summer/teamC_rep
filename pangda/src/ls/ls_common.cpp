#include<pangda/ls.h>

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

static filelist_t build_filelist_map(const param_t param, DIR *where) {
    filelist_t ret = { filelist_t::FLT_MAP };
}

static filelist_t build_filelist_list(const param_t param, DIR *where) {
    filelist_t ret = { filelist_t::FLT_LIST };
    files_t temp;
    struct stat stbuf;
    dirent *thisfile;
    bool param_a = check_param(param, PARAM_a);
    bool param_A = check_param(param, PARAM_A);

    chdir(param.path.c_str());
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
        temp.fst_gid = stbuf.st_gid;
        temp.fst_uid = stbuf.st_uid;
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
        return a.fst_name > b.fst_name;
    };
    auto sort_revcmp = [](const files_t a, const files_t b) -> bool {
        return a.fst_name < b.fst_name;
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