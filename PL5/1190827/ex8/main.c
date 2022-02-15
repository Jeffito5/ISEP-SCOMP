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

#define NUMBER_OF_THREADS 5
#define ARRAY_SIZE 1000

int data[ARRAY_SIZE];
int result[ARRAY_SIZE];
pthread_mutex_t mutex;
pthread_cond_t switch_thread;
int current_index;

void *fill_result(void *arg){
    int *index = (int *)arg; 
    int i;
	
	/* cada uma das threads irá verificar 300 valores. Mediante o índice do array (index=0 -> thread 0, etc)
	 * cada uma irá verificar 300 */
    for (i=*index*(ARRAY_SIZE/NUMBER_OF_THREADS); i<(*index + 1)*(ARRAY_SIZE/NUMBER_OF_THREADS); i++){
        result[i] = data[i]*10 + 2;
    }
	
	/* mutex é adquirido e bloqueado se uma das threads lhe deu lock. O array com o número de vezes que cada número
    * foi escolhido irá ser utilizado */
    if (pthread_mutex_lock(&mutex)!=0){
        perror("Couldn't lock.\n");
    }
	
	/* variável de condição é utilizada aqui para haver uma troca de threads na realização do preenchimento do array data.
	 * Enquanto que este índice for diferente do atual a condição de troca irá ficar em wait até ser recebido um sinal. Enquanto
	 * não for recebido a thread ficará suspensa */
    while (*index!=current_index){
        pthread_cond_wait(&switch_thread, &mutex);
        pthread_cond_signal(&switch_thread);
    }
	
	printf("\n");
	printf("RESULTS -> 12\n");
	printf("New thread\n");
	printf("\n");
	
	/* são impressos os resultados */
    for (i=*index*(ARRAY_SIZE/NUMBER_OF_THREADS); i<(*index+1)*(ARRAY_SIZE/NUMBER_OF_THREADS); i++){
        printf("Index %d --> Number %d\n", i, result[i]);
        /* garante que os dados já foram escritos no disco */
        fflush(stdout);
    }
	
    current_index++;
    /* sinal que ativa a condição */
    pthread_cond_signal(&switch_thread);
	
	/* mutex é libertado */
    if (pthread_mutex_unlock(&mutex)!=0){
        perror("Couldn't unlock.\n");
    }
	
    pthread_exit((void *)NULL);
}

int main(){
    int i;
	
	/* preenche o array data todo a 1 */
    for (i=0; i<ARRAY_SIZE; i++){
        data[i]=1;
    }
	
	/* inicializada uma variável de condição */
    pthread_cond_init(&switch_thread, NULL);
    
    /* inicializado um mutex */
    pthread_mutex_init(&mutex, NULL);
    
    /* index atual é inicializado a 0 */
    current_index=0;
    pthread_t thread_id[NUMBER_OF_THREADS];
    int position[NUMBER_OF_THREADS];
	
	/* são criadas 5 threads que irão para a função fill_result sendo o argumento o array positions que serve 
	 * para que cada uma das 5 threads cumpra a sua função */
    for (i=0; i<NUMBER_OF_THREADS; i++){
        position[i]=i;
        pthread_create(&thread_id[i], NULL, fill_result, (void *)&position[i]);
    }
	
	/* espera que as threads terminem */
    for (i=0; i<NUMBER_OF_THREADS; i++){
        pthread_join(thread_id[i], (void *)NULL);
    }
	
	/* variável de condição é removida */
    pthread_cond_destroy(&switch_thread);
    
    /* mutex é removido */
    pthread_mutex_destroy(&mutex);

    return 0;
}
