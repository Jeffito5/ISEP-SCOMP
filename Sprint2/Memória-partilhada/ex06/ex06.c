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
int array_integers[ARRAY_SIZE];

typedef struct{
	int numero;
	int canRead;
	int canWrite;
} shared_data_type;

void fill_array(){
	time_t t;
	srand((unsigned)time(&t));
	int i;
	for(i=0;i<ARRAY_SIZE;i++){
		array_integers[i]=1+rand()%1000;
	}
}

int main(void){
	shared_data_type *shared_data;
	int fd,i, size_shared=sizeof(shared_data_type);
	fill_array();
	fd=shm_open("/ex06",O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR);
	ftruncate(fd,size_shared);
	shared_data =(shared_data_type *) mmap(NULL,size_shared, PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	shared_data->canWrite=1;
	for(i=0;i<ARRAY_SIZE;i++){
			while(!shared_data->canWrite){/** waits*/};
			shared_data->numero=array_integers[i];
			shared_data->canWrite=0;
			shared_data->canRead=1;
	}

	close(fd);
	shm_unlink("/ex06");
	return 0;
}
