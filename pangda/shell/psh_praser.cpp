#include<pangda/psh.h>
#include<sstream>

static argument_t split_command(std::string command) {
    argument_t ret;
    std::stringstream out(command);
    std::string t;
    while (out >> t) {
        ret.push_back(t);
    }
    return ret;
}

static void stylize_command(std::string &cmd) {
    for (auto i = 0u; i < cmd.length(); i++) {
        int offset = 0;
        if (cmd[i] == '<' || cmd[i] == '>' || cmd[i] == '|' || cmd[i] == '&') {
            if (i - 1 >= 0 && cmd[i - 1] != ' ') {
                cmd.insert(i, " ");
                offset--;
            }
            if (i + 1 < cmd.length() && cmd[i + 1 + offset] != ' ') {
                cmd.insert(i + 1, " ");
            }
        }
    }
}

static int pipe_buildcmd(command_t origin_cmd, std::vector<std::string>::iterator from) {
    return -1;
}

static void setmark_command(command_t &cmdt) {
    for (auto it = cmdt.arguments.begin() + 1; it != cmdt.arguments.end(); ++it) {
        if (*it == ">") {
            cmdt.is_redirect_stdout = true;
            cmdt.filename_out = *++it;  //FIXIT:Check if there's no match filename.
        } else if (*it == "<") {
            cmdt.is_redirect_stdin = true;
            cmdt.filename_out = *++it; //FIXIT:See LINE 17.
        } else if (*it == "|") {
            cmdt.is_pipe = true;
            cmdt.pipe_nextno = pipe_buildcmd(cmdt, it);   //fixit:if it doesn\'t has next.
        } else if (*it == "&") {
            if (it + 1 == cmdt.arguments.end()) {
                cmdt.is_background = true;
            } else {
                cmdt.is_right_cmd = false;
                break;
            }
        }
    }
}

command_t prase_command(std::string command) {
    command_t ret;
    stylize_command(command);
    ret.arguments = split_command(command);
    ret.execfile = ret.arguments[0];
    setmark_command(ret);

    return ret;
}