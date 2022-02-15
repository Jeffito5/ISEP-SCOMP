#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#define ARRAY_SIZE 1000000
int array_integers[ARRAY_SIZE];

typedef struct{
	int numero;
	int canRead;
	int canWrite;
} shared_data_type;

void fill_array(){
	time_t t;
	srand((unsigned)time(&t));
	int i;
	for(i=0;i<ARRAY_SIZE;i++){
		array_integers[i]=1+rand()%1000;
	}
}

int main(void){
	int i,fd[2],status;
	pid_t p;
	fill_array();
	pipe(fd);
	
	p=fork();
	if(p>0){
		close(fd[0]);
		for(i=0;i<ARRAY_SIZE;i++){
			write(fd[1],&array_integers[i],sizeof(int));
		}
		close(fd[1]);
		
	}else{
		int num[ARRAY_SIZE],timer;
		close(fd[1]);
		clock_t before = clock();
		for(i=0;i<ARRAY_SIZE;i++){
		if(read(fd[0], &num[i], sizeof(int))!=0){
			}else
          {
             break;
          }
			
		}
		clock_t difference = clock() - before;
		timer=difference*1000/CLOCKS_PER_SEC;
		printf("This progrma spent %d seconds and %d miliseconds.\n",timer/1000,timer%1000);
		close(fd[0]);
		exit(EXIT_SUCCESS);
	}
	wait(&status);
	
	return EXIT_SUCCESS;
}
