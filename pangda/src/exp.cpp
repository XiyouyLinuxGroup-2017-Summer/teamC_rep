#include<iostream>

using namespace std;

int gcd(const int a, const int b) {
	return b ? gcd(b, a % b) : a;
}

int main() {
	int cas;
	cin >> cas;
	while (cas--) {
		int a, b;
		cin >> a >> b;
		int ret = b * 2;
		while (!(a % b == 0 && ret % b == 0 && gcd(a, ret) == b))
			ret += b;
		cout << ret << endl;
	}
	return 0;
}