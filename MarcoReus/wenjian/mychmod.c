/*************************************************************************
  > File Name: mychmod.c
  > Author: YinJianxiang
  > Mail: YinJianxiang123@gmail.com 
  > Created Time: 2017年07月17日 星期一 12时40分31秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>

int add_mode(int mode,char t,int count) {
	if(t == 'r') {
		mode += 4 * count; 
	} else if(t == 'w') {
		mode += 2 * count;
	} else if(t == 'x') {
		mode += 1 * count;
	}
}

int sub_mode(int mode,char t,int count) {
	if(t == 'r') {
		mode -= 4 * count;
	} else if(t == 'w') {
		mode -= 2 * count;
	} else if(t == 'x') {
		mode -= 1 * count;
	}
}


int main(int argc,char *argv[]) {
	int mode;		//权限
	int mode_u;		//所有者权限
	int mode_g;		//组权限
	int mode_o;		//其他人权限
	char *file_name;
	struct stat buf;
	int count;

	if(argc == 3) {
		/*atoi将字符串转化成int*/
		mode = atoi(argv[1]);
	
	
		if(mode == 0) {
			stat(argv[2],&buf);
			mode = buf.st_mode;
		
			printf("%d\n",mode);

			if(argv[1][1] == '+') {
				if(argv[1][0] == 'o') {
					count = 1;
					add_mode(mode,argv[1][3],count);
				} else if (argv[1][0] == 'g') {
					count = 10;
					add_mode(mode,argv[1][3],count);
				} else if (argv[1][0] == 'o') {
					count = 100;
					add_mode(mode,argv[1][3],count);
				} else if (argv[1][0] == 'a') {
					count = 1;
					add_mode(mode,argv[1][3],count);
					count = 10;
					add_mode(mode,argv[1][3],count);
					count = 100;
					add_mode(mode,argv[1][3],count);
				}
			} else if(argv[1][1] == '-') {
				if(argv[1][0] == 'o') {
					count = 1;
					sub_mode(mode,argv[1][3],count);
				} else if (argv[1][0] == 'g') {
					count = 10;
					sub_mode(mode,argv[1][3],count);
				} else if (argv[1][0] == 'o') {
					count = 100;
					sub_mode(mode,argv[1][3],count);
				} else if (argv[1][0] == 'a') {
					count = 1;
					sub_mode(mode,argv[1][3],count);
					count = 10;
					sub_mode(mode,argv[1][3],count);
					count = 100;
					sub_mode(mode,argv[1][3],count);
				}	
			}
		}


		/*判断mode是否符合要求*/
		if(mode < 0 || mode > 777) {
			printf("%d\n",mode);
			printf("mode is wrong\n");
			exit(0);
		}

		/*chmod()识别八进制，转换进制*/
		mode_u = mode/100;
		mode_g = (mode/10)%10;
		mode_o = mode%10;
		mode = (mode_u * 8 * 8) + (mode_g * 8) + mode_o;

		file_name = argv[2];

		if(chmod(file_name,mode) == -1) {
			perror("chmod fail\n");
			exit(0);
		} 
	}


	return 0;
}


