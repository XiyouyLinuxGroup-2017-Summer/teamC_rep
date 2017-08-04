#include <iostream>
#include <cstring>
using namespace std;
int main() {
	int n, i, j, len;
	char a[80];

	cin >> n;
	getchar();

	while(n--) {
		memset(a, 0, sizeof(a));
		cin >> a;
		len = strlen(a);

		for(i = 1; i <= len; i++) {
			if(len % i == 0) {
				for(j = i; j < len && a[j] == a[j%i]; j++)
					;
				if(j == len){
					cout << i << endl;
					break;
				}
			}
		}

		if(n > 0)
			cout << endl;
	}

	return 0;
}