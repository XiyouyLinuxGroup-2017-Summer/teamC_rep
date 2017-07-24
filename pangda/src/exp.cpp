#include<iostream>

using namespace std;
using llong = long long;

llong gcd(const llong a, const llong b) {
	return b ? gcd(b, a % b) : a;
}

int main() {

	int cas;
	cin >> cas;
	while (cas--) {
		llong nume1, nume2, deno1, deno2;
		char trash;
		cin >> nume1 >> trash >> deno1 >> nume2 >> trash >> deno2;
		llong f = gcd(nume1, deno1);
		nume1 /= f; deno1 /= f;
		f = gcd(nume2, deno2);
		nume2 /= f; deno2 /= f;
		llong x = (nume1 * nume2) / gcd(nume1, nume2),
			  y = gcd(deno1, deno2);
		if (x % y)
			cout << x << "/" << y << endl;
		else
			cout << x / y << endl;
	}
	return 0;
}
