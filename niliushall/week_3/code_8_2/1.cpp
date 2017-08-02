#include <iostream>
using namespace std;

int main() {
	int n, time, count, case_n = 1;
	int a[3], b[3], c[3];
	while(cin >> n && n) {
		for (int i = 0; i < n; ++i)
			cin >> a[i] >> b[i] >> c[i];

		for(time = 1; time < 1000000; time++) {
			count = 0;

			for(int i = 0; i < n; i++) {
				if(c[i] <= a[i])
					count++;
			}

			if(count == n)
				break;

			for(int i = 0; i < n; i++) {
				if((c[i] == b[i] + a[i]) || (a[i] == c[i] && count >= n - count))
					c[i] = 1;
				else
					c[i]++;
			}
		}

		if(time == 1000000)
			time = -1;

		cout << "Case " << case_n << ": " << time << endl;
		case_n++;
	}

	return 0;
}