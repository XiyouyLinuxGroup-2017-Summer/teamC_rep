#include <stdio.h>
#include <pthread.h>
#define N 5
#define MAX 1000000
long long tsum[N];

void *summary(void *arg) {
	int start, end, i;
	int t = *(int *)arg;
	start = MAX / N * t;
	end = MAX / N * (t + 1);
	printf("The thread start from %d, until %d\n", start, end);

	for(i = start; i < end; i++)
		tsum[t] += i;

	free(arg);
}

int main (void) {
	int i;
	pthread_t tHandles[N];
	long long sum = 0;

	for(i = 0; i < N; i++){
		int *t = &i;
		pthread_create(tHandles[i], NULL, summary, (void *)t);		
	}

	for(i = 0; i < N; i++) {
		pthread_join(tHandles[i], NULL);
		sum += tsum[i];
	}

	printf("The result is %lld\n", sum);

	return 0;
}








#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS 4
int N;
int *X;
int gSum[NUM_THREADS];
void* Summation(void *pArg){
 int tNum= *((int *)pArg);
 int lSum=0;
 int start,end;
 int i;
 start = (N/NUM_THREADS)*tNum;
  printf("start is:%d\n",start);
 end = (N/NUM_THREADS)*(tNum+1);
 printf("end is:%d\n",end);
 if(tNum == NUM_THREADS -1)
  end =N;
 for(i=start;i<end;i++)
  lSum+=X[i];
 gSum[tNum]=lSum;
 free(pArg);
}
void initArr(){
 int i;
 puts("input array length:：");
 scanf("%d",&N);
 X=(int*)malloc(N*sizeof(int));//
 for(i=0;i<N;i++)
 {
   X[i]=i+1;
   printf("%d\t",X[i]);
 }
}
int main(void){
 int j,sum=1;
 pthread_t tHandles[NUM_THREADS];
 initArr();
 for(j=0;j<NUM_THREADS;j++ ){
  int *threadNum =malloc(4);
  *threadNum=j;
//  printf("threadNum is:%d\n",*threadNum);
  pthread_create(&tHandles[j],NULL,Summation,(void*)threadNum);
 }
 for(j=0;j<NUM_THREADS;j++){
  pthread_join(tHandles[j],NULL);
  sum+= gSum[j];
 }
 printf("the sum of array elements is %d\n",sum);
 return 0;
}