#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <semaphore.h>
#include <string.h>
#include <sys/mman.h>
#define CAPACITY 200

typedef struct {
	int passagers;
}train;

int main(void){
	sem_t *sem1;
	int fd,size_shared=sizeof(train),i,y;
	pid_t p;
	sem1=sem_open("sem1",O_CREAT|O_EXCL,0644,3);
	train *train1;
	
	fd = shm_open("/ex11",O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR);	
	ftruncate(fd,size_shared);
	train1 =(train *) mmap(NULL,size_shared,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	p=fork();
	train1->passagers=0;
	for(i=0;i<3;i++){
		if(p>0)
		p=fork();
		if(p==0){
			if(i==0){
				sem_trywait(sem1);
				for(y=0;y<50;y++){
					(train1->passagers)++;
					if((train1->passagers)>=CAPACITY)
					break;
				}
				sem_post(sem1);
			}	
			
			if(i==1){
				sem_trywait(sem1);
				for(y=0;y<50;y++){
					(train1->passagers)--;
					if((train1->passagers)<=0)
					break;
				}
				sem_post(sem1);
			}
			if(i==2){
				sem_wait(sem1);
				for(y=0;y<100;y++){
					(train1->passagers)++;
					if((train1->passagers)>=CAPACITY)
					break;
				}
				sem_post(sem1);
			}
			if(i==3){
				sem_trywait(sem1);
				for(y=0;y<150;y++){
					(train1->passagers)++;
					if((train1->passagers)>=CAPACITY){
					printf("Maximum capacity: %d",train1->passagers);
					break;
					}
					
				sem_post(sem1);
				}
			}
			exit(EXIT_SUCCESS);
		}
	}

	for(i=0;i<4;i++){
		wait(NULL);
	}
	return 0;
}
