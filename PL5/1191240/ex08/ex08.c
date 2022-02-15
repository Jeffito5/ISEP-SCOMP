#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define ARRAYS_QUANT 1000
#define THREADS_QUANT 5
#define THREADS_PRINT 200

pthread_mutex_t mutex;


typedef struct{
	int data[ARRAYS_QUANT];
	int result[ARRAYS_QUANT];
	int id_thread;
}calculations;

void thread_func(void* arg){
	calculations* cal = (calculations*) arg;
	cal->id_thread++;
	int i,id=cal->id_thread;
	
	for(i=(id-1)*THREADS_PRINT;i<id*THREADS_PRINT;i++){
		cal->result[i]=cal->data[i]*10+2;
	}
	
	pthread_mutex_lock(&mutex);
	
	printf("Thread[%d]\n",cal->id_thread);
	for(i=(id-1)*THREADS_PRINT;i<id*THREADS_PRINT;i++){
		printf("[%d]\n",cal->result[i]);
	}
	
	
	pthread_mutex_unlock(&mutex);	
	
	
	pthread_exit((void *)NULL);	
}

int main(void){
	calculations calcu;
	calcu.id_thread=0;
	pthread_t thread_id[THREADS_QUANT];
	int i;
	
    for (i=0; i<ARRAYS_QUANT; i++){
        calcu.data[i]=i;
    }
	
	for (i=0; i<THREADS_QUANT; i++){
        pthread_create(&thread_id[i], NULL, (void *)thread_func, (void *)&calcu);
    }
	
	for (i=0; i<THREADS_QUANT; i++){
        pthread_join(thread_id[i], (void *)NULL);
    }
	
	pthread_mutex_destroy(&mutex);

	return 0;
}