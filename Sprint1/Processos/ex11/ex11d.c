#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define ARRAY_SIZE 1000

void maxxx(int numbers[],int max){
	int result[ARRAY_SIZE];
	int g;
	for(g=(ARRAY_SIZE/2);g<(ARRAY_SIZE);g++){
		result[g]=((int) numbers[g]/max)*100;
	}
		
	for(g=0;g<(ARRAY_SIZE/2);g++){
		printf("%d\n",result[g]);
	}
}

int max_f(int numbers[]){
	int i,y,max,maxx,status;
	pid_t a;
	for(i=0;i<5;i++){
		max=0;
		a=fork();
		if(a==0){
			for(y=i*(ARRAY_SIZE/5);y<(i+1)*(ARRAY_SIZE/5);y++){
				if(numbers[y]>max){
					max=numbers[y];
				}
			}
			exit(max);
		}else{
			waitpid(a, &status, 0);
			if(WEXITSTATUS(status)>maxx){
				maxx=WEXITSTATUS(status);
			}
		}
	}
	return maxx;
}

int main(){
	int numbers[ARRAY_SIZE];
	int status;
	time_t t; 
	int i,max;
	srand((unsigned) time(&t));
	
	for(i=0;i<ARRAY_SIZE;i++){
		numbers[i]=rand()%256;
	}
	pid_t a;
	int y;
	for(i=0;i<5;i++){
		max=0;
		a=fork();
		if(a==0){
			for(y=i*(ARRAY_SIZE/5);y<(i+1)*(ARRAY_SIZE/5);y++){
				if(numbers[y]>max){
					max=numbers[y];
				}
			}
			exit(max);
		}else{
			waitpid(a, &status, 0);
			if(WEXITSTATUS(status)<=255){
				printf("Max= %d\n", WEXITSTATUS(status));
			}else{
				printf("Erro.\n");
			}
		}
	}

	pid_t b;
	int status2;
	b=fork();
	max=max_f(numbers);
	int g;
	int result[ARRAY_SIZE];
	if(b==0){
		for(g=0;g<(ARRAY_SIZE/2);g++){
			result[g]=((int) numbers[g]/max)*100;
		}
		for(g=0;g<(ARRAY_SIZE/2);g++){
			printf("%d\n",result[g]);
		} 
		exit(0);
	}else{
			waitpid(b, &status2, 0);
			maxxx(numbers,max);
			if(WEXITSTATUS(status2)==0){
				for(g=0;g<(ARRAY_SIZE/2);g++){
					printf("%d\n",result[g]);
				} 
			}else{
				printf("Erro.\n");
			}
	}
	return EXIT_SUCCESS;
}	
