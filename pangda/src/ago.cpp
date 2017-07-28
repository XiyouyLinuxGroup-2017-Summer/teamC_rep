#include<iostream>
#include<stack>
#include<set>
#include<algorithm>

using namespace std;

const int MAXDATA = 105;
const int NOTV = -1;
const int WALL = 1000;
const int OPR[8][2] = {  0, 1,  0, -1,
						 1, 0, -1,  0,
						 1, 1,  1, -1,
						-1, 1, -1, -1 };
struct d2Pos {
	int x, y, d;
};

int M__, N__;
int vis[MAXDATA][MAXDATA];
char g[MAXDATA][MAXDATA];
stack<d2Pos> S;

void readg() {
	int R = 0;
	
	for (int i = 0; i < M__; i++) {
		for (int j = 0; j < N__; j++) {
			cin >> g[i][j];
			if (g[i][j] == '*') 
				vis[i][j] = WALL;
			else if (g[i][j] == '@') {
				vis[i][j] = NOTV;
				d2Pos t = { i, j, R++ };
				S.push(t);
			}
		}
	}
}

void Search() {
	d2Pos u, v;
	int lastok = WALL;
	while (!S.empty()) {
		u = S.top();
		S.pop();
		lastok = u.d;

		if (vis[u.x][u.y] != NOTV && vis[u.x][u.y] != lastok)
			continue;
		vis[u.x][u.y] = u.d;

		for (int i = 0; i < 8; i++) {
			v.x = u.x + OPR[i][0];
			v.y = u.y + OPR[i][1];

			if (vis[v.x][v.y] != NOTV)
				continue;
			if (v.x < 0 && v.y < 0 &&
				v.x >= M__ && v.y >= N__)
				continue;

			v.d = u.d;
			vis[v.x][v.y] = u.d;
			S.push(v);
		}
	}
}

int count() {
	set<int> t;
	for (int i = 0; i < M__; i++) {
		for (int j = 0; j < N__; j++) {
			if (vis[i][j] != WALL)
				t.insert(vis[i][j]);
		}
	}
	return t.size();
}

int main() {
	while(cin >> M__ >> N__, M__ && N__) {
		readg();
		Search();
		int r = count();
		cout << r << endl;
	}
	return 0;
}