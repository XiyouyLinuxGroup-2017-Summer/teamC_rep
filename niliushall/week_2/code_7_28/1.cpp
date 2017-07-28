#include <iostream>
#include <string.h>
using namespace  std;

int book[13];
int len;
char ch[14], tmp[14];

void sort1(char a[], int len){
	int i, j;
	char t;
	for(i = 0; i < len-1; i++)
		for(j = i+1; j < len; j++)
			if(a[i] > a[j]){
				t = a[i];
				a[i] = a[j];
				a[j] = t;
			}
}

void sort(char a[], char b[]){
	int len1 = strlen(a), len2 = strlen(b);
	int i = 0, j = 0, k = 0;
	sort1(a, len1);
	sort1(b, len2);
	for( ; i < len1 && j < len2; ){
		if(a[i] - 32 >= b[j])
			ch[k++] = b[j++];
		else
			ch[k++] = a[i++];
	}
	if(i == len1)
		for( ; j < len2; j++)
			ch[k++] = b[j];
	else
		for( ; i < len1; i++)
			ch[k++] = a[i];
	ch[k] = 0;

}

void dfs(char tmp[], int num){
	if(num == len){
		cout << tmp << endl;
		return ;
	}

	for(int i = 0; i < len; i++){
		if(!book[i]){
			tmp[num] = ch[i];
			book[i] = 1;
			dfs(tmp, num + 1);
			book[i] = 0;
			while(i+1<len&&ch[i+1]==ch[i]) i++;
		}
	}
}

int main(int argc, char const *argv[])
{
	int n, i ,j;
	char t, a[14], b[14];
	cin >> n;
	getchar();
	while(n--){
		i = 0, j = 0;
		memset(book, 0, sizeof(book));
		memset(ch, 0, 14);
		memset(tmp, 0, 14);
		while(scanf("%c", &t) && t != '\n'){
			if(t >= 'a' && t <= 'z')
				a[i++] = t;
			else
				b[j++] = t;
		}
		a[i] = 0;
		b[j] = 0;
	
		sort(a, b);

		len = strlen(ch);
		dfs(tmp, 0);
	}
	
	return 0;
}
