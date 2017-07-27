#include<iostream>
using namespace std;

char a[8][8];
int col[8];
int count;

void dfs (int x, int num, int n, int k){
    if(num == k){
        count++;
        return ;
    }
    if(x >= n)
        return ;
    for(int i = 0; i < n; i++){
        if(a[x][i] == '#' && !col[i]){
            num++;
            col[i] = 1;
            dfs(x+1, num, n, k);
            col[i] = 0;
            num--;
        }
    }
    dfs(x+1, num, n, k);
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
		}
		dfs(0, 0, n, k);
		cout << count << endl;
    }
    return 0;
}
