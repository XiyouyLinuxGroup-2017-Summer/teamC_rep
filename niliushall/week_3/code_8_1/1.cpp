#include <iostream>
#include <string.h>
#include <memory.h>
using namespace std;

int a[15][15], n;
int book[15], max1;

void dfs(int x, int time, int count) {
	if(max1 < count)
		max1 = count;
	for(int i = 1; i < n; i++) {
		if(!book[i] && a[x][i] >= time) {
			book[i] = 1;
			dfs(i, a[x][i], count + 1);
			book[i] = 0;
		}
	}
}

int main() {
	while(cin >> n) {
		memset(a, 0, sizeof(a));
		memset(book, 0, sizeof(book));
		max1 = 0;

		for(int i = 0; i < n; i++)
			for(int j = 0; j < n; j++)
				cin >> a[i][j];

		dfs(0, 0, 1);

		cout << max1 << endl;
	}
	
	return 0;
}
