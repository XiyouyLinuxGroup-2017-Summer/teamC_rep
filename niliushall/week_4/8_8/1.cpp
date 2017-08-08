#include <iostream>
#include <cstring>
using namespace std;

int main() {
	char a[5000][30], t[205];
	int i, j, k, len;

	while(fgets(t, 205, stdin)) {
		memset(a, 0, sizeof(a));
		k = 0;

		len = strlen(t);
		for(i = 0, j = 0; i <= len; i++) {
			if(t[i] >= 'A' && t[i] <= 'Z') {
				t[i] += 32;
				a[k][j++] = t[i];
			} else if(t[i] >= 'a' && t[i] <= 'z') {
				a[k][j++] = t[i];
			} else {
				a[k++][j] = 0;
				j = 0;
				for(; i <= len && !(t[i] >= 'A' && t[i] <= 'Z') && !(t[i] >= 'a' && t[i] <= 'z'); i++)
					;
				i--;
			}
		}
	}

	for(i = 0; i < k - 1; i++)
		for(j = i+1; j < k; j++)
			if(strcmp(a[i], a[j]) > 0) {
				strcpy(t, a[i]);
				strcpy(a[i], a[j]);
				strcpy(a[j], t);
			}

	for(i = 0; i < k ; i++) {
		for(j = 0; j < i && strcmp(a[i], a[j]); j++)
			;
		if(j == i)
			cout << a[i] << endl;
		j = 0;
	}

	return 0;
}

/*
#include <iostream>
#include <cstring>
using namespace std;

int main() {
	char a[5000][30] = {0}, t;
	int i, j, k, len;

	while(cin >> t && (t >= 'A' && t <= 'Z') && (t >= 'a' && t <= 'z')) {
		k = 0;

		if(t >= 'A' && t <= 'Z') {
			t += 32;
			a[k][j++] = t;
		} else if(t >= 'a' && t <= 'z') {
			a[k][j++] = t;
		} 
	}

	for(i = 0; i < k - 1; i++)
		for(j = i+1; j < k; j++)
			if(strcmp(a[i], a[j]) > 0) {
				strcpy(t, a[i]);
				strcpy(a[i], a[j]);
				strcpy(a[j], t);
			}

	for(i = 0; i < k ; i++) {
		for(j = 0; j < i && strcmp(a[i], a[j]); j++)
			;
		if(j == i)
			cout << a[i] << endl;
		j = 0;
	}

	return 0;
}
*/