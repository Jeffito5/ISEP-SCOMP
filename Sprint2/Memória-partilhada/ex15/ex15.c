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
}shared_data_type;

int main(void){
	int fd1,fd2[2],dataWrite,fd3[2],i,status ,size_shared=sizeof(shared_data_type),size_int=sizeof(int);
	pid_t p;
	shared_data_type *shared_data;
	if(pipe(fd2)<-1||pipe(fd3)<-1){
		perror("It was not possible to create the pipe for some reason.");
		return EXIT_FAILURE;
	}
	p=fork();
	if(p>0){
		close(fd2[0]);
		close(fd3[1]);
		int num=0;
		dataWrite=0;
		fd1=shm_open("/buffer",O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR);
		ftruncate(fd1,size_shared);
		shared_data = (shared_data_type *) mmap(NULL,size_shared,PROT_READ|PROT_WRITE,MAP_SHARED,fd1,0);
		shared_data->available=AVAILABLE;
		while(1){
		for(i=0;i<INT_QUANT;i++){
			if(shared_data->available==0){
				close(fd1);
				shm_unlink("/buffer");
				return 0;
			}
			while(dataWrite!=0){
				read(fd3[0],&dataWrite,size_int);
			};
			num++;
			shared_data->inteiros[i]=num;
			--(shared_data->available);
			++dataWrite;
			write(fd2[1],&dataWrite,size_int);
			
		}
		
		}
		wait(&status);
	}else{
		close(fd2[1]);
		close(fd3[0]);
		fd1=shm_open("/buffer",O_RDWR,S_IRUSR|S_IWUSR);
		ftruncate(fd1,size_shared);
		shared_data = (shared_data_type *) mmap(NULL,size_shared,PROT_READ|PROT_WRITE,MAP_SHARED,fd1,0);
		
		while(1){
			for(i=0;i<INT_QUANT;i++){
					if(shared_data->available==0){
						exit(EXIT_SUCCESS);
					}
				
				while(dataWrite!=1){
					read(fd2[0],&dataWrite,size_int);
				}
				printf("%d\n",shared_data->inteiros[i]);
				dataWrite--;
				write(fd3[1],&dataWrite,size_int);
			}
			
		}
		
		
	}
}
