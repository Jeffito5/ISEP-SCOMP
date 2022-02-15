#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <semaphore.h>
#include <string.h>
#include <sys/mman.h>
#include <string.h>
#define MAX_STRING 200
#define MAX_WRITER 100
#define SEM_QUANT 3

typedef struct{
	char string[MAX_STRING];
	int pids[MAX_WRITER];
	int canClose;
	int writers;
} server_data;

int main(void){
	/**Numbers to inicialiaze the semaphores*/
	int i,fd,size_shared=sizeof(server_data); 
	/**The semaphores*/
	sem_t *semaphores[SEM_QUANT];
	char sem_names[SEM_QUANT][MAX_STRING];
	strcpy(sem_names[0],"/sem1");
	strcpy(sem_names[1],"/sem2");
	strcpy(sem_names[2],"/sem3");
	
	server_data *shared_data;

	semaphores[0]=sem_open(sem_names[0],O_CREAT|O_EXCL,0644,1);
	semaphores[1]=sem_open(sem_names[1],O_CREAT|O_EXCL,0644,1);
	semaphores[2]=sem_open(sem_names[2],O_CREAT|O_EXCL,0644,0);
	
	fd=shm_open("/ex14",O_CREAT|O_RDWR,S_IRUSR|S_IWUSR);
	ftruncate(fd,size_shared);
	shared_data=(server_data *) mmap(NULL,size_shared,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	shared_data->canClose=0;
	shared_data->writers=0;
	strcpy(shared_data->string,"Mais um exercicio de Scomp.");
	while(shared_data->canClose==0);
	
	
	/**Disconecta e elimina os semaphores e memória partilhada*/
	close(fd);
	shm_unlink("/ex14");
	for(i=0;i<SEM_QUANT;i++){
		if(sem_unlink(sem_names[i])==-1){
			printf("\n%d ",i+1);
			perror("Error in sem_unlink");
			return EXIT_FAILURE;
		}
	}
	
	return EXIT_SUCCESS;	
}

