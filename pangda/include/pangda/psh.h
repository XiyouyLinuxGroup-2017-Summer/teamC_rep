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
    int pipe_nextno; //next command saved in where?
};


command_t prase_command(std::string command);
void exec_command(command_t &cmd);
int psh_error(int error);
#endif