#include<iostream>
#include<string>

using namespace std;

bool check(const string email) {
    int len = email.size();
    if (email[0] == '@' || email[0] == '.' || email[len - 1] == '@' || email[len - 1] == '.')
        return false;

    int atnum = 0, atpos = 0;
    for (int i = 0; i < len; i++) {
        if (email[i] == '@') {
            atnum++;
            atpos = i;
        }
    }
    if (atnum != 1)
        return false;
    
    for (int i = atpos; i < len - 2; i++) {
        if (email[i] == '.' && email[i - 1] != '@' && email[i + 1] != '@')
            return true;
    }
    return false;
}

int main() {
    string em;
    while (cin >> em) {
        if (check(em))
            cout << "YES" << endl;
        else
            cout << "NO" << endl;
    }
}