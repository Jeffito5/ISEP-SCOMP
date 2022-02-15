#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

pid_t a;

typedef struct {
		char cmd[32];
		int tempo;
} comando; 

comando *last;

void handle_SIGCHLD(int signo){
	char msg[100];
	snprintf(msg, 100, "Execution of the command %s ended!\n",last->cmd);
	int nbytes=strlen(msg);
	write(STDOUT_FILENO,msg,nbytes);
}

void handle_SIGALRM(int signo){
	kill(a,SIGKILL);
	char message[100];
	snprintf(message, 100, "The command %s didn’t end in its allowed time!\n",last->cmd);
	int nbytes=strlen(message);
	write(STDOUT_FILENO,message,nbytes);
}


int main(void){
	struct sigaction act1,act2;
	memset(&act1, 0, sizeof(struct sigaction));
	memset(&act2, 0, sizeof(struct sigaction));
	act1.sa_handler=handle_SIGALRM;
	act2.sa_handler=handle_SIGCHLD;
	sigaction(SIGCHLD,&act2,NULL);
	sigaction(SIGALRM,&act1,NULL);
	
	comando c1 = {"./prog1", 8};  
	comando c2 = {"./prog2", 0};  
	comando c3 = {"./prog3", 4};  
	comando c4 = {"./prog4", 3};  
	comando c5 = {"./prog5", 10};
	int n = 5, i, ret;
	comando* comandos[n];
	comandos[0]=&c1;
	comandos[1]=&c2;
	comandos[2]=&c3;
	comandos[3]=&c4;
	comandos[4]=&c5;
	
	for(i=0; i < n; i++) {
		last=comandos[i];
		a=fork();
		if (a==0) {
			if((ret = execlp(last->cmd, last->cmd, (char*) NULL)) == -1) {
				printf("It was not possible to execute the command %s because of some error!\n",last->cmd);
			}else{
				exit(EXIT_SUCCESS);
			}
	
		} else if(a>0){
			alarm(last->tempo);
			wait(NULL);
		} else {
			perror("Erro ao criar processo");
			exit(-1);
		}
		}
		return EXIT_SUCCESS;
}
