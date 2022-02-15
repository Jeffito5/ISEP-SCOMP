#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void){
	
	int* st1,st2;
	pid_t a,b;
	a = (int) fork();
	
	if(a==0){
		sleep(1);
		exit(1);
	} else {
		b = (int) fork();
		if(b==0){
			sleep(2);
			exit(2);
		} else {
			waitpid(a,&st1,0);
			waitpid(b,&st2,0);
			printf("%d\n",WEXITSTATUS(st1));
			printf("%d\n",WEXITSTATUS(st2));
		}
	}
	return EXIT_SUCCESS;
	
}
