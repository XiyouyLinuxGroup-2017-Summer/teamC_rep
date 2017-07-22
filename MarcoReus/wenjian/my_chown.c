/*************************************************************************
  > File Name: my_chown.c
  > Author: YinJianxiang
  > Mail: YinJianxiang123@gmail.com 
  > Created Time: 2017年07月18日 星期二 15时38分06秒
 ************************************************************************/

#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	uid_t uid;
	gid_t gid;
	struct passwd *pwd;
	char *owner;
	char *group;

	if (argc == 3 ) {
		uid = strtol(argv[1], &owner, 10); 

		if (*owner != 0) {       
			pwd = getpwnam(argv[1]);  
			if (pwd == NULL) {
				perror("getpwnam");
				exit(0);
			}   

			uid = pwd->pw_uid;
		}

		if (chown(argv[2], uid, -1) == -1) {
			perror("chown");
			exit(0);
		}
	} else if (argc == 4) {
		uid = strtol(argv[1],&owner,10);
		gid = strtol(argv[2],&group,10);

		if(*owner != 0) {
			pwd = getpwnam(argv[1]);
			if(pwd == NULL) {
				perror("getnam");
				exit(0);
			}
		}

		if(chown(argv[3],uid,gid) == -1) {
			perror("chown");
			exit(0);
		}
	} else {
		printf("%s <owner> <group> filename or %s <owner> filename\n",argv[0],argv[0]);
		exit(0);
	}

	return 0;
}

