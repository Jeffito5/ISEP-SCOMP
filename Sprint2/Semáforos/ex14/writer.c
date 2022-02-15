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
	int fd,size_shared=sizeof(server_data); 
	/**The semaphores*/
	sem_t *semaphores[SEM_QUANT];
	char sem_names[SEM_QUANT][MAX_STRING],str[MAX_STRING];
	strcpy(sem_names[0],"/sem1");
	time_t t;
	time(&t);
	
	server_data *shared_data;

	semaphores[0]=sem_open(sem_names[0],O_CREAT);
	
	fd=shm_open("/ex14",O_RDWR,S_IRUSR|S_IWUSR);
	ftruncate(fd,size_shared);
	shared_data=(server_data *) mmap(NULL,size_shared,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	
	sem_wait(semaphores[0]);
	(shared_data->writers)++;
	
	sprintf(str,"%d ",getpid());
	strcat(str,ctime(&t));
	strcpy(shared_data->string,str);
	
	printf("Writers: %d \n",shared_data->writers);
	sleep(5);
	(shared_data->writers)--;
	sem_post(semaphores[0]);
	

	close(fd);
	munmap(shared_data,size_shared);
	sem_close(semaphores[0]);
	
	return EXIT_SUCCESS;
}
