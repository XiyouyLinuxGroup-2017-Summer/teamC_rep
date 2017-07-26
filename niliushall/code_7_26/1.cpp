#include <iostream>
using namespace std;

char a[8][8];
int book[8][8];
int row[8], icol[8];
int count;

void dfs(int x, int y, int num, int n, int k){
	int next[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
	int tx, ty;

	if(num == k){
        count++;
		return;
	}

	if(a[x][y] == '#' && !row[x] && !col[y]){
		num++;
		row[x] = 1;
		row[y] = 1;
	}
	book[x][y] = 1;
 
	for(int i = 0; i < 4; i++){
		tx = x + next[i][0];
		ty = y + next[i][1];
printf("tx %d %d\n", tx, ty);
		if(tx >= n || tx < 0 || ty >= n || ty < 0 || book[tx][ty])
			;
		else{
			dfs(tx, ty, num, n, k);
			book[tx][ty] = 0;
			row[tx] = 0;
			col[ty] = 0;
		}
	}
	return ;
}

int main(){
	int i, j;
	int n, k;
	while(cin >> n >> k && (n != -1 && k != -1)){
		count = 0;
		for(i = 0; i < 8; i++)
			for(j = 0; j < 8; j++)
				a[i][j] = 0;

		for(i = 0; i < n; i++){
			for(j = 0; j < n; j++){
				cin >> a[i][j];
			}
			getchar();
		}
		dfs(0, 0, 0, n, k);
		cout << count << endl;
	}

	return 0;
}
