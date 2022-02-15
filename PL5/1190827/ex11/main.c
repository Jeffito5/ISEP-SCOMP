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
#define NUMBER_OF_SIGNALS 2
#define NUMBER_OF_PROVAS 300
#define FINISH_PROVA 0

int flag_signal[]={0, 0};

typedef struct{	
	int num_mecanografico;
    int nota1;
    int nota2;
    int nota3;
    int notaFinal;
} shared_data_type;

shared_data_type provas[NUMBER_OF_PROVAS];

int current_index=0;
int pos;
int neg;
time_t t;
pthread_mutex_t mutex[NUMBER_OF_SIGNALS];
pthread_cond_t sinal[NUMBER_OF_SIGNALS];

int random_number(){
	int num=(rand()%300)+1000;
	return num;
}

void *fill_provas(void *arg){
    int i;
    
    for (i=0; i<NUMBER_OF_PROVAS; i++){
		
		/* mutex é adquirido e bloqueado se uma das threads lhe deu lock */
        if (pthread_mutex_lock(&mutex[FINISH_PROVA])!=0){
            perror("Couldn't lock.\n");
        }
        
        /* é sorteado um número mecanográfico entre 1000 e 1300 para o array estrutura */
		provas[i].num_mecanografico=random_number();
		
		/* são sorteadas 3 notas de 0 a 100 para os 3 trabalhos */
        provas[i].nota1=(rand()%100);
        provas[i].nota2=(rand()%100);
        provas[i].nota3=(rand()%100);
        
        flag_signal[FINISH_PROVA]++;
		
        printf("Prova %d.\nStudent number: %d.\n",i, provas->num_mecanografico);
        
        /* é enviado um sinal para a função check_grade executar a sua função através do valor da new prova. Se esta for 1 então 
		* a função poderá calcular a nota final. A thread fica suspensa enquanto não receber o sinal */
        pthread_cond_signal(&sinal[FINISH_PROVA]);
        
        /* mutex é libertado */
        if (pthread_mutex_unlock(&mutex[FINISH_PROVA])!=0){
            perror("Couldn't unlock.\n");
        }
    }
    pthread_exit((void *)NULL);
}

void *check_grade(void *arg){
    int sum;
    int final_grade;
    
    /* enquanto há provas para verificar */
    while (current_index!=NUMBER_OF_PROVAS){
		
		 /* mutex é adquirido e bloqueado se uma das threads lhe deu lock */
        if (pthread_mutex_lock(&mutex[FINISH_PROVA])!=0){
            perror("Couldn't lock.\n");
        }
        
        /* Thread continua suspensa e à espera de um sinal enquanto que o número da new_prova não passa de 0 para 1.
		* Isto significa que a função poderá calcular a nota final */
        while (flag_signal[FINISH_PROVA]!=1){
            pthread_cond_wait(&sinal[FINISH_PROVA], &mutex[FINISH_PROVA]);
        }
        
        sum=provas[current_index].nota1+provas[current_index].nota2+provas[current_index].nota3;
        final_grade=sum/3;
       
        /* coloca a nota final na variável da estrutura */
        provas[current_index].notaFinal=final_grade;
         
        printf("Prova %d.\nStudent number: %d\n",current_index, provas->num_mecanografico);
        flag_signal[FINISH_PROVA]=0;
        current_index++;
        
        if (sum>50){
			flag_signal[FINISH_PROVA+1]++;
        } else{
			flag_signal[FINISH_PROVA+1]--;
        }
        
        pthread_cond_signal(&sinal[FINISH_PROVA+1]);
        
        /* mutex é libertado */
        if (pthread_mutex_unlock(&mutex[FINISH_PROVA])!=0){
            perror("Couldn't unlock.\n");
        }
    }
    pthread_exit((void *)NULL);
}

void *positive(void *arg){
   
	/* mutex é adquirido e bloqueado se uma das threads lhe deu lock */
	if (pthread_mutex_lock(&mutex[FINISH_PROVA+1])!=0){
		perror("Couldn't lock.\n");
	}
        
	/* Thread continua suspensa e à espera de um sinal enquanto que o número do array pos_perc não passa de 0 para 1.
	* Isto significa que há mais 1 positiva a ser contabilizada */
    while (flag_signal[FINISH_PROVA+1]!=1){
		pthread_cond_wait(&sinal[FINISH_PROVA+1], &mutex[FINISH_PROVA+1]);
	}
        
	pos++;

	flag_signal[FINISH_PROVA+1]=0;

	/* mutex é libertado */
	if (pthread_mutex_unlock(&mutex[FINISH_PROVA+1])!=0){
		perror("Couldn't unlock.\n");
	}
	
	pthread_exit((void *)NULL);
}

void *negative(void *arg){
   
	/* mutex é adquirido e bloqueado se uma das threads lhe deu lock */
	if (pthread_mutex_lock(&mutex[FINISH_PROVA+1])!=0){
		perror("Couldn't lock.\n");
	}
        
	/* Thread continua suspensa e à espera de um sinal enquanto que o número do array neg_perc não passa de 0 para -1.
	* Isto significa que há mais 1 negativa a ser contabilizada */
    while (flag_signal[FINISH_PROVA+1]!=-1){
		pthread_cond_wait(&sinal[FINISH_PROVA+1], &mutex[FINISH_PROVA+1]);
	}
        
	neg++;

	flag_signal[FINISH_PROVA+1]=0;

	/* mutex é libertado */
	if (pthread_mutex_unlock(&mutex[FINISH_PROVA+1])!=0){
		perror("Couldn't unlock.\n");
	}
	
	pthread_exit((void *)NULL);
}

int main(){
    srand((unsigned)time(&t));
    pos=0;
    neg=0;
    int i;
    
    /* inicializadas variáveis de condição e mutexes */
    for (i=0; i<NUMBER_OF_SIGNALS; i++){
        pthread_cond_init(&sinal[i], NULL);
        pthread_mutex_init(&mutex[i], NULL);
    }
    
    pthread_t thread_id[NUMBER_OF_THREADS];
	
	/* são criadas 3 threads  que irão para as funções fill_provas e check_grade */
    pthread_create(&thread_id[0], NULL, fill_provas, (void *)NULL);
    pthread_create(&thread_id[1], NULL, check_grade, (void *)NULL);
    pthread_create(&thread_id[2], NULL, check_grade, (void *)NULL);
    pthread_create(&thread_id[3], NULL, positive, (void *)NULL);
    pthread_create(&thread_id[4], NULL, negative, (void *)NULL);
	
	/* espera que as threads terminem */
    for (i=0; i<NUMBER_OF_THREADS; i++){
        pthread_join(thread_id[i], (void *)NULL);
    }
    
    int percentage_positives=(pos/NUMBER_OF_PROVAS)*100;
    int percentage_negatives=(neg/NUMBER_OF_PROVAS)*100;
    printf("Percentage of positives: %d.\nPercentage of negatives: %d.\n", percentage_positives, percentage_negatives);
    
    /* variáveis de condição e mutexes são removidos */
    for (i=0; i<NUMBER_OF_SIGNALS; i++){
        pthread_cond_destroy(&sinal[i]);
        pthread_mutex_destroy(&mutex[i]);
    }

    return 0;
}
