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
#define NUMBER_OF_THREADS 3
#define MIN_BALANCE 1000
#define MAX_BALANCE 5000

typedef struct {
    int* array;
    int value;
    char balance[255];
} shared_data_type;

int randomBalance(int min, int max) {
    return (rand()%max)+min;
}

void* lowestBalance(void* arg) {
	/* argument será o array com os valores das contas sorteados */
    shared_data_type* argument=(shared_data_type*)arg;
    int* array=argument->array;
    int lowest=MAX_BALANCE;
    int i, balance;
    
    for (i=0; i<ARRAY_SIZE; i++) {
        balance=array[i];
        if (balance<lowest) {
            lowest=balance;
        }
    }
    argument->value=lowest;
    pthread_exit((void*)NULL);
}

void* higestBalance(void* arg) {
	/* argument será o array com os valores das contas sorteados */
    shared_data_type* argument=(shared_data_type*)arg;
    int* array=argument->array;
    int highest=MIN_BALANCE;
    int i, balance;
    
    for (i=0; i<ARRAY_SIZE; i++) {
        balance=array[i];
        if (balance>highest) {
            highest=balance;
        }
    }
    argument->value=highest;
    pthread_exit((void*)NULL);
}

void* averageBalance(void* arg) {
	/* argument será o array com os valores das contas sorteados */
    shared_data_type* argument=(shared_data_type*)arg;
    int* array=argument->array;
    int sum=0;
	int i;
	
    for (i=0; i<ARRAY_SIZE; i++) {
        sum=sum+array[i];
    }

    int average=sum/ARRAY_SIZE;
    argument->value=average;
    pthread_exit((void*)NULL);
}

int main(int argc, char* agrv[]) {
    srand(time(NULL));
    pthread_t threads[NUMBER_OF_THREADS];
    shared_data_type threadsData[NUMBER_OF_THREADS];
    /* array para guardar os valores em cada conta do banco */
    int balancesArray[ARRAY_SIZE];
	int i, j, k;
	
	/* preenche o array com valores na conta entre os 1000 e os 5000€ */
    for (i=0; i<ARRAY_SIZE; i++) {
        balancesArray[i]=randomBalance(MIN_BALANCE, MAX_BALANCE);
    }

	/* para cada thread, o array da estrutura será igual ao array auxiliar criado para colocar os valores das contas */
    for (j=0; j<NUMBER_OF_THREADS; j++) {
        threadsData[j].array=balancesArray;
    }
    
    /* nome de cada uma das contas que as threads encontrarem */
    strcpy(threadsData[0].balance, "lowest");
    strcpy(threadsData[1].balance, "highest");
    strcpy(threadsData[2].balance, "average");
	
	/* thread[0] irá ao método lowestBalance encontrar o menor valor das contas a partir do array com os valores sorteados */
    pthread_create(&threads[0], NULL, lowestBalance, (void*)&threadsData[0]);
    /* thread[1] irá ao método higestBalance encontrar o maior valor das contas a partir do array com os valores sorteados */
    pthread_create(&threads[1], NULL, higestBalance, (void*)&threadsData[1]);
     /* thread[2] irá ao método averageBalance encontrar a média do valor das contas a partir do array com os valores sorteados */
    pthread_create(&threads[2], NULL, averageBalance, (void*)&threadsData[2]);
	
	/* espera que cada thread termine */
    for (k=0; k<NUMBER_OF_THREADS; k++) {
        pthread_join(threads[k], NULL);
        printf("A thread found the %s balance: %d€.\n", threadsData[k].balance, threadsData[k].value);
    }

    return EXIT_SUCCESS;
}
