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
