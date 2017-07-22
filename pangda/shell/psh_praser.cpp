#include<pangda/psh.h>
#include<sstream>

//分割命令中的各个参数
static argument_t split_command(std::string command) {
    argument_t ret;
    std::stringstream out(command);     //构建字符串流
    std::string t;
    while (out >> t) {  //若流中仍然有内容
        ret.push_back(t);
    }
    return ret;
}

//分割语法，解决ls>c的问题
static void stylize_command(std::string &cmd) {
    for (auto i = 0u; i < cmd.length(); i++) {
        //offset用于计算偏移值，若在语法元素左右插入了空格，那么cmd的长度会发生变化
        //而i没有发生变化，因此需要计算偏移值。
        int offset = 0; 
        //若检测到了语法元素:><|&
        if (cmd[i] == '<' || cmd[i] == '>' || cmd[i] == '|' || cmd[i] == '&') {
            if (i - 1 >= 0 && cmd[i - 1] != ' ') {  //若语法元素左方没有空格
                cmd.insert(i, " ");
                offset--;
            }
            if (i + 1 < cmd.length() && cmd[i + 1 + offset] != ' ') {   //若语法元素右方无空格
                cmd.insert(i + 1, " ");
            }
        }
    }
}

//构建管道命令。管道的实现方式：将管道左右分别变为两个不同的命令，分别执行两个命令
static void pipe_buildcmd(command_t &origin_cmd) {
    auto in = origin_cmd.arguments;
    origin_cmd.pipe_prompt[0] = origin_cmd.pipe_prompt[1] = "";
    
    bool front = true;
    for (auto i : in) {
        if (i != "|") { //若未检测到|，说明仍然在管道左方
            if (front) {
                origin_cmd.pipe_prompt[0] += i + " ";
            } else {
                origin_cmd.pipe_prompt[1] += i + " ";
            }
        } else {
            front = false; 
        }
    }
    origin_cmd.pipe_prompt[0] += " > /tmp/psh_pipefile";    //补充管道左方命令
    origin_cmd.pipe_prompt[1] += " < /tmp/psh_pipefile";    //补充管道右方命令
}

//删除命令中语法元素所占参数表位置
static void setarg_command(command_t &cmdt) {
    std::vector<std::string> ret, in = cmdt.arguments;
    int sz = in.size();

    //若命令中不含任何语法元素，不含管道原因是若含有管道会提前返回，不在这里执行。
    if (!(cmdt.is_redirect_stdin || cmdt.is_redirect_stdout || cmdt.is_background)) {
        return;
    }

    //逐个检查参数列表中的内容
    for (int i = 0; i < sz; i++) {
        if (in[i] == ">" || in[i] == "<") { //若为<>，需要多跳过一个内容
            i++;
        } else if (in[i] == "&") {  //若为&，直接跳过即可，循环结束会自动i++
            continue;
        } else {
            ret.push_back(in[i]);   //没有语法元素说明是参数的一部分，应当保留
        }
    }
    cmdt.arguments = ret;   //改变原本的参数表
}

//标记语法元素
static void setmark_command(command_t &cmdt) {
    //从除过命令名之后的其他元素开始逐个检查
    for (auto it = cmdt.arguments.begin() + 1; it != cmdt.arguments.end(); it++) {
        //若出现>元素
        if (*it == ">") {
            cmdt.is_redirect_stdout = true; //标记>语法元素
            if (it + 1 == cmdt.arguments.end()) {
                cmdt.is_right_cmd = 400; //错误400：>语法元素后没有跟任何内容，错误语法
                break;
            }
            cmdt.filename_out = *++it;  //存储重定向的文件名
            continue;
        } else if (*it == "<") {
            cmdt.is_redirect_stdin = true;  //标记<语法元素
            if (it + 1 == cmdt.arguments.end()) {
                cmdt.is_right_cmd = 401; //错误400：>语法元素后没有跟任何内容，错误语法
                break;
            }
            cmdt.filename_in = *++it; //存储重定向的文件名
            continue;
        } else if (*it == "|") {
            cmdt.is_pipe = true;    //标记|语法元素
            cmdt.is_right_cmd = 490;    //错误490：命令是一个管道，应当使用管道的执行方法
            pipe_buildcmd(cmdt);    //构建管道命令
            break;
        } else if (*it == "&") {
            if (it + 1 == cmdt.arguments.end()) {
                cmdt.is_background = true;  //标记&语法元素
            } else {
                cmdt.is_right_cmd = 403; //错误400：&语法元素出现在了命令中间，错误语法
                break;
            }
        }
    }
    if (cmdt.is_right_cmd || cmdt.is_pipe)  //若命令是错误的或者是一个管道，则交由各自的处理程序处理
        return;
    setarg_command(cmdt);   //删除句中出现的多余语法元素
}

command_t prase_command(std::string command) {
    command_t ret;
    stylize_command(command);   //让句中的语法元素与其他元素分割开来
    ret.arguments = split_command(command); //构建参数列表
    ret.execfile = ret.arguments[0];    //指定要执行的命令名
    setmark_command(ret);   //标识命令中出现的语法元素
    return ret;
}