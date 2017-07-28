#include<iostream>
#include<cctype>
#include<algorithm>
using namespace std;

int main() {
    int cas;
    cin >> cas;
    while (cas--) {
        string st;
		cin >> st;
		auto sortcmp = [](const char a, const char b) {
			if (tolower(a) == tolower(b))
				return a < b;
			return tolower(a) < tolower(b);
		};
		sort(st.begin(), st.end(), sortcmp);

		cout << st << endl;
		while (next_permutation(st.begin(), st.end(), sortcmp)) {
			cout << st << endl;
		}
    }
    return 0;
}