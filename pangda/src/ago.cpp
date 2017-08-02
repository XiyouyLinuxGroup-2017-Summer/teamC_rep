#include<iostream>
using namespace std;

struct stud {
    int a, b;
    int now;
    bool awake;
} stu[15];

int n__;
int sleep;

void time_passed(stud &ts) {
    ts.now++;
    if (ts.now <= ts.a) {
        ts.awake = true;
    } else {
        if ((ts.now == ts.a + 1 && sleep * 2 <= n__) || ts.now == ts.a + ts.b + 1) {
            ts.awake = true;
            ts.now = 1;
        } else {
            ts.awake = false;
        }
    }
}

int main() {
    int casno = 0;
    while (cin >> n__, n__) {
        casno++;
        sleep = 0;
        bool flag = true;
        for (int i = 0; i < n__; i++) {
            cin >> stu[i].a >> stu[i].b >> stu[i].now;
            if (stu[i].now <= stu[i].a)
                stu[i].awake = true;
            else
                stu[i].awake = false;
            if (!stu[i].awake)
                sleep++;
        }
        if (sleep == 0) {
            flag = false;
            cout << "Case " << casno << ": 1" << endl;
        }
        for (int t = 1; t < 100000 && flag; t++) {
            int ns = 0;
            for (int i = 0; i < n__; i++) {
                time_passed(stu[i]);
                if (!stu[i].awake)
                    ns++;
            }
            if (ns == 0) {
                cout << "Case " << casno << ": " << t + 1 << endl;
                flag = false;
                break;
            }
            sleep = ns;
        }
        if (flag)
            cout << "Case " << casno << ": -1" << endl;

    }
}