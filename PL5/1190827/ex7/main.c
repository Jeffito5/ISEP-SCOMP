#include <errno.h>
#include <fcntl.h>
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
#include <limits.h>
#include <pthread.h>
#include <assert.h>

#define NUMBER_OF_THREADS 16
#define TICKETS 8000
#define NUMBERS 5
#define MAX_NUMBERS 49

/* array com todas as apostas */
int lottery_keys[TICKETS][NUMBERS];
/* array com os números de 1 a 49 a serem escolhidos */
int number_of_picks[MAX_NUMBERS];
pthread_mutex_t mutex;

void *statistics(void *arg){
	/* pointer que aponta para o array positions */
    int *index = (int *)arg;
	int i, j;
	
	/* cada uma das threads irá verificar 500 chaves. Mediante o índice do array (index=0 -> thread 0, etc)
	 * cada uma irá verificar 500 */
    for (i= *index*(TICKETS/NUMBER_OF_THREADS); i<(*index + 1)*(TICKETS/NUMBER_OF_THREADS); i++){
        
        /* mutex é adquirido e bloqueado se uma das threads lhe deu lock. O array com o número de vezes que cada número
         * foi escolhido irá ser utilizado */
        if (pthread_mutex_lock(&mutex) != 0){
            perror("Couldn't lock.\n");
        }
        
        /* verifica qual é o número escolhido dentro do lottery_keys e incrementa esse número no índice do number_of_picks */
        for (j=0; j<NUMBERS; j++){
            number_of_picks[lottery_keys[i][j]]++;
        }
		
		/* mutex é libertado */
        if (pthread_mutex_unlock(&mutex) != 0){
            perror("Couldn't unlock.\n");
        }
    }
    pthread_exit((void *)NULL);
}

void fill_array(){
    int random_number, betNumbersCounter, betNumbersRemaining, j, i;
    time_t t;
    srand((unsigned)time(&t));
    /* vai preencher o array com 8000 tickets */
    for (j=0; j<TICKETS; j++){
		
		/* número de números jogados numa aposta inicializados a 0 */
        betNumbersCounter = 0;
        
        /* vai escolher um número de 1 a 49 e o número de números jogados não podem ser superiores a 5 */
        for (i=0; i<MAX_NUMBERS && betNumbersCounter<NUMBERS; i++){
			
			/* random_number irá começar em 49 e vai ser decrementado */
            random_number=MAX_NUMBERS-i;
            
            /* número de números que faltam ser jogados */
            betNumbersRemaining=NUMBERS-betNumbersCounter;
            
            /* se o número sorteado for menor que o número de números que faltam ser jogados
             * então preenche o array das apostas com o i+1 e incrementa o contador de números jogados
             * Como a probabilidade de o random number ser menor que o betNumbersRemaining então
             * há a possibilidade de os números maiores estarem no array */
            if (rand()%random_number<betNumbersRemaining)
                lottery_keys[j][betNumbersCounter++]=i+1;
        }
        /* se a expressão for verdadeira então não faz nada. Serve para verificar se foram jogados 5 números
         * logo após sair do segundo ciclo for */
        assert(betNumbersCounter==NUMBERS);
    }
}

int main(){
    fill_array();
    int i;
    int position[NUMBER_OF_THREADS];
    
    /* inicializa o array com os números de 1 a 49 todo a 0 */
    for (i=0; i<MAX_NUMBERS; i++){
        number_of_picks[i]=0;
    }
	
	/* é inicializado o mutex */
    pthread_mutex_init(&mutex, NULL);
    pthread_t thread_id[NUMBER_OF_THREADS];
	
	/* são criadas 16 threads que irão para a função statistics sendo o argumento o array positions que serve 
	 * para que cada uma das 16 threads cumpra a sua função */
    for (i=0; i<NUMBER_OF_THREADS; i++){   
        position[i]=i;
        pthread_create(&thread_id[i], NULL, statistics, (void *)&position[i]);
    }
	
	/* espera que as threads terminem */
    for (i=0; i<NUMBER_OF_THREADS; i++){
        pthread_join(thread_id[i], (void *)NULL);
    }
    
    /* mutex é removido */
    pthread_mutex_destroy(&mutex);
	
    for (i=0; i<MAX_NUMBERS; i++){
        printf("Number %d was chosen %d times\n", i+1, number_of_picks[i]);
    }
    return 0;
}
