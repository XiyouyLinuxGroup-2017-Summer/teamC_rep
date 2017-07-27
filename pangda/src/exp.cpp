#include<iostream>

using namespace std;
int n__;
unsigned long long r__;
bool flag;

void dfs(unsigned long long now, int ori, int depth = 1) {
	if (now % ori == 0) {
		flag = true;
		r__ = now;
		return;
	}

	if (depth == 20 || flag)
		return;
	
	dfs(now * 10, ori, depth + 1);
	dfs(now * 10 + 1, ori, depth + 1);
}

int main() {
	while (cin >> n__, n__) {
		flag = false;
		dfs(1, n__);
		cout << r__ << endl;
	}
}