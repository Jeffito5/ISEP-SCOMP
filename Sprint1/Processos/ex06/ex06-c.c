#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void){
	int i;
	pid_t getppid(void);
	//int status;
	pid_t a, b;	
	for(i=0;i<4;i++){
		a=fork();
		if(a==-1){
			perror("Fork falhou");exit(1);
		}
		if(a!=0){
			exit(0);
		}
	}
	if(a>0 && a%2==0){
		waitpid(a, &status, 0);
	}
	if(a>0){
		b=getppid();
		printf("%d", b);
	}
	printf("\nThis is the end.\n");
	return EXIT_SUCCESS;
}
			
	
