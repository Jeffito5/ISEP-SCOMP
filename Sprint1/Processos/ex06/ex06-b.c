#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(void){
	int i;
	//int status;
	pid_t a;
	a=fork();
	if(a==-1){
		perror("Fork falhou");exit(1);
	}
	if(a!=0){
		for(i=0;i<2;i++){
			a=fork();
			if(a==0){
				sleep(1);
			}
		}
	}
	printf("\nThis is the end.\n");
	return EXIT_SUCCESS;
}
