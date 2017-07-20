#include<stdio.h>
#define input(ret) {char c; ret = 0; while((c = getchar()) < '0' || c > '9'); while (c >= '0' && c <= '9') ret = ret * 10 + (c - '0'), c = getchar();}
int main() {
	int cas;
	input(cas);
	while (cas--) {
		char tmp, t[1001];
		int tsz = 0;
		while (tmp = getchar()) {
			if (tmp == ' ' || tmp == '\n') {
				for (int i = tsz - 1; i >= 0; i--)
					putchar(t[i]);
				tsz = 0;

				if (tmp == '\n') {
					putchar('\n');
					break;
				} else {
					putchar(' ');
				}
			} else {
				t[tsz++] = tmp;
			}
		}
	}
	return 0;
}