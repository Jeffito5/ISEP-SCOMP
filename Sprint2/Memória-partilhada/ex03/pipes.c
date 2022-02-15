#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#define ARRAY_LENGTH 200
#define TOTAL_ARRAYS 100000

typedef struct {
	int num;
	char msg[ARRAY_LENGTH];
} note;

note notes[TOTAL_ARRAYS];

int main(void){
	//printf("Cheguei aqui");	
	pid_t p;
	note notes[TOTAL_ARRAYS];
	int i,fd[2];
	note *notes_pipe;	

	for(i=0;i<TOTAL_ARRAYS;i++){
		notes[i].num=(i+1);
		strcpy(notes[i].msg,"SCOMP - ISEP 2020");
	}

	if(pipe(fd)<0){
		perror("Não foi possivel criar o pipe");
		return EXIT_FAILURE;
	}
	int size = sizeof(note);
	p=fork();
	
		if(p>0){
			close(fd[0]);
			for(i=0;i<TOTAL_ARRAYS;i++){
				notes_pipe=&notes[i];
			write(fd[1],notes_pipe,size+1);
			}
			close(fd[1]);
		} else {
			note notes2[TOTAL_ARRAYS];
			close(fd[1]);
			for(i=0;i<TOTAL_ARRAYS;i++){
				read(fd[0],notes_pipe,size+1);
				notes2[i]=*(notes_pipe);
				printf("%d\n",notes2[2].num);
			}
			close(fd[0]);
		}
	
	return 0;
}

