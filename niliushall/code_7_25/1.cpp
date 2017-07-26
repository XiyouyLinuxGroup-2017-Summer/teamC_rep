#include <iostream>
using namespace std;

int gcd(int a, int b){
    int x;
    if(a < b){
        x = a;
        a = b;
        b = x;
    }
    while(x = a % b){
        a = b;
        b = x;
    }
    return b;
}

int main() {
    int n, a, b, c;
    cin >> n;
    while(n--){
        cin >> a >> b;
        c = b * 2;
        while(gcd(a, c) != b)
            c += b;
        cout << c << endl;
    }

    return 0;
}