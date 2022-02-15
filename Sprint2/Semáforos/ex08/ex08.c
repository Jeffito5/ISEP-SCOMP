#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <stdbool.h>
#include <sys/wait.h>
#define MAX_CICLE 5

typedef struct{
	int amount_print;
}num_writed;

void printTimes(int t,char letter){
 int i;
	 for(i=0;i<t;i++){
		printf("%c",letter);
		fflush(stdout);
	 }
}

int main(void){
    int i, semC=0,semS=1,random_option,size=sizeof(num_writed),fd;
    sem_t *semp, *semf;
    pid_t p;
    
    num_writed *ptr_num;
    
    srand(time(NULL));
	
	if((fd = shm_open("/ex08",O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR))==-1){
		perror("Error in shm_open\n");
		return EXIT_FAILURE;
	}
	ftruncate(fd,size);
	ptr_num=(num_writed *) mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	ptr_num->amount_print=1;
	if ((semp = sem_open("ex8_S", O_CREAT,S_IRUSR|S_IWUSR, semS))== SEM_FAILED) {
        perror("Error in sem_open() or sem already exists");
        exit(1);
    }

    if ((semf = sem_open("ex8_C", O_CREAT,S_IRUSR|S_IWUSR, semC))== SEM_FAILED) {
        perror("Error in sem_open() or sem already exists");
        exit(1);
    }
    
    p=fork();
    if(p==-1){
		perror("Error in fork()");
		return EXIT_FAILURE;
	}
	
    for(i=0;i<MAX_CICLE;i++){
		if(p>0){
			sem_wait(semp);
			do{
			random_option=rand()%3+1;
			if(random_option==1){
				ptr_num->amount_print--;
			}else if(random_option==2){
				ptr_num->amount_print++;
			}else{
				/**do nothing*/
			}
			}while(ptr_num->amount_print<0);
			printTimes(ptr_num->amount_print,'S');
			sem_post(semf);
		}else{
			sem_wait(semf);
			do{
			random_option=rand()%3+1;
			if(random_option==1){
				ptr_num->amount_print--;
			}else if(random_option==2){
				ptr_num->amount_print++;
			}else{
				/**do nothing*/
			}
			}while(ptr_num->amount_print<0);
			printTimes(ptr_num->amount_print,'C');
			sem_post(semp);
		}
	}
	
	if(p==0)
		exit(EXIT_SUCCESS);
    
    wait(NULL);
    printf("\n");
    close(fd);
	shm_unlink("/ex08");
	if(sem_unlink("ex8_S")==-1||sem_unlink("ex8_C")==-1){
		perror("Error in sem_unlink().\n");
		exit(EXIT_FAILURE);
	}
    
    return EXIT_SUCCESS;
}
