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
#define NUMBER_OF_SIGNALS 3
#define NUMBER_OF_STATIONS 4
#define NUMBER_OF_CONNECTIONS 3
#define TRIP_DURATION 5

typedef struct{
    char station[7];
    int connections[NUMBER_OF_CONNECTIONS];
} shared_data_type;

time_t t;

shared_data_type stations[NUMBER_OF_STATIONS];
pthread_mutex_t mutex[NUMBER_OF_SIGNALS];

/* sorteia um número para saber a primeira cidade */
int first_city(int min_city, int max_city){
    return (rand()%max_city)+min_city;
}

int go_next_city(int current_city){
	/* se o número correponder à cidade A(O), C(2) ou D(3) então retorna o caminho dessas cidades até à cidade B
	 * pois é o único que podem fazer */
    if (current_city!=1){
        return stations[current_city].connections[0];
    }
    else{
		/* se o número corresponder à cidade B(1), então a próxima cidade terá de ser obtida outra vez através de um sorteio
		 * pois a cidade B tem mais do que 1 ligação */
        int next_city=first_city(1, 3);
        /* se sair 1 então retorna 0 que corresponde à cidade A */
        if (next_city==1){
            return 0;
        }
        /* se sair 2 ou 3 retorna esses valores que já são correspondentes às cidades C e D */
        else{
            return next_city;
        }
    }
}

/* Caminhos:
 * A -> B ou B -> A: 0
 * B -> C ou C -> B: 1
 * B -> D ou D -> B: 2
 */
int connectionCities(int starting_city, int next_city){
	/* se a cidade de partida for a cidade A e a de chegada a B então retorna 0 */
    if (starting_city==0){
        return 0;
    }
    
    /* se a cidade de partida for a cidade C e a de chegada a B então retorna 1 */
    if (starting_city==2){
        return 1;
    }
    
    /* se a cidade de partida for a cidade D e a de chegada a B então retorna 2 */
    if (starting_city==3){
        return 2;
    }
    
    /* se a cidade de chegada for a cidade A e a de partida a B então retorna 0 */
    if (next_city==0){
        return 0;
    }
    
    /* se a cidade de chegada for a cidade C e a de partida a B então retorna 1 */
    if (next_city==2){
        return 1;
    }
    
    /* se a cidade de chegada for a cidade D e a de partida a B então retorna 2 */
    if (next_city==3){
        return 2;
    }
    
    return -1;
}

void *fill_result(void *arg){
    int index=(int)arg;
    
    /* sorteia um número para saber qual será a primeira cidade */
    int starting_city=first_city(0, 3);
    
    int next_city=go_next_city(starting_city);
    printf("Thread %d: Starting in city %d and going to city %d.\n", index, starting_city, next_city);
    int count=0;
    while (count<4){
        int connection=connectionCities(starting_city, next_city);
		
		/* mutex é adquirido e bloqueado se uma das threads lhe deu lock */
        if (pthread_mutex_lock(&mutex[connection])!=0){
            perror("Couldn't lock.\n");
        }

        printf("Train %d: connection %d.\n", index, connection);
        sleep(TRIP_DURATION);
		
		/* mutex é libertado */
        if (pthread_mutex_unlock(&mutex[connection])!=0){
            perror("Couldn't unlock.\n");
        }
        
        /* a cidade a qual o comboio chegou passa a ser a de partida */
        starting_city=next_city;
        next_city=go_next_city(starting_city);
        
        count++;
    }

    pthread_exit((void *)NULL);
}

int main(){
    strcpy(stations[0].station, "City A");
    strcpy(stations[1].station, "City B");
    strcpy(stations[2].station, "City C");
    strcpy(stations[3].station, "City D");
	
	/* A-O
	 * B-1
	 * C-2
	 * D-3 */
	/* City A tem uma conexão (City B) */
    stations[0].connections[0]=1;
    
    /* City B tem três conexões (City A, City B e City C) */
    stations[1].connections[0]=0;
    stations[1].connections[1]=2;
    stations[1].connections[2]=3;
    
    /* City C tem uma conexão (City B) */
    stations[2].connections[0]=1;
    
    /* City D tem uma conexão (City B) */
    stations[3].connections[0]=1;

    srand((unsigned)time(&t));

    int i;
    /* inicializado um mutex */
    for (i=0; i< NUMBER_OF_SIGNALS; i++){
        pthread_mutex_init(&mutex[i], NULL);
    }
    
    pthread_t thread_id[NUMBER_OF_THREADS];
    int position[NUMBER_OF_THREADS];
    
    /* são criadas 5 threads que irão para a função fill_result sendo o argumento o array positions que serve 
	 * para que cada uma das 5 threads cumpra a sua função */
    for (i=0; i<NUMBER_OF_THREADS; i++){
        position[i]=i;
        pthread_create(&thread_id[i], NULL, fill_result, (void *)position[i]);
    }
	
	/* espera que as threads terminem */
    for (i=0; i<NUMBER_OF_THREADS; i++){
        pthread_join(thread_id[i], (void *)NULL);
    }
    
    /* mutex é removido */
    for (i = 0; i < NUMBER_OF_SIGNALS; i++){
        pthread_mutex_destroy(&mutex[i]);
    }

    return 0;
}
