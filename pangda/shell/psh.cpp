/****************************************************
Goal:
1.commands with arguments
like: ls -l /tmp
2.input / output
ls -l / > a
wc -c < a
3.pipe
ls -l / | wc -c
4.&
5.alias
6.chdir
*****************************************************/
#include<iostream>
#include<cstdlib>
#include<pangda/psh.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<string>
#include<map>
#include<functional>
extern std::map<std::string, std::function<int(command_t)> > shell_commands;

std::map<std::string, std::string> shell_alias;
char **envir;
int main(int argc, char *argv[], char **envp) {
    envir = envp;
    shell_commands["exit"] = shellfunc_exit;
    shell_commands["logout"] = shellfunc_logout;
    shell_commands["cd"] = shellfunc_cd;
    while (true) {
        std::string st = readline(get_tip().c_str());
        if (string_trim(st) == "")
            continue;
        else
            add_history(st.c_str());
        command_t cmd = prase_command(st);
        if (cmd.is_pipe) {
            command_t pipe1 = prase_command(cmd.pipe_prompt[0]);
            command_t pipe2 = prase_command(cmd.pipe_prompt[1]);
            if (exec_command(pipe1) != 0)
                continue;
            exec_command(pipe2);
        } else if (cmd.is_right_cmd) {
            psh_error(cmd.is_right_cmd);
            continue;
        }
        exec_command(cmd);
    }
    return 0;
}