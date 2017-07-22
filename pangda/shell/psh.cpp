#include<pangda/psh.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<cstdlib>

extern std::map<std::string, std::function<int(command_t)> > shell_commands;
char **envir;

int main(int argc, char *argv[], char **envp) {
    envir = envp;   //保存shell获得的环境变量，便于传递给子进程

    //构建内建命令与实现函数的映射
    shell_commands["exit"] = shellfunc_exit;
    shell_commands["logout"] = shellfunc_logout;
    shell_commands["cd"] = shellfunc_cd;

    while (true) {
        std::string st = readline(get_tip().c_str());   //获得用户输入的内容

        //若用户输入的不是全空格，则将这条命令保存在历史记录中。
        //否则就不处理这条命令，直接获得下一条命令。
        if (string_trim(st) == "")
            continue;
        else
            add_history(st.c_str());

        //解析命令
        command_t cmd = prase_command(st);

        //若命令是管道，则分别执行两条管道命令
        if (cmd.is_pipe) {
            command_t pipe1 = prase_command(cmd.pipe_prompt[0]);
            command_t pipe2 = prase_command(cmd.pipe_prompt[1]);
            if (exec_command(pipe1) != 0)   //若管道的第一条命令就是错误的，不再执行第二条命令
                continue;
            exec_command(pipe2);
        } else if (cmd.is_right_cmd) {  //若解析命令之后发现命令存在错误，则进入错误处理程序
            psh_error(cmd.is_right_cmd);
            continue;
        }
        exec_command(cmd);      //交由解释器解释执行命令
    }
    return 0;
}