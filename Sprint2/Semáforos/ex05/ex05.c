#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h> 
#include <semaphore.h>

int main(void){
	pid_t p;
	sem_t *sem_father,*sem_son;
	
	if((sem_father=sem_open("/sem_father",O_CREAT,0644,1))==SEM_FAILED||(sem_son=sem_open("/sem_son",O_CREAT,0644,0))==SEM_FAILED){
		perror("Error in sem_open()");
		exit(EXIT_FAILURE);
	}
	
	if((p=fork())==-1){
		perror("Error in fork()");
		exit(EXIT_FAILURE);
	}
	
	if(p>0){
		if(sem_wait(sem_father)==-1){
			perror("Error in sem_wait().\n");
			exit(1);
		}
		printf("I'm the father\n");	
		if(sem_post(sem_son)==-1){
			perror("Error in sem_post().\n");
			exit(1);
		}	
	}else{
		if(sem_wait(sem_son)==-1){
			perror("Error in sem_wait().\n");
			exit(1);
		}
		printf("I'm the child\n");
		exit(0);
	}
	
	wait(NULL);
	
	if(sem_unlink("/sem_father")==-1||sem_unlink("/sem_son")==-1){
		perror("Error in sem_unlinkh().\n");
		exit(EXIT_FAILURE);
	}
	
	
	return EXIT_SUCCESS;
}
