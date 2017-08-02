#include<cstdio>
#include<cstring>
using namespace std;

int r__, c__;
char g[30][30];
bool vis[26];
int result;

const int DIR[4][2] = { -1, 0, 1, 0, 0, 1, 0, -1};

void dfs(int lastx, int lasty, int depth) {
    if (depth > result)
        result = depth;
    for (int i = 0; i < 4; i++) {
        int thisx = lastx + DIR[i][0],
            thisy = lasty + DIR[i][1];
        
        if (thisx >= r__ || thisy >= c__ || thisx < 0 || thisy < 0 || vis[g[thisx][thisy] - 'A'])
            continue;
        vis[g[thisx][thisy] - 'A'] = true;
        dfs(thisx, thisy, depth + 1);
        vis[g[thisx][thisy] - 'A'] = false;
    }
}

int main() {
    while (~ scanf("%d%d", &r__, &c__)) {
        memset(vis, 0, sizeof(vis));
        result = 0;
        for (int i = 0; i < r__; i++) {
            for (int j = 0; j < c__; j++) {
                scanf(" %c", &g[i][j]);
            }
        }
        vis[g[0][0] - 'A'] = true;
        dfs(0, 0, 1);
        printf("%d\n", result);
    }
    return 0;
}