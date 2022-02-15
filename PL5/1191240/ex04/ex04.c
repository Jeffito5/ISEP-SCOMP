#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define LINES 8
#define COLUMNS 8
#define THREADS_SIZE 8

typedef struct {
	int array1[LINES][COLUMNS];
	int array2[LINES][COLUMNS];
	int array3[LINES][COLUMNS];
	int line1;
}matriz;

void thread_func(void* arg){
	matriz *thread_matrix = (matriz *) arg;
	int line=thread_matrix->line1,y,i;
	
	for(i=0;i<LINES;i++){
		thread_matrix->array3[line][i]=0;
		for(y=0;y<COLUMNS;y++){
			thread_matrix->array3[line][i]+=(thread_matrix->array1[line][y])*(thread_matrix->array2[y][i]);
		}
	}
}

int main(void){
	matriz matrix;
	int i,y;
	srand(time(NULL));
	pthread_t thread_id[THREADS_SIZE];
	
	for(i=0;i<LINES;i++){
		for(y=0;y<COLUMNS;y++){
			matrix.array1[i][y]=y+1;
			matrix.array2[i][y]=y+1;
		}
	}
	
	for(i=0;i<LINES;i++){
		matrix.line1=i;
		
		pthread_create(&thread_id[i],NULL,(void *)thread_func,(void*)&matrix);
		
		pthread_join(thread_id[i],NULL);
	}
	
	for(i=0;i<LINES;i++){
		printf("%d	%d	%d	%d	%d	%d	%d	%d\n",matrix.array3[i][0],matrix.array3[i][1],matrix.array3[i][2],matrix.array3[i][3],matrix.array3[i][4],matrix.array3[i][5],matrix.array3[i][6],matrix.array3[i][7]);
	}
	

	return 0;
}
