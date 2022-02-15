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
#include <time.h>
#define ARRAY_SIZE 1000
int array_integers[ARRAY_SIZE];
int max=0;

typedef struct{
	int num[10];
} shared_data_type;

void fill_array(void){
	time_t t;
	srand((unsigned)time(&t));
	int i;
	for(i=0;i<ARRAY_SIZE;i++){
		array_integers[i]=1+rand()%1000;
	}	
}

int main(void){
	shared_data_type *shared_data;
	int size_shared=sizeof(shared_data),i,fd,y,status;
	pid_t p;
	fill_array();
	fd=shm_open("/shared_int",O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR);
	ftruncate(fd,size_shared);
	p=fork();
	if(p>0)
	shared_data = (shared_data_type *) mmap(NULL,size_shared,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	for(i=0;i<9;i++){
		if(p>0){
			p=fork();
		}
		
		if(p==0){
			
			for(y=i*100;y<(i*100+100);y++){
				shared_data=(shared_data_type *)mmap(NULL,size_shared,PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
				if(array_integers[y]>max){
					max=array_integers[y];
				}
			}
			shared_data->num[i]=max;
			exit(EXIT_SUCCESS);
		}
	}
	
	for(i=0;i<10;i++){
	waitpid(p,&status,0);	
	}
	for(i=0;i<10;i++){
		if(shared_data->num[i]>max){
			max=shared_data->num[i];
		}
	}
	printf("%d\n",max);
	close(fd);
	shm_unlink("/shared_int");
	
	
	
	return 0;
}
