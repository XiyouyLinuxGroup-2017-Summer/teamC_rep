/*************************************************************************
	> File Name: my_ls_2.c
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年07月20日 星期四 11时32分26秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<errno.h>
#include<pwd.h>
#include<linux/limits.h>
#include<time.h>
#include<grp.h>
#include<dirent.h>


#define PARAM_NONE 0  //r
#define PARAM_A    1  //a
#define PARAM_L    2  //l
#define PARAM_I    4  //i
#define PARAM_T    8  //t
#define PARAM_R_   16 //r
#define PARAM_R    32 //R

#define MAXROWLEN 80

int g_leavelen = MAXROWLEN;
int g_maxlen;

#define NORMAL 0
#define GREEN  1 
#define BLUE  2

void display_dir(int flag_param,char *path);

/*简单错误处理*/
void my_err(const char *err_string,int line) {
    fprintf(stderr,"line:%d ",line);
    perror(err_string);
    if(errno != 13) {
        exit(0);
    } 
}

/*颜色打印包裹函数,之区分了可执行文件，目录和普通文件*/
void printfColor(char *name,int color) {
    if(color == GREEN) {
        printf("\033[;32m %-s\033[0m" "",name);
    } else if(color == BLUE){
        printf("\033[;36m %-s\033[0m" "",name);
    } else if(color == NORMAL){
        printf(" %-s",name);
    }
}

/*
* 输出文件信息 -l
* 文件信息结构体
* 文件名
* 颜色
* */
void display_attribute(struct stat buf,char *name,int color) {
	char buf_time[32];
	struct passwd *psd;
	struct group  *grp;

    //文件类型
	if(S_ISLNK(buf.st_mode)){           //符号链接
		printf("l");            
	} else if(S_ISREG(buf.st_mode)){    //一般文件
        		printf("-");
	} else if(S_ISDIR(buf.st_mode)){    //目录文件
		printf("d");
        color = BLUE;
	} else if(S_ISCHR(buf.st_mode)){    //字符设备文件
		printf("c");
	} else if(S_ISBLK(buf.st_mode)){    //块设备文件
		printf("b");
	} else if(S_ISFIFO(buf.st_mode)){   //先进先出文件
		printf("f");
	} else if(S_ISSOCK(buf.st_mode)){   //socket
		printf("s");
	}

    //拥有者权限
    if(buf.st_mode & S_IRUSR)   
		printf("r");
	else 
		printf("-");
	if(buf.st_mode & S_IWUSR)
		printf("w");
	else 
		printf("-");
	
	if(buf.st_mode & S_IXUSR){
		printf("x");
    }
	else 
		printf("-");
	
    //组权限	
	if(buf.st_mode & S_IRGRP)
		printf("r");
	else 
		printf("-");

    if(buf.st_mode & S_IWGRP)
		printf("w");
	else 
		printf("-");
	
	if(buf.st_mode & S_IXGRP){
		printf("x");
    }
	else 
		printf("-");
	

    //其他用户权限
	if(buf.st_mode & S_IROTH)
		printf("r");
	else 
		printf("-");
	
    if(buf.st_mode & S_IWOTH)
		printf("w");
	else 
		printf("-");
	
	if(buf.st_mode & S_IXOTH){
		printf("x");
    }
	else 
		printf("-");
	

    printf("\t");	
    //通过用户和组id得到用户的信息和其所在组的信息
	psd = getpwuid(buf.st_uid);
	grp = getgrgid(buf.st_gid);

	printf("%4ld ",buf.st_nlink);    //打印文件的硬链接数
	printf("%-8s  ",psd->pw_name);    //打印用户的名字
	printf("%-8s", grp->gr_name);   //打印用户组的名字

	printf("%6ld", buf.st_size);     //打印文件大小
	strcpy(buf_time,ctime(&buf.st_mtime));  //把时间转换成普通表示格式

	buf_time[strlen(buf_time)-1] = '\0';    //去掉换行符
	printf("  %s", buf_time);               //输出时间 
    
    printfColor(name,color);                //颜色打印
    printf("\n");
}
/*
 * 没有-l时的输出
 * 参数
 * 名字
 * 颜色
 *
 */
void display_single(char *name,int color) {
    int i;
    int len;

    if(g_leavelen < g_maxlen) { //剩余空间不够时换行
        printf("\n");           
        g_leavelen = MAXROWLEN;
    }
    
    //算出空格的个数，用最大长度对齐
    len = strlen(name);
    len = g_maxlen - len;
    
    printfColor(name,color);

    //打印空格
    for(i = 0;i < len;i++) {
        printf(" ");
    }    
    printf(" ");
    
    //多减去的2是空格的个数
    g_leavelen -= (g_maxlen + 2);
}
/*
* 含参数i时的打印
* 参数
* 文件名
* 颜色
* */
void display_inode(char *name,int color) {
    int i;
    int len;
    struct stat buf;

    //剩余空间不够时换行
    if(g_leavelen < g_maxlen) {
        printf("\n");
        g_leavelen = MAXROWLEN;
    }
    
    //获取文件信息
    if(lstat(name,&buf) == -1) {
        my_err("lstat",__LINE__);
    }

    //打印inode编号
    printf("%ld ",buf.st_ino);

    //算出空格的个数
    len = strlen(name);
    len  = g_maxlen - len;

    printfColor(name,color);
    
    //打印空格
    for(i = 0;i < len;i++) {
        printf(" ");
    }
    printf(" ");

    //因为inode号和空格多减了8
    g_leavelen -= (g_maxlen + 2 + 8);
}

/*
* 显示文件函数
* 参数
* 权限
* 文件名
* 
* */

void display(int flag,char *pathname) {
    int i;
    int j;
    struct stat buf;
    char name[PATH_MAX + 1];
    int color = NORMAL;
    
    //获取文件路径
    for(i = 0,j = 0;i < strlen(pathname);i++) {
        if(pathname[i] == '/') {
            j = 0;
            continue;
        }
        name[j++] = pathname[i];
    }
    name[j] = 0;
    
    //获取文件信息
    lstat(pathname,&buf);

    //判断颜色
    if(S_ISDIR(buf.st_mode)) {
        color = BLUE;
    }

    if((buf.st_mode & S_IXUSR) && color != BLUE) {
	    color = GREEN;
    }      
    
    /*把与打印无关的参数去掉*/
    if(flag & PARAM_T) {
        flag -= PARAM_T;
    }
    if(flag & PARAM_R) {
        flag -= PARAM_R;
    }
    if(flag & PARAM_R_) {
        flag -= PARAM_R_;
    }

    /*-a显示本目录和上一级目录，-i显示node,-l显示详细信息*/
    switch(flag) {
        case PARAM_NONE:        //没有参数
            if(name[0] != '.') {
                display_single(name,color);
            }
            break;
        case PARAM_L:           //-l
            if(name[0] != '.') {
                display_attribute(buf,name,color);
            }
            break;
        case PARAM_A:           //-a
            display_single(name,color);
            break;
        case PARAM_I:           //-i
            if(name[0] != '.') {
                display_inode(name,color);
            }
	        break;
	case PARAM_A + PARAM_L:     //-la
            display_attribute(buf,name,color);
	    break;
        case PARAM_A + PARAM_I: //-ia
            display_inode(name,color);
	        break;
        case PARAM_L + PARAM_I: //-il
            if(name[0] != '.') {
                printf(" %ld",buf.st_ino);
                display_dir(flag,name);
            }
            break;
        case PARAM_A + PARAM_L + PARAM_I:   //-ial
            printf("%ld ",buf.st_ino);
            display_dir(flag,name);
            break;
        default:
            break;
    }
}
/*
* 显示目录中的文件
* 参数
* 权限
* 路径
* 
* */

void display_dir(int flag_param,char *path) {
    DIR *dir;
    struct dirent *ptr;
    int count = 0;
    struct stat buf;
    //char filename[2000][PATH_MAX + 1];
    char **filename;
    filename = (char **)malloc(sizeof(char *) * 200000);
    int t;
    for(t = 0;t < 200000;t++) {
        filename[t] = (char *)malloc(PATH_MAX+1);
    }
    
    long *filetime;
    filetime = (long *)malloc(sizeof(long) * 200000);
    char temp[PATH_MAX];
    //long filetime[2000];
    long timeTemp;
    /*用malloc来分配堆空间，堆空间可以开比较大*/
    
    dir = opendir(path);    //打开目录
    if(dir == NULL){
	    my_err("opendir",__LINE__);
    }
    /*获取目录下文件的个数，以及最大长度*/
    while((ptr = readdir(dir))!= NULL){
		if(g_maxlen < strlen(ptr->d_name))
			g_maxlen = strlen(ptr->d_name);
		count++;
	}
	
    closedir(dir);

    if(count > 200000) {
        my_err("too many files under this directory",__LINE__);
    }
    
    int i;
    int j;
    int len = strlen(path);
    
    dir = opendir(path);

    /*获取目录下文件名和时间参数*/
    for(i = 0;i < count;i++) {
        ptr = readdir(dir);
        if(ptr == NULL) {
            my_err("readdir",__LINE__);
        }
        strncpy(filename[i],path,len);
        filename[i][len] = 0;
        strcat(filename[i],ptr->d_name);
        filename[i][len + strlen(ptr->d_name)] = 0;
        lstat(filename[i],&buf);
        filetime[i] = buf.st_mtime;
    }
    
    closedir(dir);
    
   //排序
    if(flag_param & PARAM_T){                    //含t按时间排序
    	for(i = 0;i < count - 1;i++) {
			for(j = 0;j < count - 1 -i;j++) {
				if(filetime[j] < filetime[j+1]) {
					timeTemp = filetime[j];
					filetime[j] = filetime[j+1];
					filetime[j+1] = timeTemp;
					strcpy(temp,filename[j]);
					strcpy(filename[j],filename[j+1]);
					strcpy(filename[j+1],temp);
				}
			}
		}
    } else {                                    //按文件名排序
    	for(i = 0;i < count - 1;i++) {
			for(j = 0;j < count - 1 -i;j++) {
				if(strcmp(filename[j],filename[j+1]) > 0) {
					strcpy(temp,filename[j]);
					strcpy(filename[j],filename[j+1]);
					strcpy(filename[j+1],temp);
				}
			}
		}
    }
    
    if(flag_param & PARAM_R) {                  //如果含r倒序输出
         if(flag_param & PARAM_R_) {             
            for(i = count - 1;i >= 0;i--) {         
                display(flag_param,filename[i]); 
            }
            for(i = count - 1;i >= 0;i--) {             
                if((stat(filename[i],&buf)) == -1) {        
                    my_err("stat",__LINE__);
                }
                //判断文件是否是目录
                if(S_ISDIR(buf.st_mode)) {
                    len = strlen(filename[i]);
                    //.和..不能自递归
                    if(filename[i][len-1] == '.' && filename[i][len-2] == '/' || 
            filename[i][len-1] == '.' && filename[i][len-2] == '.' && filename[i][len-3] == '/') {
                        continue;
                    }
                    printf("\n\n%s:",filename[i]);
                    //如果目录最后忘记了/要加上/
                    if(filename[i][len-1] != '/') {
                        filename[i][len] == '/';
                        filename[i][len+1] == 0;
                    }
                    display(flag_param,filename[i]);     
                }
            }
        } else {
            for(i = 0;i < count;i++) {
                stat(filename[i],&buf);
                //没有进行错误处理，选择直接打印，一旦错误处理之后，会导致后面无法进行
                if(S_ISDIR(buf.st_mode)) {
                    len = strlen(filename[i]);
                    if(filename[i][len-1] == '.' && filename[i][len-2] == '/' || 
            filename[i][len-1] == '.' && filename[i][len-2] == '.' && filename[i][len-3] == '/') {
                        continue;
                    }
                    printf("\n\n%s:\n",filename[i]);
                    if(filename[i][len-1] != '/') {
                        filename[i][len] == '/';
                        filename[i][len+1] == 0;
                    }
                    display_dir(flag_param,filename[i]);     
                } else {
                    display(flag_param,filename[i]);
                }
            }
        }
    }else {
         if(flag_param & PARAM_R_) {

            for(i = count - 1;i >= 0;i--) {
                display(flag_param,filename[i]);
            }
        } else {
            for(i = 0;i < count;i++) {
                display(flag_param,filename[i]);
            }
        }
    }
    
    if((flag_param & PARAM_L) == 0) {
        printf("\n");
    }
    
    for(i = 0;i < 200000;i++) {
        free(filename[i]);
    }
    
    free(filename);
    free(filetime);
}


int main(int argc, char *argv[]) {
	int i;
    int j;
    int k;
    int num;
	char path[PATH_MAX+1];
	char param[32];
	int  flag_param = PARAM_NONE;
	struct stat  buf;

	j = 0;
	num = 0;
    
    //解析其参数，以-为标志
	for(i = 1; i < argc;i++) {
		if(argv[i][0] == '-') {
			for(k = 1;k < strlen(argv[i]);k++,j++) {
				param[j] = argv[i][k];
			}
			num++;
		}
	}
    
    
    //利用位运算确定具体参数
	for(i = 0;i < j;i++) {
		if(param[i] == 'a'){
			flag_param |= PARAM_A;//利用位运算
			continue;
        }
        else if(param[i] == 'l'){
			flag_param |= PARAM_L;
			continue;
		} 
		else if(param[i] == 'i'){
			flag_param |= PARAM_I;
			continue;
		} 
		else if(param[i] == 'r'){
			flag_param |= PARAM_R_;
			continue;
		} 
		else if(param[i] == 't'){
			flag_param |= PARAM_T;
			continue;
		}else if(param[i] == 'R'){
			flag_param |= PARAM_R;
			continue;
		}    
		else{
			printf("my_ls: incalid option -%c\n", param[i]);
			exit(1);
		}
	}
    
	param[j] = 0;
    
    //默认为当前路径
	if((num+1) == argc) {
        strcpy(path,"./");
		path[2] =  0;
        display_dir(flag_param,path);
        return 0;
	}
    
	i = 1;
	do{	
        if(argv[i][0] == '-') {
			i++;
			continue;
		} else {
            //得到具体路径
			strcpy(path, argv[i]);
			if(stat(path,&buf) == -1)
				my_err("stat",__LINE__);

	    	//判断是否为目录文件	
            if(S_ISDIR(buf.st_mode)) {
                //如果目录最后忘记了/则加上
				if(path[strlen(argv[i]) - 1] != '/') {
					path[strlen(argv[i])]  = '/';
					path[strlen(argv[i])+1] = 0;
				}
				else 
					path[strlen(argv[i])] = 0;
			
                display_dir(flag_param,path);//按照目录输出
				i++;
			} else {
                //按照文件输出
				display(flag_param,path);
				i++;
			}
		}
	} while(i < argc);

    return 0;
}



