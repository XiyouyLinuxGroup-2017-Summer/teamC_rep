#include<iostream>
#include<cstring>
using namespace std;

struct d2Pos {
	int x, y;
};
d2Pos result[1000];
int p__, q__;
bool vis[30][30];
bool found;

bool check(int x, int y) {
	if (x <= p__ && x > 0 && y <= q__ && y > 0 && !vis[x][y] && !found) {
		return true;
	}
	return false;
}

void dfs(d2Pos tp, int depth) {	
	result[depth] = tp;
	if (depth == p__ * q__ ) {	
		found = true;
		return;
	}

	static const int OPR[8][2] = { -1, -2,  1, -2,
								   -2, -1,  2, -1,
								   -2,  1,  2,  1,
								   -1,  2,  1,  2 };

	for (int i = 0; i < 8; i++) {
		d2Pos np = { tp.x + OPR[i][0] , tp.y + OPR[i][1] };
		if (check(np.x, np.y)) {
			vis[np.x][np.y] = true;
			dfs(np, depth + 1);
			vis[np.x][np.y] = false;
		}
	}
}


int main() {
	int cas;
	cin >> cas;
	for (int casno = 0; casno < cas; casno++) {
		found = false;
		memset(vis, 0, sizeof(vis));
		cin >> p__ >> q__;
		d2Pos temp = { 1, 1 };
		vis[1][1] = true;
		dfs(temp, 1);
		cout << "Scenario #" << casno + 1 << ":" << endl;

		if (!found) {
			cout << "impossible" << endl;
		} else {
			for (int i = 1; i <= p__ * q__; i++) {
				cout << char(result[i].y + 'A' - 1) << result[i].x;
			}
			cout << endl;
		}
		if (casno != cas - 1)
			cout << endl;
	}
	return 0;
}