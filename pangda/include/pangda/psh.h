#pragma once
#ifndef _HEADER_PANGDA_SHELL__
#define _HEADER_PANGDA_SHELL__

#include<unistd.h>
#include<string>
#include<vector>
#include<iostream>
typedef std::vector<std::string> argument_t;
//typedef std::vector<std::

struct command_t {
    int is_right_cmd = 0;     //0 represent correct, any other number represent wrong.
    std::string execfile;
    argument_t arguments;
    bool is_redirect_stdin = -false;
    std::string filename_in;
    bool is_redirect_stdout = false;
    std::string filename_out;
    bool is_background = false;
    bool is_pipe = false;
    std::string pipe_prompt[2];
};


command_t prase_command(std::string command);
std::string string_trim(std::string s);
std::string get_tip();
int exec_command(command_t &cmd);
int psh_error(int error);
int shellfunc_exit(command_t);
int shellfunc_logout(command_t);
int shellfunc_cd(command_t);
#endif