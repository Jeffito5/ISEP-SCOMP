#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <semaphore.h>
#include <string.h>
#include <sys/mman.h>
#include <time.h>
#define STRING_MAX 200
#define CAPACITY 100
#define SEM_QUANT 3

typedef struct{
	char name[STRING_MAX];
	int id_number;
	char address[STRING_MAX];
}data;

typedef struct{
	data dados[CAPACITY];
	int next_data;
	int nr_leitores;
}shared_data;

int main(void){
	char sem_names[SEM_QUANT][STRING_MAX];
	int id_usr,fd,size=sizeof(shared_data),i;
	shared_data *shared_mem;
	sem_t *semaphores[SEM_QUANT];
	
	strcpy(sem_names[0],"sem1");
	strcpy(sem_names[1],"sem2");
	strcpy(sem_names[2],"sem3");
	
	semaphores[0]=sem_open(sem_names[0],O_CREAT);
	semaphores[1]=sem_open(sem_names[1],O_CREAT);
	semaphores[2]=sem_open(sem_names[2],O_CREAT);
	
	
	fd=shm_open("/ex10",O_RDWR,S_IRUSR|S_IWUSR);
	ftruncate(fd,size);
	shared_mem=(shared_data *) mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	
	printf("Digit the id: ");
	scanf("%d",&id_usr);
	sem_wait(semaphores[0]);
	sem_post(semaphores[0]);
	
	sem_wait(semaphores[2]);
	(shared_mem->nr_leitores)++;
	if(shared_mem->nr_leitores==1)
		sem_wait(semaphores[1]);
	sem_post(semaphores[2]);
	
	int index=shared_mem->next_data;
	for(i=0;i<index;i++){
			if(id_usr==(shared_mem->dados[i].id_number)){
				printf("Name: %s\n",shared_mem->dados[i].name);
				printf("Id: %d\n",shared_mem->dados[i].id_number);
				printf("Address: %s\n",shared_mem->dados[i].address);
				
				sem_wait(semaphores[2]);
				(shared_mem->nr_leitores)--;
				if(shared_mem->nr_leitores==0)
						sem_post(semaphores[1]);
				sem_post(semaphores[2]);
				close(fd);
				munmap(shared_mem,size);
				sem_close(semaphores[0]);
				sem_close(semaphores[1]);
				sem_close(semaphores[2]);
				return EXIT_SUCCESS;
			}
	}
	printf("No data found.\n");
	sem_wait(semaphores[2]);
	(shared_mem->nr_leitores)--;
		if(shared_mem->nr_leitores==0)
			sem_post(semaphores[1]);
		sem_post(semaphores[2]);
	close(fd);
	munmap(shared_mem,size);
	sem_close(semaphores[0]);
	sem_close(semaphores[1]);
	sem_close(semaphores[2]);
	return EXIT_FAILURE;
}
