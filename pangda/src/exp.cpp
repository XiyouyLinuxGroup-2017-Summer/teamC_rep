#include<cstdio>
#include<stack>

using namespace std;

int main() {
	int cas;
	scanf("%d", &cas);
	getchar();
	while (cas--) {
		stack<char> S;
		char t;
		while (t = getchar()) {
			if (t == ' ') {
				while (!S.empty()) {
					t = S.top();
					S.pop();
					putchar(t);
				}
				putchar(' ');
			} else if (t == '\n') {
				while (!S.empty()) {
					t = S.top();
					S.pop();
					putchar(t);
				}
				putchar('\n');
				break;
			} else {
				S.push(t);
			}
		}

	}
	return 0;
}