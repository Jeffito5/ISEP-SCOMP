#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#define BUFFER_SIZE 80

int main(void){
	pid_t a;
	int status;
	int fd[2];
	char write_msg1[]="Hello World!\n";
	char write_msg2[]="Goodbye!\n";
	char read_msg1[BUFFER_SIZE];
	char read_msg2[BUFFER_SIZE];
	
	if(pipe(fd)==-1){
		perror("Um erro aconteceu.\n");
		return EXIT_FAILURE;
	}
	a=fork();
	if(a<0){
		perror("Um erro impediu o procedimento.\n");
		return EXIT_FAILURE;
	}
	if(a>0){
		close(fd[0]);
		write(fd[1],write_msg1,strlen(write_msg1)+1);
		write(fd[1],write_msg2,strlen(write_msg2)+1);
		close(fd[1]);
		wait(&status);	
		int sta=WEXITSTATUS(status);
		printf("Pid=%d.\nStatus: %d.\n",getpid(),sta);	
	} else {
		close(fd[1]);
		while(read(fd[0],read_msg1,strlen(write_msg1)+1)==0){
			//Do nothing
		}
		write(STDOUT_FILENO,read_msg1,strlen(read_msg1));
		while(read(fd[0],read_msg2,strlen(write_msg2)+1)==0){
			//Do nothing
		}
		write(STDOUT_FILENO,read_msg2,strlen(read_msg2));
		close(fd[0]);
		exit(strlen(read_msg2)+strlen(read_msg1));
	}
	
	return EXIT_SUCCESS;
}
