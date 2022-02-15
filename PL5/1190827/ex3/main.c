#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define ARRAY_SIZE 1000
#define POSITIONS 100
#define NUMBER_OF_THREADS 10

typedef struct {
    int number;
    int position;
    int *ptr;
} shared_data_type;

void* print(void* arg) {
    shared_data_type* argument=(shared_data_type*)arg;
    int number=argument->number;
    int position=argument->position;
    int *ptr=argument->ptr;
	int i;
	
    for (i=position*POSITIONS;i<(position+1)*POSITIONS;i++) {
        if (ptr[i]==number) {
            printf("Number: %d ---> Thread number: %d\n", ptr[i], position);
            /* garante que os dados já foram escritos no disco */
            fflush(stdout);
            pthread_exit((void*)position);
        }
    }
    pthread_exit((void*)NULL);
}

int main(int argc, char* agrv[]) {
	shared_data_type data[NUMBER_OF_THREADS];
    pthread_t threads[NUMBER_OF_THREADS];
    int array2[ARRAY_SIZE];
    int number, i;
	
    printf("Enter a number to search:\n");
    scanf("%d", &number);
    printf("\n");
    /* garante que os dados já foram escritos no disco */
    fflush(stdout);
	
	/* preenche o array com números */
    for (i=0;i<ARRAY_SIZE;i++) {
        array2[i]=i;
    }
	
	/* coloca os dados nos valores da estrutura */
    for (i=0;i<NUMBER_OF_THREADS;i++) {
		data[i].number=number;
        data[i].position=i;
		data[i].ptr=array2;
		/* cria 10 threads que vão executar a função print sendo o argumento o array data */
        pthread_create(&threads[i], NULL, print, (void*)&data[i]);
    }
	
	/* espera que cada thread termine */
    for (i=0;i<NUMBER_OF_THREADS;i++) {
        int ret;
        pthread_join(threads[i], (void*)&ret);
        printf("Return value of the thread that found the number: %d\n", ret);
    }

    return EXIT_SUCCESS;
}
