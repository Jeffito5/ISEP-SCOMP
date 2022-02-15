#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define BUFFER_SIZE 80	

int main(){
	pid_t pid;
	int t=getpid();
	char read_msg[BUFFER_SIZE];
	char write_msg[BUFFER_SIZE];
	snprintf(write_msg,BUFFER_SIZE,"Pid=%d\n.",t);
	int fd[2];
	if(pipe(fd)==-1){
		perror("Que dia triste");
		return EXIT_FAILURE;
	}
	pid=fork();
	if(pid>0){
		close(fd[0]);
		write(fd[1],write_msg,BUFFER_SIZE);
		write(STDOUT_FILENO,write_msg,BUFFER_SIZE);
		close(fd[1]);
	}else{
		close(fd[1]);
		read(fd[0],read_msg,strlen(write_msg));
		printf("O filho leu %s\n",read_msg);
		close(fd[0]);
		exit(EXIT_SUCCESS);
	}
	return EXIT_SUCCESS;
}
