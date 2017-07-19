#include<iostream>
#include<cstdio>
using namespace std;

int main() {
	int cas__;
	cin >> cas__;
	getchar();
	while (cas__--) {
        wchar_t t;
		int cnt = 0;
		while ((t = getwchar()) != '\n') {
			if (t > 128)
				cnt++;
		}
		cnt /= 2;
		cout << cnt << endl;
	}
	return 0;
}w