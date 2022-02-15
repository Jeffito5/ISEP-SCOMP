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
#define SEM_QUANT 2

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
	char sem_names[SEM_QUANT][STRING_MAX],name_usr[STRING_MAX],add_usr[STRING_MAX];
	int id_usr,fd,size=sizeof(shared_data),i;
	shared_data *shared_mem;
	sem_t *semaphores[SEM_QUANT];
	
	strcpy(sem_names[0],"sem1");
	strcpy(sem_names[1],"sem2");
	
	semaphores[0]=sem_open(sem_names[0],O_CREAT);
	semaphores[1]=sem_open(sem_names[1],O_CREAT);
	
	
	fd=shm_open("/ex10",O_RDWR,S_IRUSR|S_IWUSR);
	ftruncate(fd,size);
	shared_mem=(shared_data *) mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	
	sem_wait(semaphores[0]);
	sem_wait(semaphores[1]);
	
	int index=shared_mem->next_data;
	printf("Put the name pls: ");
	scanf("%s",name_usr);
	
		printf("Insert the number pls: ");
		scanf("%d",&id_usr);
		for(i=0;i<index+1;i++){
			if(id_usr==(shared_mem->dados[i].id_number)){
				printf("Id_number already exists in the system.\n");
				(shared_mem->next_data)++;
				sem_post(semaphores[0]);
				sem_post(semaphores[1]);
				close(fd);
				munmap(shared_mem,size);
				sem_close(semaphores[0]);
				sem_close(semaphores[1]);
				return EXIT_FAILURE;
			}
		}
	
	printf("Put the address pls: ");
	scanf("%s",add_usr);
	
	strcpy(shared_mem->dados[index].name,name_usr);
	strcpy(shared_mem->dados[index].address,add_usr);
	shared_mem->dados[index].id_number=id_usr;
	
	sem_post(semaphores[0]);
	sem_post(semaphores[1]);
	
	close(fd);
	munmap(shared_mem,size);
	sem_close(semaphores[0]);
	sem_close(semaphores[1]);
	return EXIT_SUCCESS;
}
