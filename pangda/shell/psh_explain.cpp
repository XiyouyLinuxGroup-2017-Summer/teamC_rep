#include<pangda/psh.h>
#include<sys/wait.h>
#include<dirent.h>
#include<fcntl.h>
#include<cstring>

//shell_commands：构建内建命令与处理函数的映射
std::map<std::string, std::function<int(command_t)> > shell_commands;
extern char **envir;    //导入环境变量

//分割字符串，主要用于分割PATH环境变量。让a:b:c变成["a","b","c"]的列表，方便查找
static std::vector<std::string> split_string(std::string str, char sep) {
    std::vector<std::string> ret;
    unsigned int start = 0; //用于标记上一个分割符所在的位置
    for (auto i = 0u; i < str.length(); i++) {
        if (str[i] == sep) {    //若当前指向的位置就是分割符
            ret.push_back(str.substr(start, i - start));
            start = i + 1;
        }
    }
    ret.push_back(str.substr(start, str.length()));
    return ret;
}

static std::string find_exec(command_t &cmd) {
    //检查命令是否属于内建命令
    if (shell_commands.find(cmd.execfile) != shell_commands.end()) { 
        shell_commands[cmd.execfile](cmd);  //若属于内建命令直接执行相关函数
        cmd.is_right_cmd = 1; //错误1：命令是一个内建命令，无需处理
        return "";
    }
    
    //命令不属于内建命令，那么依次在环境变量目录中查找命令的可执行文件
    std::vector<std::string> envpath = split_string(getenv("PATH"), ':');   //构建目录列表
    envpath.push_back("./");    //将当前目录也放在查找列表中

    for (auto it : envpath) {
        DIR *dp = opendir(it.c_str());      //打开相应目录，开始查找
        dirent *dirp;
        if (dp == NULL) {
            cmd.is_right_cmd = 300;     //错误300：环境变量中配置的PATH目录有错误
            return "";
        }
        while ((dirp = readdir(dp)) != NULL) {
            if (it != "./" && cmd.execfile == dirp->d_name) {   //若不再当前目录下查找
                std::string ret = it;
                if (ret[ret.length() - 1] != '/') {
                    ret.push_back('/');
                }
                ret = ret + cmd.execfile;
                return ret;        
            }
            //在当前目录下要保证前方有./才去执行
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
    cmd.is_right_cmd = 100;    //错误100：命令不存在
    return "";
}

int exec_command(command_t &cmd) {
    //若命令存在错误，执行错误处理程序
    if (cmd.is_right_cmd) {
        psh_error(cmd.is_right_cmd);
        return -1;
    }

    std::string path = find_exec(cmd);  //查找到命令的绝对路径

    //若命令未查找到，那么执行错误处理程序
    if (cmd.is_right_cmd) {
        psh_error(cmd.is_right_cmd);
        return -1;
    }

    //构建符合系统调用要求的参数列表
    char *arglist[cmd.arguments.size() + 1];
    char args[200][256];
    for (auto i = 0u; i < cmd.arguments.size(); i++) {
        strcpy(args[i], cmd.arguments[i].c_str());
        arglist[i] = (char *)args[i];
    }
    arglist[cmd.arguments.size()] = (char *)NULL; //参数列表要以NULL结尾，不然会出错误

    pid_t child = fork();   //调用fork()

    if (child < 0) {    //若fork<0，说明fork出错
        psh_error(200);     //错误200：无法fork()出子进程
        return -1;
    }

    //若child>0，说明在执行的是父进程
    if (child > 0) {
        //如果是后台进程，那么不再管他，输出完他的id后继续读取下一条命令
        if (cmd.is_background) {
            printf("[Process id] %d\n", (int)child);
            return 0;
        } else {
            int ret;
            //等待子进程运行完毕，不使用wait()的原因是：
            //若在此之前有一个带有&语法元素的进程已经执行完毕，那么这里调用wait获取到的是上一个
            //执行完毕的后台进程的状态，这样可能造成当前命令还没有执行完毕，
            //父进程已经退出了等待程序开始等待新的输入。
            if (waitpid(child, &ret, 0) == -1) {    //若出现错误
                perror("psh");
                return -1;
            }   
            return 0;
        }
    }

    //若child=0，说明执行的是子进程
    if (child == 0) {
        if (cmd.is_redirect_stdin) {    //若有<语法元素
            int fd = open(cmd.filename_in.c_str(), O_RDONLY);   //打开重定向的stdin
            if (fd < 0) {
                psh_error(201); //错误201：打开文件出错
                exit(0);
            }
            //不使用dup的原因是，dup只会复制文件描述符，不可以指定新的文件描述符是什么
            //这里将新的描述符指定为标准输入的文件描述符，dup2会自动关闭原来的STDIN描述符
            //并将fd的文件描述符设置为STDIN_FILENO
            dup2(fd, STDIN_FILENO);
        }

        if (cmd.is_redirect_stdout) {   //若有>语法元素
            mode_t mode = S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH;    //配置文件属性
            int fd = open(cmd.filename_out.c_str(), O_WRONLY | O_CREAT | O_TRUNC, mode);
            if (fd < 0) {
                psh_error(202); //错误201：打开文件出错
                exit(0);
            }
            //不使用dup的原因同理
            dup2(fd, STDOUT_FILENO);
        }

        int ret = execve(path.c_str(), arglist, envir);
        
        if (ret == -1) {
            perror("psh");
            exit(-1);
        }
        exit(0);    //结束掉子进程
    }
    return -1;
}