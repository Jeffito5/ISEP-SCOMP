#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#define TIMES 1000000

typedef struct{
	int num1;
} shared_data_type;

int main(void) {
	pid_t a;
	int status,fd, data_size = sizeof(shared_data_type);
	shared_data_type *shared_data;
	fd = shm_open("/ex8",O_RDWR, S_IRUSR|S_IWUSR);
	if (fd == -1){
		perror("Error.");
		return 1;
	}
	
	if (ftruncate (fd, data_size)==-1){
		perror("Error.");
		return 1;
	}
	
	shared_data=(shared_data_type *)mmap(NULL,data_size,PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	
	shared_data->num1=100;
	printf("Initial value: %d\n", shared_data->num1);

	a = fork();
	if (a==-1){
		perror("Error. Couldn't create the process.\n");
		exit(0);
	}
	
	if(a==0){
		int i;
		for(i=0;i<TIMES;i++){
			shared_data->num1--;
		}

	 }else{
		int i;
		for(i=0;i<TIMES;i++){
			shared_data->num1++;
		}

	}
	
	printf("Final value: %d\n", shared_data->num1);
	exit(EXIT_SUCCESS);
	wait(&status);
	
	munmap(shared_data,100);
	close(fd);
	shm_unlink("/ex08");

	return 0;
}

