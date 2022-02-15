#include "header.h"

int client(int id){
	
	const int MIN_RAND = 1;
	const int MAX_RAND = 10;
	srand((unsigned)time(NULL));
	
	/* cria o semáforo */
    sem_t *sem;
	/* verifica se o semáforo criado deu erro */
    if ((sem = sem_open(SEMAPHORE_NAME, 0)) == SEM_FAILED){
        perror("Error in sem_open().\n");
        exit(EXIT_FAILURE);
    }
    
	/* cliente espera que a memória seja libertada para comprar o bilhete */
	printf("Client %d: Waiting...\n", id);
	/* decrementa ficando em waiting */
	sem_wait(sem);
	
	/* gera o tempo que o cliente demora a ser atendido */
	sleep(rand()%(MAX_RAND-MIN_RAND+1)+MIN_RAND);
	int ticket_number = seller(id);
	if(ticket_number == -1){
		printf("Client %d: Tickets sold out.\n", id);
	}else{
		printf("Client %d: Bought the ticket %d.\n", id, ticket_number);
	}
	
	/* Incrementa o semáforo */
	sem_post(sem);
	
	/* fecha o descritor */
    if(sem_close(sem)==-1){
        perror("Error in sem_close.\n");
        exit(EXIT_FAILURE);
    }
	
	return 0;
}
