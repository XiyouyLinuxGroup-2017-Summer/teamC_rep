#include <iostream>
#include <cstring>
using namespace std;
int main() {
	int n, i, j, max, x;
	char a[1000][16];
	int num[1000];

	while(cin >> n && n) {
		getchar();
		max = 0;
		x = 0;

		for(i = 0; i < n; i++) {
			cin >> a[i];
			for(j = 0; j < i; j++) {
				if(!strcmp(a[i], a[j])) {
					num[j]++;
					if(max < num[j]){
						max = num[j];
						x = j;
						break;
					}
				}
			}
		}

		cout << a[x] << endl;
	}

	return 0;
}