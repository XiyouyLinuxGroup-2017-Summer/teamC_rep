#include <iostream>
#include <memory.h>
using namespace std;

int book[8][8];
char path[64][2];
int step, flag, p, q;
int tnext[8][2] = {{-2, -1}, {-2, 1},
				   {-1, -2}, {-1, 2},
				   {1, -2},  {1, 2},
                   {2, -1},  {2, 1}}; //按字典顺序，即查找顺序按ABC... / 123....

int check(int x, int y) {
	if(x < 0 || x >= q || y < 0 || y >= p)
		return 0;
	return 1;
}

void dfs(int x, int y, int count) {
	int tx, ty;

	if(count == step) {
		for(int i = 0; i< step; i++) {
			cout << path[i][0] << path[i][1];
			flag = 1;
		}
		cout << endl;
		return ;
	}

	for(int i = 0; i < 8 && !flag; i++) {
		tx = x + tnext[i][0];
		ty = y + tnext[i][1];

		if(check(tx, ty) && !book[tx][ty]) {
			book[x][y] = 1;
    
            path[count][0] = 'A' + tx;
            path[count][1] = '1' + ty;
			dfs(tx, ty, count + 1);
			book[x][y] = 0;
		}
	}

}

int main(void)
{
	int i, n;
	cin >> n;
	for(i = 1; i <= n; i++) {
		memset(book, 0, sizeof(book));
		memset(path, 0, sizeof(path));
		flag = 0;

		cin >> p >> q;
		step = p * q;
		path[0][0] = 'A';//一定要注意字母代表列，数字代表行
		path[0][1] = '1';

		cout << "Scenario #" << i << ":" << endl;
		dfs(0, 0, 1);

		if(!flag) {
			cout << "impossible" << endl;
		}
		if(i != n) {
			cout << endl;
		}

	}

	
	return 0;
}
