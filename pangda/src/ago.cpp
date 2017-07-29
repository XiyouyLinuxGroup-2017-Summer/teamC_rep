#include<iostream>
#include<cstring>
#include<vector>

using namespace std;
int n__;
bool isprime[51] = {0,
					0,1,1,0,1,
					0,1,0,0,0,
					1,0,1,0,0,
					0,1,0,1,0,
					0,0,1,0,0,
					0,0,0,1,0,
					1,0,0,0,0,
					0,1,0,0,0,
					1,0,1,0,0,
					0,1,0,0,0};
bool vis[21];

void dfs(vector<int> prod, int depth) {
	if (depth == n__ && isprime[prod[n__ - 1] + 1]) {
		for (int i = 0; i < prod.size(); i++) {
			cout << prod[i];
			if (i != prod.size() - 1)
				cout << ' ';
			else
				cout << endl;
		}
		return;
	}

	for (int i = 2; i <= n__; i++) {
		int last = prod[depth - 1];

		if (!vis[i] && isprime[last + i]) {
			vis[i] = true;
			prod[depth] = i;
			dfs(prod, depth + 1);
			vis[i] = false;
		}
	}
}

int main() {
	int i = 0;
	while (cin >> n__) {
		memset(vis, 0, sizeof(vis));
		vector<int> pro;
		pro.resize(n__);
		pro[0] = 1;
		cout << "Case " << ++i << ":" << endl;
		dfs(pro, 1);
		
		cout << endl;
	}
}