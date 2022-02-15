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

void* checkPrimeAndPrint(void* arg) {
    int number = *((int*)arg);
    printf("All prime numbers less than %d:\n", number);
    /* garante que os dados já foram escritos no disco */
    fflush(stdout);
	int i, j, prime;
		
	for(i=2; i<=number; i++){
        /* assume-se que o número atual é primo */
        prime=1; 

        /* verifica se o número atual é primo */
        for(j=2; j<=i/2; j++){
			
            /* se o i for divisível por outro qualquer número sem ser por 1 ou por ele próprio então não é primo */
            if(i%j==0){
                prime=0;
                break;
            }
        }

        /* Se o número for primo então dá print */
        if(prime==1){
            printf("%d\n", i);
        }
    }
    
    pthread_exit((void*)NULL);
}

int main(int argc, char* agrv[]) {
    pthread_t thread;
    unsigned int number;
    
    printf("Write a number that will be the limit of the prime numbers:\n");
    scanf("%u", &number);
    /* garante que os dados já foram escritos no disco */
    fflush(stdout);
	
	/* cria uma thread que irá executar a função print sendo o argumento o número indicado */
    pthread_create(&thread, NULL, checkPrimeAndPrint, (void*)&number);
    /* espera que a thread termine */
    pthread_join(thread, NULL);

    return EXIT_SUCCESS;
}
