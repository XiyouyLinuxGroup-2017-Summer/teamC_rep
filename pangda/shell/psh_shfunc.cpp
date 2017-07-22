#include<pangda/psh.h>
#include<sys/utsname.h>
#include<pwd.h>
#include<cstring>

//获得当前用户名
std::string get_username(uid_t uid) {
    passwd *ret = getpwuid(uid);
    if (ret == NULL)
        return "";
    return std::string(ret->pw_name);
}

//获得shell提示符
//返回类似于："[username@hostname folder] #"
std::string get_tip() {
    char hostname[65];
    gethostname(hostname, 65);  //获得主机名
    char *curdir = getcwd(NULL, 0); //获得当前工作目录
    std::string ret = "[" + get_username(getuid()) + "@" + hostname + " ";

    int lastpos = 0;
    for (auto i = 0u; i < strlen(curdir); i++) {
        if (curdir[i] == '/')
            lastpos = i;
    }
    std::string where = std::string(curdir).substr(lastpos + 1);//将工作目录前的内容删除
    //构建shell提示符

    if (where == "")
        where = "/";    //这里若where是空，那么这里工作目录在根目录下
    ret += where;
    ret += "]";
    ret += ((geteuid() == 0) ? "# " : "$ ");    //当以root权限运行时，提示符为'#'，否则为'$'
    free(curdir);   //释放getcwd()以malloc()开辟的空间
    return ret;
}

//根据错误码输出错误信息
int psh_error(int error) {
    switch (error) {
    case 1:
        break;
    case 100:
        printf("psh: command not found.\n");
        break;
    case 201:case 202:
        printf("psh: file doesn\'t exist.\n");
        break;
    case 300:
        printf("psh: environment error.\n");
        break;
    case 400:case 401:case 403:
        printf("psh: error present.\n");
        break;
    }
    return -1;
}

//去除字符串两侧多余的空格
std::string string_trim(std::string s) {
    if(s.empty()) {
        return s;
    }
    s.erase(0,s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
    return s;
}

//内建命令：cd
int shellfunc_cd(command_t cmdt) {
    if (cmdt.arguments.size() == 1)  {
        cmdt.arguments.push_back(".");  //若cd无参数，默认给参数.
    }
    if (chdir(cmdt.arguments[1].c_str()) != 0) {    //切换shell的工作目录，若失败则输出原因。
        perror("psh");
        return -1;
    }
    return 0;
}

//内建命令：exit
int shellfunc_exit(command_t cmdt) {
    exit(0);
    return 0;
}

//内建命令：logout
int shellfunc_logout(command_t cmdt) {
    return shellfunc_exit(cmdt);   //logout功能与exit相同，直接跳转使若exit有更多功能不必再次复制
}
