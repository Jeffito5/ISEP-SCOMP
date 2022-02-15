#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h> 
#include <semaphore.h>
#include <string.h>
#include <time.h>

#define CAPACITY 300
#define SEMAPHORES 3
#define VIP 1
#define SPECIAL 2
#define NORMAL 3

typedef struct{
	int spectators;
} shared_data_type;


int main(void) {
	int data_size = sizeof(shared_data_type);
	/* criar e abre uma zona de memória partilhada */
	int open_shm = shm_open("/ex17", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
	ftruncate(open_shm,data_size);
	shared_data_type *shared_data = (shared_data_type*) mmap(NULL, data_size,
	PROT_READ|PROT_WRITE,MAP_SHARED, open_shm, 0);
	
	/* verifica se a zona de memória deu errro */
	if(open_shm == -1){
		perror("Error in shm.\n");
		return 1;
	}
	
	int i;
	/* cria um semáforo */
	sem_t *sem[SEMAPHORES];
	char* names[SEMAPHORES] =  {"sem_vip","sem_special","sem_normal"};
	for(i = 0; i < SEMAPHORES; i++){
		/* verifica se o semáforo criado deu erro */
		if ((sem[i] = sem_open(names[i], O_CREAT, 0644,CAPACITY)) == SEM_FAILED) {
			perror("Error in sem_open().\n");
			exit(1);
		}
    }
    
    /* verifica se o semáforo criado deu erro */
    sem_t *sem_full;
    if ((sem_full = sem_open("sem_full", O_CREAT, 0644,CAPACITY)) == SEM_FAILED) {
		perror("Error in sem_open().\n");
		exit(1);
	}
	
	int vip,special,normal, flag_full;
	
	sem_getvalue(sem_full, &flag_full);
	int client = (rand()%3) + 1;
	sem_wait(sem_full);
	printf("Cliente %d.\n", client);
	if(flag_full == 0){
		if(client == VIP){
			/* decrementa o semáforo */
			sem_wait(sem[VIP-1]);
			printf("VIP client waiting.\n");
		}
		else if(client == SPECIAL){
			/* decrementa o semáforo */
			sem_wait(sem[SPECIAL-1]);
			printf("Special client waiting.\n");
		}
		else if(client == NORMAL){
			/* decrementa o semáforo */
			sem_wait(sem[NORMAL-1]);
			printf("Normal client waiting.\n");
		}
		
	sem_getvalue(sem[VIP-1], &vip);
	sem_getvalue(sem[SPECIAL-1], &special);
	sem_getvalue(sem[NORMAL-1], &normal);
		
	printf("%d VIP clients waiting.\n", (CAPACITY - vip));
	printf("d Special clients waiting.\n", (CAPACITY - special));
	printf("d Normal clients waiting.\n", (CAPACITY - normal));
	
		if(client == VIP){
			shared_data->spectators++;
			printf("VIP client entered.\n");
			/* incrementa o semáforo */
			sem_post(sem[VIP-1]);
		}
		if(vip == CAPACITY && client == SPECIAL){
			shared_data->spectators++;
			printf("Special client entered.\n");
			/* incrementa o semáforo */
			sem_post(sem[SPECIAL-1]);
		}
		if(vip == CAPACITY && special == CAPACITY && client == NORMAL){
			shared_data->spectators++;
			printf("Normal client entered.\n");
			/* incrementa o semáforo */
			sem_post(sem[NORMAL-1]);
		}
	} else {
		shared_data->spectators++;		
	}
	
	for(i = 0; i < SEMAPHORES; i++){
		/* verifica se deu erro a fechar o semáforo */	
		if(sem_close(sem[i])==-1){
			perror("Error in close().\n");
			exit(1);
		}
	}
	/* verifica se deu erro a fechar o semáforo */	
	if (sem_close(sem_full) == -1){ 
        perror("Error in close().\n");
    }
    /* verifica se deu erro a fechar a memória partilhada */	
    if (close(open_shm) == -1){
        perror("Error in close().\n");
    }
	return 0;
}
