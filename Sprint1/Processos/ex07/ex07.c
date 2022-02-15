#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define ARRAY_SIZE 1000

int main(){
	int numbers[ARRAY_SIZE];
	int* s;
	int n;
	int total=0;
	time_t t; 
	int i;
	srand((unsigned) time(&t));
	
	for(i=0;i<ARRAY_SIZE;i++){
		numbers[i]=rand()%10000;
	}
	
	n = rand()%10000;
	
	pid_t pid;
	pid = fork();
	
	if(pid==0){
		int lim=ARRAY_SIZE/2;
		for(i=0;i<lim;i++){
			if(numbers[i]==n){
				total++;
			}
		}
		exit(total);
	} else {
		int lim=ARRAY_SIZE/2;
		for(i=lim;i<ARRAY_SIZE;i++){
			if(numbers[i]==n){
			total++;
			}
		}
		waitpid(pid,&s,0);
		total= total + WEXITSTATUS(s);
		printf("O numero de vezes que aparece é: %d\n",total);
	}
	return EXIT_SUCCESS;
}