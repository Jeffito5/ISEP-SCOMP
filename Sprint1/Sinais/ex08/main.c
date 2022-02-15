#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

volatile sig_atomic_t counter=5;

void handle_child(int signo) {
	counter--;
	printf("%d child(ren) are/is still executing...\n", counter);
}

int main(){
	struct sigaction act;
	memset(&act, 0, sizeof(struct sigaction)); // limpar a estrutura
	act.sa_handler = handle_child;
	act.sa_flags = SA_NOCLDWAIT;
	sigaction(SIGCHLD, &act, NULL);
	pid_t a;
	int i,j,result;
	printf("5 children are going to be executed...\n");
	for(i=0;i<5;i++){
		a=fork();
		if(a==0){
			for(j=i*2;j<((i+5)*2)+1;j++){
				result=j;
				printf("Result: %d\n", result);
				sleep(1);
			}
			kill(getppid(),17);
			exit(result);
			
		}else{
			pause();
			if(counter==0){
				printf("End.");
			}
		}
		}
	return 0;
}
