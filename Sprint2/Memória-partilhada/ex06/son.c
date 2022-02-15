#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h> 
#include <time.h>
#define ARRAY_SIZE 1000000

typedef struct{
	int numero;
	int canRead;
	int canWrite;
} shared_data_type;

int main(){
	shared_data_type *shared_data;
	int fd,i,timer, num[ARRAY_SIZE],size_shared=sizeof(shared_data_type);
	fd=shm_open("/ex06",O_RDWR,S_IRUSR|S_IWUSR);
	ftruncate(fd,size_shared);
	shared_data =(shared_data_type *) mmap(NULL,size_shared, PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	clock_t before = clock();
	for(i=0;i<ARRAY_SIZE;i++){
		while(!shared_data->canRead);
		num[i]=shared_data->numero;
		shared_data->canRead=0;
		shared_data->canWrite=1;
	}
	clock_t difference = clock() - before;
	timer=difference*1000/CLOCKS_PER_SEC;
	printf("This progrma spent %d seconds and %d miliseconds.\n",timer/1000,timer%1000);
	close(fd);
	shm_unlink("/ex06");
	return 0;
}
