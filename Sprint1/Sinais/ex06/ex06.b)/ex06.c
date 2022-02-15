#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
int URS1_COUNTER=0;

void handle_SIGSUR1(){
	URS1_COUNTER++;
	char message[45];
	size_t nbytes;
	snprintf(message,45,"SIGURS1 signal captured, URS1_COUNTER=%d\n",URS1_COUNTER);
	nbytes=strlen(message);	
	write(STDOUT_FILENO,message,nbytes);
}

int main(void){
	int i;
	printf("PID: %d\n",getpid());
	struct sigaction act;
	memset(&act, 0, sizeof(struct sigaction));
	sigemptyset(&act.sa_mask);
	
	for(i=1;i<65;i++){
		sigaddset(&act.sa_mask,i);
	}
	
	act.sa_handler = handle_SIGSUR1;
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &act, NULL);
	
	for(;;){
		printf("I'm working...\n");
		sleep(2);
	}
}
