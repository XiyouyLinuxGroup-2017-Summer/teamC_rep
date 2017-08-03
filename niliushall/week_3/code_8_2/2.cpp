#include <iostream>
#include <cstring>
using namespace std;

char a[20][20];
int m, n;
int vis[26], max1;
int tnext[4][2] = {0, 1, 1, 0, 0, -1, -1, 0};

void dfs(int x, int y, int count) {
	int tx, ty;

	if(max1 < count)
		max1 = count;

	for(int i = 0; i < 4; i++) {
		tx = x + tnext[i][0];
		ty = y + tnext[i][1];

		if( !vis[ a[tx][ty] - 'A' ] && tx >= 0 && tx < m && ty >= 0 && ty < n) {
			vis[ a[tx][ty] - 'A' ] = 1;
			dfs(tx, ty, count + 1);
			vis[ a[tx][ty] - 'A' ] = 0;
		}
	}
}

int main() {
	while(cin >> m >> n) {
		memset(a, 0, sizeof(a));
		memset(vis, 0, sizeof(vis));
		max1 = 0;

		for(int i = 0; i < m; i++)
			for(int j = 0; j < n; j++)
				cin >> a[i][j];

		vis[ a[0][0] - 'A' ] = 1;
		dfs(0, 0, 1);
		cout << max1 << endl;
	}

	return 0;
}
