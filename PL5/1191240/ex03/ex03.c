#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define ARRAY_SIZE 1000
#define THREADS_SIZE 10

typedef struct{
	int id_thread;
	int num_read;
	int *ptr;
	int exists;
}thread_data;

void thread_func(void* arg){
	thread_data *thr_ptr = (thread_data *) arg;
	
	int i,id=thr_ptr->id_thread,num=thr_ptr->num_read;
	int *array_ptr=thr_ptr->ptr;
	for(i=(id-1)*100;i<id*100;i++){
		if(array_ptr[i]==num){
			printf("Number %d founded in #%d thread\n",array_ptr[i],id);
			thr_ptr->exists=1;
		}
	}
}

int main(void){
	thread_data data;
	pthread_t thread_id[THREADS_SIZE];
	int nume_read,i,array_int[ARRAY_SIZE];
	
	for(i=0;i<ARRAY_SIZE;i++){
		array_int[i]=i+1;
	}
	printf("Type a number to see if exists in the array.\n");
	scanf("%d",&nume_read);
	data.num_read=nume_read;
	data.ptr=&array_int[0];
	
	data.exists=0;
	
	for(i=0;i<THREADS_SIZE;i++){
		data.id_thread=i+1;
		pthread_create(&thread_id[i],NULL,(void *)thread_func,(void*)&data);
		
		pthread_join(thread_id[i],NULL);
	}
	
	if(data.exists==0){
		printf("The number not founded.\n");
	}

	return 0;
}