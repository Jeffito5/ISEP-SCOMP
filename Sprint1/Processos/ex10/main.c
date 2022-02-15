#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define ARRAY_SIZE 2000

int main(){
	int numbers[ARRAY_SIZE];
	int* status;
	int n;
	time_t t; 
	int i;
	srand((unsigned) time(&t));
	for(i=0;i<ARRAY_SIZE;i++){
		numbers[i]=rand()%2000;
	}
	n = rand()%2000;
	pid_t a;
	int y;
	for(i=0;i<10;i++){
		a=fork();
		if(a==0){
			for(y=0;y<200;y++){
				if(numbers[i*200+y]==n){
					exit(y);
				}else{
					exit(255);
			}
		}
		}else{
			waitpid(a, &status, 0);
			if(WEXITSTATUS(status)<200){
				printf("%d\n", WEXITSTATUS(status));
			}else{
				printf("Não encontrado qualquer número igual a n\n");
			}
		}
	}
	return EXIT_SUCCESS;
} 
	
