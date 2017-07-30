#include <iostream>
#include <math.h>
using namespace std;
int n;
int a[20] = {1}, book[20];

int isPrime(int a){
	int i;
	for(i = 2; i <= sqrt(a) && a % i; i++)
		;
	if(i > sqrt(a))
		return 1;
	return 0;
}

void dfs(int num){
	if(num == n && isPrime(a[num-1] + 1) && isPrime(a[num-1] + a[num-2])){
		for( int i = 0; i < n; i++){
			if(i == n-1)
				cout << a[i];
			else
				cout << a[i] << ' ';
		}
		cout << endl;
		return ;
	}

	for(int i = 2 ; i <= n; i++){
		if(!book[i] && isPrime(a[num-1] + a[num - 2]) ){
			book[i] = 1;
			a[num] = i;
			dfs(num + 1);
			book[i] = 0;
		}
	}
}

int main(void)
{
	int cas = 0;
	while(cin >> n){
		cout << "Case " << cas + 1 << ":" << endl;
		dfs(1);
		cout << endl;
		cas++;
	}
	
	return 0;
}