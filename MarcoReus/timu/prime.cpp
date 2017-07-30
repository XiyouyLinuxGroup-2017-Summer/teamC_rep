#include <iostream>
#include <cstring>
#include <stack>

using namespace std;
int t;

stack <int> s;

int isvisited[20];

int is_Prime[] = {0,0,1,1,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0};


void dfs() {
    if(s.size() == t && is_Prime[s.top() + 1])  {
        while(s.size() != 1) {
            cout << s.top() << " ";
            s.pop();
        }
        cout << s.top() << endl;
        s.pop();
    }
    while (!s.empty()) {
        s.pop();
    }
    memset(isvisited,1,sizeof(isvisited));
    isvisited[1] = 0;
    s.push(1);
    
    while(!s.empty()) {
        int x = s.top();
        for(int i = 2;i <= t;i++) {
            
            if(isvisited[i] && is_Prime[x + i]){
                isvisited[i] = 0;
                s.push(i);
            }
        }
        isvisited[s.top()] = 1;
        s.pop();
    }
}


int main(int argc, char *argv[]) {
    int count = 1;
    while(cin >> t) {
        cout << "Case " << count << ":" << endl;
        count++;
        dfs();
        cout << endl;
    }

    return 0;
}
