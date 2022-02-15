#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <semaphore.h>
#include <string.h>
#include <sys/mman.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>
#define WAYS 3
#define MAX_STRING 100
#define CARS 5
#define TIME_IN_BRIDGE 30

int main(void){
	char sem_nomes[WAYS][MAX_STRING];
	sem_t *semaphores[WAYS];
	int i,num;
	pid_t p;
	strcpy(sem_nomes[0],"west");
	strcpy(sem_nomes[1],"east");
	strcpy(sem_nomes[2],"ocupar");
	
	semaphores[0]=sem_open(sem_nomes[0],O_CREAT|O_EXCL,0644,1);
	semaphores[1]=sem_open(sem_nomes[1],O_CREAT|O_EXCL,0644,1);
	semaphores[2]=sem_open(sem_nomes[2],O_CREAT|O_EXCL,0644,1);
	
	p=getpid();
	
	for(i=0;i<CARS;i++){
		if(p>0){
			p=fork();
		}
		if(p==0){
			srand(time(NULL)+getpid());
			num=rand()%2+1;
			if(num==1){
				sem_wait(semaphores[0]);
				while(sem_trywait(semaphores[2])==0);
				sleep(TIME_IN_BRIDGE);
				printf("Um carro passou de WEST para EAST\n");
				fflush(stdout);
				sem_post(semaphores[2]);
				sem_post(semaphores[0]);
			}else{
				sem_wait(semaphores[1]);
				while(sem_trywait(semaphores[2])==0);
				sleep(TIME_IN_BRIDGE);
				printf("Um carro passou de EAST para WEST\n");
				fflush(stdout);
				sem_post(semaphores[2]);
				sem_post(semaphores[1]);
			}
			exit(EXIT_SUCCESS);
		}
	
	
	}
	
	for(i=0;i<CARS;i++){
			wait(NULL);
	}
		
	printf("Passaram todos os carros\n");
	sem_unlink(sem_nomes[0]);
	sem_unlink(sem_nomes[1]);
	sem_unlink(sem_nomes[2]);
	return EXIT_SUCCESS;
}
