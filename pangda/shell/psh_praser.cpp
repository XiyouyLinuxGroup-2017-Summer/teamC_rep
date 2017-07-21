#include<pangda/psh.h>
#include<sstream>
#include<map>
#include<functional>


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

static void pipe_buildcmd(command_t &origin_cmd) {
    auto in = origin_cmd.arguments;
    origin_cmd.pipe_prompt[0] = origin_cmd.pipe_prompt[1] = "";
    
    bool front = true;
    for (auto i : in) {
        if (i != "|") {
            if (front) {
                origin_cmd.pipe_prompt[0] += i + " ";
            } else {
                origin_cmd.pipe_prompt[1] += i + " ";
            }
        } else {
            front = false;
        }
    }
    origin_cmd.pipe_prompt[0] += " > /tmp/psh_pipefile";
    origin_cmd.pipe_prompt[1] += " < /tmp/psh_pipefile";
}

static void setarg_command(command_t &cmdt) {
    std::vector<std::string> ret, in = cmdt.arguments;
    int sz = in.size();
    if (!(cmdt.is_redirect_stdin || cmdt.is_redirect_stdout || cmdt.is_background)) {
        return;
    }
    for (int i = 0; i < sz; i++) {
        if (in[i] == ">" || in[i] == "<") {
            i++;
        } else if (in[i] == "&") {
            continue;
        } else {
            ret.push_back(in[i]);
        }
    }
    cmdt.arguments = ret;
}

static void setmark_command(command_t &cmdt) {
    for (auto it = cmdt.arguments.begin() + 1; it != cmdt.arguments.end(); it++) {
        if (*it == ">") {
            cmdt.is_redirect_stdout = true;
            if (it + 1 == cmdt.arguments.end()) {
                cmdt.is_right_cmd = 400; //wrong command present
                break;
            }
            cmdt.filename_out = *++it;
            continue;
        } else if (*it == "<") {
            cmdt.is_redirect_stdin = true;
            if (it + 1 == cmdt.arguments.end()) {
                cmdt.is_right_cmd = 401; //WRONG PRESENT
                break;
            }
            cmdt.filename_in = *++it; 
            continue;
        } else if (*it == "|") {
            cmdt.is_pipe = true;
            cmdt.is_right_cmd = 490; //pipe
            pipe_buildcmd(cmdt);
            break;   //fixit:if it doesn\'t has next.
        } else if (*it == "&") {
            if (it + 1 == cmdt.arguments.end()) {
                cmdt.is_background = true;
            } else {
                cmdt.is_right_cmd = 403; //wrong present
                break;
            }
        }
    }
    if (cmdt.is_right_cmd || cmdt.is_pipe)
        return;
    setarg_command(cmdt);
}

command_t prase_command(std::string command) {
    command_t ret;
    stylize_command(command);
    ret.arguments = split_command(command);
    ret.execfile = ret.arguments[0];
    setmark_command(ret);
    return ret;
}