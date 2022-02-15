#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>

static volatile sig_atomic_t flag=0;

void alarm_handler(){
    flag=1;
}

void executing_parent(pid_t a){
	printf("Executing task A...\n");
	int i;
		for(i=0;i<4;i++){
			printf("Task A -> segundos: %d\n", i);
			sleep(1);
		}
		waitpid(a,NULL,0);
		kill(getpid(), 10);
		exit(0);
	}

void executing_child(){
	time_t t; 
	int j;
	srand((unsigned) time(&t));
	j=rand()%5;
	alarm(j);
	printf("Executing task B...\n");
	int i;
	for(i=0;i<j;i++){
		printf("Task B -> segundos: %d\n", i);
		sleep(1);
	}
}

void executing_another_child(){
	if(flag==1){
		printf("Executing task C...\n");
	}
}

void handle_CHILD(){
	executing_another_child();
}

int main(void){
	struct sigaction act;
	memset(&act, 0, sizeof(struct sigaction)); // limpar a estrutura
	act.sa_handler = handle_CHILD;
	sigaction(SIGUSR1, &act, NULL);
	pid_t a;
	a=fork();
	if(a>0){
		signal(SIGALRM, alarm_handler);
		alarm(3);
		executing_parent(a);
	}else{
		executing_child();
	}
    return 0;
}


	
