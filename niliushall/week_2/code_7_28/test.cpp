#include <iostream>
#include <string.h>
#include <memory.h>
using namespace std;
char a[101][101];
int book[101][101];
int count;
int n, m;
int flag, flag1;
void dfs(int x, int y, int num){
	int next[8][2]={{0,1}, {1,1}, {1,0}, {1,-1}, {0,-1}, {-1,-1}, {-1,0}, {-1,1}};
	int tx, ty;
	if(x < 0 || x >= n || y < 0 || y >= m || book[x][y])
		return;

	if(!num && a[x][y] == '@')
			count++;

	book[x][y] = 1;

	for(int i = 0; i < 8; i++){
		tx = x + next[i][0];
		ty = y + next[i][1];
		if(a[tx][ty] == '@' && !book[tx][ty])
			dfs(tx, ty, num+1);
	}
}

int main(int argc, char const *argv[])
{
	while(cin >> n >> m && n && m){
		memset(a, 0, sizeof(a));
		memset(book, 0, sizeof(book));
		count = 0;

		for(int i = 0; i < n; i++)
			for(int j = 0; j < m; j++)
				cin >> a[i][j];
		for(int i = 0; i < n; i++)
			for(int j = 0; j < m; j++)
				if(a[i][j] == '@')
					dfs(i, j, 0);
		cout << count << endl;
	}	

	return 0;
}
