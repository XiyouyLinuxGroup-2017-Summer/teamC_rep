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
#include<string>
#include<map>
#include<functional>


std::map<std::string, std::string> shell_alias;
std::map<std::string, std::function<int()> > shell_commands;

int main(int argc, char *argv[], char **envp) {
    while (true) {
        std::string st = readline("psh @ ");
        command_t cmd = prase_command(st);
        exec_command(cmd);
    }
    return 0;
}