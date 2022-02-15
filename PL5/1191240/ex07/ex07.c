#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define KEYS_QUAN 8000
#define THREADS_QUANT 16
#define ARRAY_FOR 500
#define QUANT_NUM1 49
#define QUANT_NUM2 13
#define NUM_PER_KEYS 6

pthread_mutex_t mutex;

typedef struct{
	int keys[KEYS_QUAN][NUM_PER_KEYS];
	int num1[QUANT_NUM1][2];
	int num2[QUANT_NUM2][2];
	int id_thread;
}totoloto_data;

void thread_func(void* arg){
	pthread_mutex_lock(&mutex);
	totoloto_data *data=(totoloto_data *)arg;
	int y,i,id=data->id_thread;
	for(i=(id-1)*ARRAY_FOR;i<id*ARRAY_FOR;i++){
		for(y=0;y<NUM_PER_KEYS-1;y++){
			(data->num1[(data->keys[i][y])-1][1])++;
		}
		(data->num2[(data->keys[i][5])-1][1])++;
	}
	pthread_mutex_unlock(&mutex);
}

int main(void){
	pthread_t thread_id[THREADS_QUANT];
	int i,y;
	totoloto_data data;
	srand(time(NULL));
	
	for(i=0;i<QUANT_NUM1;i++){
		data.num1[i][0]=i+1;
		data.num1[i][1]=0;
	}
	
	for(i=0;i<QUANT_NUM2;i++){
		data.num2[i][0]=i+1;
		data.num2[i][1]=0;
	}
	
	for(i=0;i<KEYS_QUAN;i++){
		for(y=0;y<NUM_PER_KEYS-1;y++){
			data.keys[i][y]=rand()%49+1;
		}
		data.keys[i][5]=rand()%13+1;
	}
	
	
	for(i=0;i<THREADS_QUANT;i++){
		data.id_thread=i+1;
		pthread_create(&thread_id[i],NULL,(void *)thread_func,(void*)&data);
		pthread_join(thread_id[i],NULL);
	}
	
	printf("1-49 numbers:\n");
	
	for(i=0;i<QUANT_NUM1;i++){
		printf("Number: %d - times: %d\n",data.num1[i][0],data.num1[i][1]);
	}
	
	printf("1-13 numbers:\n");
	
	for(i=0;i<QUANT_NUM2;i++){
		printf("Number: %d - times: %d\n",data.num2[i][0],data.num2[i][1]);
	}
	
	return 0;
}