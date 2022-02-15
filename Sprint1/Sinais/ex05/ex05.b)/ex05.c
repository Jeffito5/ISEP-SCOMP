#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>

void print_message(){
	printf("\nI won’t let the process end with CTRL-C!");
}

void handle_INT(){
	print_message();
}

int main(){
	printf("PID: %d",getpid());
	 struct sigaction act;
	 memset(&act, 0, sizeof(struct sigaction));
	 act.sa_handler = handle_INT;
	 sigaction(SIGINT, &act, NULL);
	for(;;){
		printf("I like Signal.\n");
		sleep(1);
	}
}
