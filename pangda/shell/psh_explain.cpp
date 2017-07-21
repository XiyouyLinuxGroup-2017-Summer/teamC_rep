#include<pangda/psh.h>
#include<dirent.h>

static std::vector<std::string> split_string(std::string str, char sep) {
    std::vector<std::string> ret;
    unsigned int start = 0;
    for (auto i = 0u; i < str.length(); i++) {
        if (str[i] == sep) {
            ret.push_back(str.substr(start, i - start));
            start = i + 1;
        }
    }
    ret.push_back(str.substr(start, str.length()));
    return ret;
}

static std::string find_exec(command_t &cmd) {
    std::vector<std::string> envpath = split_string(getenv("PATH"), ':');
    for (auto it : envpath) {
        DIR *dp = opendir(it.c_str());
        dirent *dirp;
        if (dp == NULL) {
            cmd.is_right_cmd = 300;     //environment var error
            return "";
        }
        while ((dirp = readdir(dp)) != NULL) {
            if (cmd.execfile == dirp->d_name) {
                std::string ret = it;
                if (ret[ret.length() - 1] != '/') {
                    ret.push_back('/');
                }
                ret = ret + cmd.execfile;
                return ret;
            }
        }
    }
    cmd.is_right_cmd = 100;         //cannot found command;
    return "";
}

void exec_command(command_t &cmd) {
    std::string path = find_exec(cmd);
    if (cmd.is_right_cmd) {
        psh_error(cmd.is_right_cmd);
        return;
    }
    
}