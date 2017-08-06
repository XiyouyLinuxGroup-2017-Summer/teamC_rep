#include<iostream>
#include<string>
#include<cstring>
using namespace std;

int cpf(char *pstr) {
    char *p = pstr - 1;
    int m = strlen(pstr);
    int pi[100];
    pi[1] = 0;
    int k = 0;

    for (int q = 2; q <= m; q++) {
        while (k > 0 && p[k + 1] != p[q]) {
            k = pi[k];
        }
        if (p[k + 1] == p[q])
            k++;
        pi[q] = k;
    }

    for (int i = 1; i <= m; i++) {
        cout << pi[i] << ' ';
    }
}

int main() {
    cpf("ababaca");
}