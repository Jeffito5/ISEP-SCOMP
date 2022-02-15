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

#define NUMBER_OF_THREADS 6
#define FILTERING_THREADS 3
#define NUMBER_OF_SIGNALS 3
#define NUMBER_OF_HYPERMARKETS 3
#define NUMBER_OF_PRODUCTS 5
#define MAX_PRICE 100
#define MIN_PRICE 5
#define ARRAY_SIZE 10000

int flag_signal[]={0, 0, 0};

typedef struct{
    int id_h;
    int id_p;
    int product_price;
} shared_data_type;

time_t t;
int lowest_cost=INT_MAX;
int hypermarket_id=0;

shared_data_type vec[ARRAY_SIZE];
shared_data_type vec1[ARRAY_SIZE];
shared_data_type vec2[ARRAY_SIZE];
shared_data_type vec3[ARRAY_SIZE];
pthread_mutex_t mutex[NUMBER_OF_SIGNALS];
pthread_cond_t sinal[NUMBER_OF_SIGNALS];

void fill_array(){
    int i;
    for (i=0; i<ARRAY_SIZE; i++){
        vec[i].id_h=(rand()%NUMBER_OF_HYPERMARKETS)+1;
        vec[i].id_p=(rand()%NUMBER_OF_PRODUCTS)+1;
        vec[i].product_price=(rand()%(MAX_PRICE-MIN_PRICE+1))+MIN_PRICE;
    }
}

void *filtering(void *arg){
    int index=(int)arg;
    
    /* mutex é adquirido e bloqueado se uma das threads lhe deu lock */
    if (pthread_mutex_lock(&mutex[index - 1])!=0){
        perror("Couldn't lock.\n");
    }
    
    shared_data_type *vec_aux;
    /* se o index corresponder ao hipermercado 1 então o pointer de estrutura irá corresponder ao array de estrutura criado antes */
    if (index==1){
        vec_aux=vec1;
    }
    
    /* se o index corresponder ao hipermercado 2 então o pointer de estrutura irá corresponder ao array de estrutura criado antes */
    if (index==2){
        vec_aux=vec2;
    }
    
    /* se o index corresponder ao hipermercado 3 então o pointer de estrutura irá corresponder ao array de estrutura criado antes */
    if (index==3){
        vec_aux=vec3;
    }

    int aux=0;
    int i;
    for (i=0; i<ARRAY_SIZE; i++){
		/* se o id do vetor de estrutura inicial corresponder ao index então as informações do hipermercado do mesmo vetor inicial
		 * serão passadas para o vetor auxiliar */ 
        if (vec[i].id_h==index){
            vec_aux[aux].id_h=vec[i].id_h;
            vec_aux[aux].id_p=vec[i].id_p;
            vec_aux[aux].product_price=vec[i].product_price;
        }
        aux++;
    }
    
    /* incremento da flag_signal para ativar o sinal */
    flag_signal[index-1]++;
    
    /* é enviado um sinal para a função computing executar a sua função através do valor da flag_signal. Se esta for 1 então 
     * a função poderá analisar os dados. A thread fica suspensa enquanto não receber o sinal */
    pthread_cond_signal(&sinal[index-1]);
    
    /* mutex é libertado */
    if (pthread_mutex_unlock(&mutex[index-1])!=0){
        perror("Couldn't unlock.\n");
    }

    pthread_exit((void *)NULL);
}

void *computing(void *arg){
    int index=(int)arg;
    
    /* mutex é adquirido e bloqueado se uma das threads lhe deu lock */
    if (pthread_mutex_lock(&mutex[index - 1])!=0){
        perror("Couldn't lock.\n");
    }
    
    /* Thread continua suspensa e à espera de um sinal enquanto que o número do array flag_signal não passa de 0 para 1.
     * Isto significa que os dados de cada hipermercado passaram para os seus arrays respetivos */
    while (flag_signal[index - 1]!=1){
        pthread_cond_wait(&sinal[index-1], &mutex[index-1]);
    }

    shared_data_type *vec_aux;
    /* se o index corresponder ao hipermercado 1 então o pointer de estrutura irá corresponder ao array de estrutura criado antes */
    if (index==1){
        vec_aux=vec1;
    }
    
    /* se o index corresponder ao hipermercado 2 então o pointer de estrutura irá corresponder ao array de estrutura criado antes */
    if (index==2){
        vec_aux=vec2;
    }
    
    /* se o index corresponder ao hipermercado 3 então o pointer de estrutura irá corresponder ao array de estrutura criado antes */
    if (index==3){
        vec_aux=vec3;
    }
    
    /* vetor da soma dos produtos */
    int price_sum[NUMBER_OF_PRODUCTS];
    
    /* vetor da quantidade de produtos */
    int count_prod[NUMBER_OF_PRODUCTS];
    
    int k;
	
	/* inicializar os vetores a 0 */
    for (k=0; k<NUMBER_OF_PRODUCTS+1; k++){
        price_sum[k]=0;
        count_prod[k]=0;
    }

    int i;
    for (i=0; i<ARRAY_SIZE; i++){
        for (k=1; k<NUMBER_OF_PRODUCTS+1+1; k++){
            if (vec_aux[i].id_p==k){
                price_sum[k-1]+=vec_aux[i].product_price;
                count_prod[k-1]++;
                break;
            }
        }
    }
	
	/* vetor da média do preço de cada produto */
    int avg_prod[NUMBER_OF_PRODUCTS];
    /* variável da soma da média de cada produto */
    int sum_prod=0;

    for (k=1; k<NUMBER_OF_PRODUCTS+1; k++){
        avg_prod[k]=price_sum[k]/count_prod[k];
        sum_prod+=avg_prod[k];
    }
    
    index=(int)arg;
    /* saber qual é o hipermercado com a soma da média de preço de cada produto mais baixa */
    if (sum_prod<lowest_cost){
        lowest_cost=sum_prod;
        hypermarket_id=index;
    }
	
	/* mutex é libertado */
    if (pthread_mutex_unlock(&mutex[index-1])!=0){
        perror("Couldn't unlock.\n");
    }
    pthread_exit((void *)NULL);
}

int main(){
    srand((unsigned)time(&t));
    fill_array();
    int i;
    
    /* inicializadas variáveis de condição e mutexes */
    for (i=0; i<NUMBER_OF_SIGNALS; i++){
        pthread_cond_init(&sinal[i], NULL);
        pthread_mutex_init(&mutex[i], NULL);
    }
    
    pthread_t thread_id[NUMBER_OF_THREADS];
    int position[FILTERING_THREADS];
	
	/* são criadas 3 threads iniciais que irão para a função filtering sendo o argumento o array positions que serve 
	 * para que cada uma das 3 threads cumpra a sua função */
    for (i=0; i<FILTERING_THREADS; i++){
        position[i]=i+1;
        pthread_create(&thread_id[i], NULL, filtering, (void *)position[i]);
    }
	
	/* são criadas mais 3 threads que irão para a função computing sendo o argumento o array positions que serve 
	 * para que cada uma das 3 threads cumpra a sua função */
    int position2[NUMBER_OF_THREADS-FILTERING_THREADS];
    for (i=FILTERING_THREADS; i<NUMBER_OF_THREADS; i++){
        position2[i-FILTERING_THREADS]=i-FILTERING_THREADS+1;
        pthread_create(&thread_id[i], NULL, computing, (void *)position2[i-FILTERING_THREADS]);
    }
    
    /* espera que as threads terminem */
    for (i=0; i< NUMBER_OF_THREADS; i++){
        pthread_join(thread_id[i], (void *)NULL);
    }
	
	/* variáveis de condição e mutexes são removidos */
    for (i=0; i<NUMBER_OF_SIGNALS; i++){
        pthread_cond_destroy(&sinal[i]);
        pthread_mutex_destroy(&mutex[i]);
    }

    printf("The hypermarket with the lowest prices: Hypermarket %d.\nSum of the average price of each product: %d.\n", hypermarket_id, lowest_cost);

    return 0;
}
