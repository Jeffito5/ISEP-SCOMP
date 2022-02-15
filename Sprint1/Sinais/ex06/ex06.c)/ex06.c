#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <time.h>

enum{SECS_SLEEP=0,NSEC_SLEEP=10000000};

int URS1_COUNTER=0;
volatile sig_atomic_t signal_Captured;

void handle_SIGSUR1(int signal_num){
	struct timespec remaining, request={SECS_SLEEP,NSEC_SLEEP};
	int i;
	URS1_COUNTER++;
	char message[45];
	signal_Captured=signal_num;
	size_t nbytes;
	pid_t a,num_pid;
	num_pid=getpid();
	a=fork();
	if(a>0){
		/**sleep(1);*/
		snprintf(message,45,"SIGURS1 signal captured, URS1_COUNTER=%d\n",URS1_COUNTER);
		nbytes=strlen(message);
		write(STDOUT_FILENO,message,nbytes);
		waitpid(a,NULL,0);
	} else {
		for(i=0;i<12;i++){
			printf("I going to interrupt you.\n");
			kill(num_pid, SIGINT);
			nanosleep(&request,&remaining);
		}
		exit(EXIT_SUCCESS);
	}
}

int main(void){

	printf("PID: %d\n",getpid());
	struct sigaction act;
	memset(&act, 0, sizeof(struct sigaction));
	sigfillset(&act.sa_mask);
	act.sa_handler = handle_SIGSUR1;
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &act, NULL);
	
	for(;;){
		printf("I'm working...\n");
		sleep(2);
	}
}
