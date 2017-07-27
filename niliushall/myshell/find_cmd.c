#include "myshell.h"

/*find command*/
int find_cmd(char *command){
	DIR *dir;
	struct dirent *ptr;
	char *path[] = {"./", "/bin", "/usr/bin", NULL};

	/*判断是否在本目录下，是则删掉'./'*/
	if(!strncmp(command, "./", 2)){
		command += 2;
	}
	
	int i = 0;
	while(path[i] != NULL){
		if((dir = opendir(path[i])) == NULL){
			printf("can not open the directory.\n");
			return 0;
		}

		while((ptr = readdir(dir)) != NULL){
			if(!strcmp(ptr->d_name, command)){
				closedir(dir);
				return 1;
			}
		}

		closedir(dir);
		i++;
	}
    return 0;
}
