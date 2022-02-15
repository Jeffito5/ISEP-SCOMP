#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <semaphore.h>
#include <string.h>
#include <sys/mman.h>
#include <string.h>
#define MAX_STRING 200
#define MAX_WRITER 100

typedef struct{
	char string[MAX_STRING];
	int pids[MAX_WRITER];
	int canClose;
} server_data;

int main(void){
	int fd, size_shared=sizeof(server_data);
	
	server_data *shared_data;
	fd=shm_open("/ex14",O_RDWR,S_IRUSR|S_IWUSR);
	ftruncate(fd,size_shared);
	shared_data = (server_data *) mmap(NULL,size_shared,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	
	shared_data->canClose=1;
	//close(fd);

	return EXIT_SUCCESS;
}

