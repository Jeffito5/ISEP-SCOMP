#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h> 
#include <time.h>
#include <semaphore.h>

#define PROCESSES 15

int main(){
	
	sem_t *sem;
	pid_t a;
	int i;

	/* cria um semáforo com o valor 0 e verifica se dá erro ao criar */
	if ((sem = sem_open("/sem_ex6", O_CREAT, 0644,0)) == SEM_FAILED) {
        perror("Error in sem_open().\n");
        exit(1);
    }

	for (i=0;i<PROCESSES; i++){
		a=fork();
		/* verifica se deu erro ao criar o fork */
		if(a<0){
			perror("Error: couldn't create the process.\n");
			exit(1);
		}
		
		/* processo filho */
		if(a == 0) {
			printf("I'm the child.\n");
			/* incrementa o semáforo */
			sem_post(sem);
			exit(0);
		}else{
			/* decrementa o semáforo */
			sem_wait(sem);
			printf("I'm the father\n");
		}
	}
	
	/* verifica se deu erro ao fechar o semáforo */
	if(sem_unlink("/sem_ex6")==-1){
		perror("Error in sem_unlink().\n");
		exit(1);
	}
	
	return 0;
}
