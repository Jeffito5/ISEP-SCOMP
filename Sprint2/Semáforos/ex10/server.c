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
	int option, fd,size=sizeof(shared_data);
	shared_data *shared_mem;
	sem_t *semaphores[SEM_QUANT];
	
	strcpy(sem_names[0],"sem1");
	strcpy(sem_names[1],"sem2");
	strcpy(sem_names[2],"sem3");
	
	semaphores[0]=sem_open(sem_names[0],O_CREAT|O_EXCL,0644,1);
	semaphores[1]=sem_open(sem_names[1],O_CREAT|O_EXCL,0644,1);
	semaphores[2]=sem_open(sem_names[2],O_CREAT|O_EXCL,0644,1);
	
	
	fd=shm_open("/ex10",O_CREAT|O_RDWR,S_IRUSR|S_IWUSR);
	ftruncate(fd,size);
	shared_mem=(shared_data *) mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	
	shared_mem->next_data=0;
	shared_mem->nr_leitores=0;
	
	do{
		printf("What u want to do?!\n1. Consult \n2. Insert \n3. Consult all\n");
		scanf("%d",&option);
		
		switch(option){
			case 0:
				printf("Ok, getting out\n");
				break;
			case 1:
				execlp("./consult","./consult",(char *)NULL);
				break;
		
			case 2:
				execlp("./insert","./insert",(char *)NULL);
				break;
		
			case 3:
				execlp("./consultall","./consultall",(char *)NULL);
				break;
			default:
				printf("The selected option not available.\nPls select a accpetable one.\n");
				break;
		}
		
	}while(option!=0);	
	
	close(fd);
	shm_unlink("/ex10");
	if(sem_unlink(sem_names[0])==-1||sem_unlink(sem_names[1])==-1||sem_unlink(sem_names[3])==-1){
		perror("Erro fatal.");
		return EXIT_FAILURE;
	}	


	return EXIT_SUCCESS;
}
