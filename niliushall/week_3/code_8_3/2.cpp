#include <iostream>
using namespace std;
int main() {
	int n, x, count;
	int a[6] = {100, 50, 10, 5, 2, 1};
	while(cin >> n && n) {
		count = 0;

		for(int i = 0; i < n; i++) {
			cin >> x;
			for(int i= 0; i < 6; i++) {
				count += x / a[i];
				x %= a[i];
			}
		}

		cout << count << endl;
	}
}