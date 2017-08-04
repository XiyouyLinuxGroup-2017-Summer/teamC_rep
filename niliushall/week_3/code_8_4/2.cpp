#include <iostream>
#include <cstring>
using namespace std;

int main() {
	int i, j, n, len, flag;
	char a[95];
	double sum, x, y;
	cin >> n;
	getchar();

	while(n--) {
		memset(a, 0, sizeof(a));
		sum = 0;

		cin >> a;
		len = strlen(a);

		for(i = 0; i < len; i++) {
			if(a[i] >= 'C' && a[i] <= 'O') {
				y = 0;
				flag = 0;

				switch(a[i]) {
					case 'C': x = 12.01; break;
					case 'H': x = 1.008; break;
					case 'O': x = 16.00; break;
					case 'N': x = 14.01; break;
				}

				for(j = i+1; j < i+4 && a[j] > '0' && a[j] <= '9'; j++) {
						y = y * 10 + a[j] - '0';
						flag = 1;
				}

				if(flag)
					sum += y * x;
				else
					sum += x;
			}
		}

		printf("%.3lf\n", sum);
	}

	return 0;
}