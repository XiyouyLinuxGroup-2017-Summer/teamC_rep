#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

int glb = 10;
int main() {
	pid_t pid = vfork();
	int val = 10;
	if (pid == 0) {
		val++;
		glb++;
		return 0;
	} else if (pid > 0) {
		printf("%d\n", val);
	} else {
		perror("fork");
	}
	int pp = 0;
	printf("%d", pp);
	return 0;
}
