#include <iostream>
#include <cstring>

using namespace std;

int main() {
	/*char a[100001], b[100001];
	int i, j, x ,y, len1, len2;
	while(cin >> a >> b) {
		len1 = strlen(a);
		len2 = strlen(b);
		if(strstr(a, "."))
		for(j = len1 - 1; j >= 0 && a[j] == '0'; j--)
			a[j] = 0;
		if(strstr(b, "."))
		for(y = len2 - 1; y >= 0 && b[y] == '0'; y--)
			b[y] = 0;

		if(a[j] == '.')
			a[j] = 0;
		if(b[y] == '.')
			b[y] = 0;

		
		for( i = 0, x = 0; i <= j && x <= y && a[i] == b[x]; i++, x++)
			;
		if(!strcmp(a, b))
			cout << "YES" << endl;
		else
			cout << "NO" << endl;
	}*/
	printf("%d\n", strcmp("01", "0001"));
}