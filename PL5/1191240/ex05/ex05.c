#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define ARRAY_SIZE 1000
#define THREAD_SIZE 3

typedef struct{
	int data[ARRAY_SIZE];
	int min;
	int max;
	int average;
	int id_thread;
}balance_thread;

int func_average(int array[]){
	int sum=0,average;
	int i;
	for(i=0;i<ARRAY_SIZE;i++){
		sum+=array[i];
	}
	average=sum/ARRAY_SIZE;
	return average;
}

int func_max(int array[]){
	int max=0;
	int i;
	for(i=0;i<ARRAY_SIZE;i++){
		if(array[i]>max){
			max=array[i];
		}
	}
	
	return max;
}

int func_min(int array[]){
	int min=INT_MAX;
	int i;
	for(i=0;i<ARRAY_SIZE;i++){
		if(array[i]<min){
			min=array[i];
		}
	}
	
	return min;
}

void thread_func(void* arg){
	balance_thread *ptr = (balance_thread *) arg;
	switch(ptr->id_thread){
		case 1:
			ptr->average=func_average(ptr->data);
			break;
		case 2:
			ptr->min=func_min(ptr->data);
			break;
		case 3:
			ptr->max=func_max(ptr->data);
			break;
		default: 
			printf("Opção indisponível.");
			break;
	}
}

int main(void){
	balance_thread balance;
	pthread_t thread_id[THREAD_SIZE];
	int i;
	
	for(i=0;i<ARRAY_SIZE;i++){
		balance.data[i]=i+1;
	}
		
	for(i=0;i<THREAD_SIZE;i++){
		balance.id_thread=i+1;
		
		pthread_create(&thread_id[i],NULL,(void *)thread_func,(void*)&balance);
		
		pthread_join(thread_id[i],NULL);
	}
	
	printf("The balance average: %d\n",balance.average);
	printf("The balance max: %d\n",balance.max);
	printf("The balance min: %d\n",balance.min);

	return 0;
}