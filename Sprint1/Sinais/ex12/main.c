#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#define CHILDREN 50;
#define true 1
#define false 0

void simulate2(){
	printf("Child process reach simulate 2. PID=%d\n", getpid());
}

void handle_child(int signo) {
	simulate2();
}

int simulate1(){
	int num;
	num=1 + (rand() % 10); 
	printf("Number: %d\n", num);
	if(num%2==0){
		kill(getppid(), SIGUSR1);
		return true;
	}else{
		kill(getppid(), SIGUSR2);
		return false;
	}
	wait(0);
}

int main(void){
	struct sigaction act;
	memset(&act, 0, sizeof(struct sigaction)); // limpar a estrutura
	act.sa_handler = handle_child;
	sigaction(SIGCHLD, &act, NULL);
	pid_t a;
	int i, counter=0, flag;
	for(i=0;i<50;i++){
		a=fork();
		if(a==0){
			flag=simulate1();
			if(flag==1){
				counter++;
			}
		}else{
			while(i<24){
				pause();
			}
			if(counter>0){
				kill(getpid(), SIGCHLD);
			}else{
				printf("Inefficient algorithm!\n");
				kill(a, SIGKILL);
			}
		}
	}
	return 0;
}
	
	
	
	
	
