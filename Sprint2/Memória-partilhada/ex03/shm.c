#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h> 
#include <time.h>
#include <string.h>
#define ARRAY_LENGTH 200
#define TOTAL_ARRAYS 100000

typedef struct {
	int num;
	char msg[ARRAY_LENGTH];
	int dataWrite;
} note;

int main(void){
	pid_t p;
	int i,status,fd,size_shared=sizeof(note);
	note notes[TOTAL_ARRAYS];
	note *notes_shm;
	
	fd=shm_open("/shm",O_RDWR,S_IRUSR|S_IWUSR);
	ftruncate(fd,size_shared);
	notes_shm =(note *) mmap(NULL,size_shared, PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	
	for(i=0;i<TOTAL_ARRAYS;i++){
		notes[i].num=(i+1);
		strcpy(notes[i].msg,"SCOMP - ISEP 2020");
	}
	p=fork();
	notes_shm->dataWrite=1;
	if(p>0){
		for(i=0;i<TOTAL_ARRAYS;i++){
			while(!notes_shm->dataWrite);
			notes_shm=&notes[i];
			notes_shm->dataWrite=1;
		}		
	}else{
		note notes2[TOTAL_ARRAYS];
		for(i=0;i<TOTAL_ARRAYS;i++){
			while(notes_shm->dataWrite);
			notes2[i]=*(notes_shm);
			notes_shm->dataWrite=0;			
		}
		exit(EXIT_SUCCESS);		
	}
	wait(&status);
	close(fd);
	shm_unlink("/shm");
	return EXIT_SUCCESS;
}
