#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h> 
#include <sys/wait.h> 
#define INT_QUANT 10
#define AVAILABLE 30
typedef struct{
	int inteiros[INT_QUANT];
	int available;
	int dataWrite;
}shared_data_type;

int main(void){
	int fd,i,status ,size_shared=sizeof(shared_data_type);
	pid_t p;
	shared_data_type *shared_data;
	p=fork();
	if(p>0){
		int num=0;
		fd=shm_open("/buffer",O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR);
		ftruncate(fd,size_shared);
		shared_data = (shared_data_type *) mmap(NULL,size_shared,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
		shared_data->available=AVAILABLE;
		while(1){
		for(i=0;i<INT_QUANT;i++){
			if(shared_data->available==0){
				close(fd);
				shm_unlink("/buffer");
				return 0;
			}
			while(shared_data->dataWrite!=0);
			num++;
			shared_data->inteiros[i]=num;
			--(shared_data->available);
			++(shared_data->dataWrite);
		}
		
		}
		wait(&status);
	}else{
		fd=shm_open("/buffer",O_RDWR,S_IRUSR|S_IWUSR);
		ftruncate(fd,size_shared);
		shared_data = (shared_data_type *) mmap(NULL,size_shared,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
		while(1){
		for(i=0;i<INT_QUANT;i++){
			if(shared_data->available==0){
			exit(EXIT_SUCCESS);
			}
			while(shared_data->dataWrite!=1);
			printf("%d\n",shared_data->inteiros[i]);
			--(shared_data->dataWrite);
		}
		}
		
		
	}
}
