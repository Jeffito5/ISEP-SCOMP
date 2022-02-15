#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

void handle_SIGALRM(int signo){
	char message[]="To slow, that is why the program will end!\n";
	int nbytes=strlen(message);
	write(STDOUT_FILENO,message, nbytes);
	raise(SIGKILL);
}

int main(void){
	int i;
	int count=0;
	char input[100];
	struct sigaction act;
	memset(&act, 0,sizeof(struct sigaction));
	sigemptyset(&act.sa_mask);
	act.sa_handler = handle_SIGALRM;
	sigaction(SIGALRM,&act,NULL);
	printf("Let we start a game hahahhahahaha!\nYou have to write fast than 10secs\nStarting in...");
	for(i=3;i>=1;i--){
		printf("%d...\n",i);
		sleep(1);
	}
	printf("NOW!\n");
	alarm(10);
	while(fgets(input,sizeof(input),stdin)==NULL){
			//do nothing
	}
	signal(SIGALRM,SIG_IGN);
	
	for(i=0;i<strlen(input);i++){
		if(input[i]!=' '){
			if(input[i]!='\n'){
			count++;
		}
		}
	}
	printf("Your string have %d characters.\nSee u later\n",count);
	sleep(20);
	printf("Sucessful execution! Bye!\n");
	
	
	return EXIT_SUCCESS;
}
