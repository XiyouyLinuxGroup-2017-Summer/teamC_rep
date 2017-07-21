#include<pangda/psh.h>
#include<pwd.h>
#include<sys/utsname.h>
#include<cstring>

std::string get_username(uid_t uid) {
    passwd *ret = getpwuid(uid);
    if (ret == NULL)
        return "";
    return std::string(ret->pw_name);
}

std::string get_tip() {
    char hostname[65];
    gethostname(hostname, 65);
    char *curdir = getcwd(NULL, 0);
    std::string ret = "[" + get_username(getuid()) + "@" + hostname + " ";
    int lastpos = 0;
    for (auto i = 0u; i < strlen(curdir); i++) {
        if (curdir[i] == '/')
            lastpos = i;
    }
    std::string where = std::string(curdir).substr(lastpos + 1);
    if (where == "")
        where = "/";
    ret += where;
    ret += "]$ ";
    free(curdir);
    return ret;
}

int psh_error(int error) {
    switch (error) {
    //fixme： add error
    case 100:
        printf("psh: command not found.\n");
        break;
    case 201:
        printf("psh: file doesn\'t exist.\n");
        break;
    }
    return -1;
}

std::string string_trim(std::string s) {
    if(s.empty()) {
        return s;
    }
    s.erase(0,s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
    return s;
}

int shellfunc_cd(command_t cmdt) {
    if (chdir(cmdt.arguments[1].c_str()) != 0) {
        perror("psh");
        return -1;
    }
    return 0;
}

int shellfunc_exit(command_t cmdt) {
    exit(0);
    return 0;
}

int shellfunc_logout(command_t cmdt) {
    shellfunc_exit(cmdt);
    return 0;
}
