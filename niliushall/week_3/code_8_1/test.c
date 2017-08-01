#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int main(int argc, char const *argv[])
{
	pthread_join(pthread_self(), NULL);
	return 0;
}