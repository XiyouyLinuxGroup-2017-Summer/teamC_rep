#include<pangda/psh.h>
#include<sys/wait.h>
#include<dirent.h>
#include<fcntl.h>
#include<cstring>

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
    //fixme: below to check if command is shell-builtin function



    //fixme: above to check if command is shell-builtin function
    std::vector<std::string> envpath = split_string(getenv("PATH"), ':');
    envpath.push_back("./");
    for (auto it : envpath) {
        DIR *dp = opendir(it.c_str());
        dirent *dirp;
        if (dp == NULL) {
            cmd.is_right_cmd = 300;     //environment var error
            return "";
        }
        while ((dirp = readdir(dp)) != NULL) {
            if (it != "./" && cmd.execfile == dirp->d_name) {
                std::string ret = it;
                if (ret[ret.length() - 1] != '/') {
                    ret.push_back('/');
                }
                ret = ret + cmd.execfile;
                return ret;        
            }
            if (it == "./" && cmd.execfile.length() >= 2 && cmd.execfile.substr(2) == dirp->d_name) {
                if (cmd.execfile.substr(0, 2) != "./")
                    break;
                char *current_dir = getcwd(NULL, 0);
                cmd.execfile.erase(0, 1);
                std::string ret = current_dir + cmd.execfile;
                free(current_dir);
                return ret;
            }
        }
    }
    cmd.is_right_cmd = 100;         //cannot found command;
    return "";
}

void exec_command(command_t &cmd) {
    if (cmd.is_right_cmd) {
        psh_error(cmd.is_right_cmd);
        return;
    }

    std::string path = find_exec(cmd);
    if (cmd.is_right_cmd) {
        psh_error(cmd.is_right_cmd);
        return;
    }

    //FIXME: 没有去除掉>和>后的内容
    char *arglist[cmd.arguments.size() + 2];
    char args[200][256];
    //strcpy(args[0], cmd.execfile.c_str());
    //arglist[0] = (char *)args[0];
    for (auto i = 0u; i < cmd.arguments.size(); i++) {
        strcpy(args[i], cmd.arguments[i].c_str());
        arglist[i] = (char *)args[i];
    }
    arglist[cmd.arguments.size()] = (char *)NULL;


    pid_t child = fork();
    if (child < 0) {
        psh_error(200); //fork error
        return;
    }

    //father process need to handle background problem && pipe
    if (child > 0) {
        if (cmd.is_background) {
            printf("[Process id] %d\n", (int)child);
            return;
        } else {
            int ret;
            wait(&ret);
            return;
        }
    }

    //child process
    if (child == 0) {
        if (cmd.is_redirect_stdin) {
            int fd = open(cmd.filename_in.c_str(), O_RDONLY);
            if (fd < 0) {
                psh_error(201); //redirect_stdin file doesn't exist.
                return;
            }
            dup2(fd, STDIN_FILENO);
        }

        if (cmd.is_redirect_stdout) {
            mode_t mode = S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH;
            int fd = open(cmd.filename_out.c_str(), O_WRONLY | O_CREAT, mode);
            if (fd < 0) {
                psh_error(202); //redirect_stdout file doesn't exist.
                return;
            }
            dup2(fd, STDOUT_FILENO);
        }
        int ret = execvp(path.c_str(), arglist);
        
        if (ret == -1) {
            printf("execfailed, path = %s\n", path.c_str());
            perror("psh");
        }
        exit(0);    //end child process
    }
}