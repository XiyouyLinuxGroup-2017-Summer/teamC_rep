#include<cstdio>
#include<cstring>

using namespace std;

int main() {
    int n;
    int casno = 0;
    while (casno++, ~scanf("%d", &n)) {
        int seq[19];
        for (int i = 1; i <= n; i++) {
            scanf("%d", &seq[i]);
        }
       
        long long a[19][19];
        memset(a, 0, sizeof(a));

        for (int i = 1; i <= n; i++) {
            long long last = 1;
            for (int j = 1; j <= n - i + 1; j++) {
                a[i][j] = last * seq[i + j - 1];
                last = a[i][j];
            }
        }

        long long maxx = 0;
        for (int i = 0; i <= n; i++) {
            for (int j = 0; j<= n; j++) {
                if (a[i][j] > maxx)
                    maxx = a[i][j];
            }
        }

        printf("Case #%d: The maximum product is %lld.\n\n", casno, maxx);
    }
    return 0;
}