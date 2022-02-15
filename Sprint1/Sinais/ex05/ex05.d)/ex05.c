#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>

void print_message1(){
	printf("\nI won’t let the process end with 'CTRL-\\'!");
}
void print_message2(){
	printf("\nI won’t let the process end with CTRL-C!");
}
void handle_INT(){
	print_message2();
}

void handle_QUIT(){
	print_message1();
}

int main(){
	printf("PID: %d",getpid());
	 struct sigaction act1;
	 memset(&act1, 0, sizeof(struct sigaction));
	 act1.sa_handler = handle_QUIT;
	 sigaction(SIGQUIT, &act1, NULL);
	 
	 struct sigaction act2;
	 memset(&act2, 0, sizeof(struct sigaction));
	 act2.sa_handler = handle_INT;
	 sigaction(SIGINT, &act2, NULL);
	 
	for(;;){
		printf("\nI like Signal.");
		sleep(1);
	}
}
