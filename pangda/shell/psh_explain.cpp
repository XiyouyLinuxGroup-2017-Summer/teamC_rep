#include<pangda/psh.h>
#include<sys/wait.h>
#include<dirent.h>
#include<fcntl.h>
#include<cstring>

std::map<std::string, std::function<int(command_t)> > shell_commands;
extern char **envir;

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
    if (shell_commands.find(cmd.execfile) != shell_commands.end()) {
        shell_commands[cmd.execfile](cmd);
        cmd.is_right_cmd = 1; //built-in function
        return "";
    }
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

int exec_command(command_t &cmd) {
    if (cmd.is_right_cmd) {
        psh_error(cmd.is_right_cmd);
        return -1;
    }

    std::string path = find_exec(cmd);
    if (cmd.is_right_cmd) {
        psh_error(cmd.is_right_cmd);
        return -1;
    }

    char *arglist[cmd.arguments.size() + 1];
    char args[200][256];
    //strcpy(args[0], cmd.execfile.c_str());
    //arglist[0] = (char *)args[0];
    for (auto i = 0u; i < cmd.arguments.size(); i++) {
        strcpy(args[i], cmd.arguments[i].c_str());
        arglist[i] = (char *)args[i];
    }
    arglist[cmd.arguments.size()] = (char *)NULL;
/****lookup
    for (auto i : arglist) {
        std::cout << i << std::endl;
    }

    getchar();getchar();
*****/
    pid_t child = fork();
    if (child < 0) {
        psh_error(200); //fork error
        return -1;
    }

    //father process need to handle background problem && pipe
    if (child > 0) {
        if (cmd.is_background) {
            printf("[Process id] %d\n", (int)child);
            return 0;
        } else {
            int ret;
            /**
            wait(&ret);
            不可使用wait！使用wait会导致上一个未获得状态的僵死进程占用此位置,导致子进程还未运行完毕，
            父进程已经开始输出下一个命令行！
            **/
            if (waitpid(child, &ret, 0) == -1) {
                perror("psh");
                return -1;
            }   
            return 0;
        }
    }

    //child process
    if (child == 0) {
        if (cmd.is_redirect_stdin) {
            int fd = open(cmd.filename_in.c_str(), O_RDONLY);
            if (fd < 0) {
                perror("psh");
                psh_error(201); //redirect_stdin file doesn't exist.
                exit(0);
            }
            dup2(fd, STDIN_FILENO);
        }

        if (cmd.is_redirect_stdout) {
            mode_t mode = S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH;
            int fd = open(cmd.filename_out.c_str(), O_WRONLY | O_CREAT | O_TRUNC, mode);
            if (fd < 0) {
                psh_error(202); //redirect_stdout file doesn't exist.
                exit(0);
            }
            dup2(fd, STDOUT_FILENO);
        }

        int ret = execve(path.c_str(), arglist, envir);
        
        if (ret == -1) {
            perror("psh");
            exit(-1);
        }
        exit(0);    //end child process
    }
    return -1;
}