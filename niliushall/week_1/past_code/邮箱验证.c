/*************************************************************************
	> File Name: 1.c
	> Author: wangliang
    > Email: sdwllinux@gmail.com
	> Created Time: 2017年07月18日 星期二 11时19分39秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
int main (void)
{
    char mail[101];
    int i, x, y, flag, len, flag_1;

    while(scanf("%s", mail) != EOF){
        x = 0;
        y = 0;
        flag = 0;
        flag_1 = 0;
        len = strlen(mail);
        if(mail[0] == '@' || mail[0] == '.' || mail[len-1] == '@' || mail[len-1] == '.'){
            flag_1 = 1;
        }
        for(i = 1; i < len; i++){
            if(mail[i] == '@'){
                x++;
                flag = 1;
                if(mail[i+1] == '.' || mail[i-1] == '.'){
                    flag_1 = 1;
                }
                break;
            }
        }

        for(i++; i < len && flag; i++){
            if(mail[i] == '.'){
                y++;
            }
            if(mail[i] == '@')
                x++;
        }

        if(x != 1 || !y){
            flag_1 = 1;
        }

        if(flag_1){
            printf("NO\n");
        }
        else{
            printf("YES\n");
        }
    }

    return 0;
}
