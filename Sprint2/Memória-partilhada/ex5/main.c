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

typedef struct{
	int num1;
	int num2;
	int times;
} shared_data_type;

int main(int argc, char *argv[]) {
	pid_t a;
	int fd, data_size = sizeof(shared_data_type);
	shared_data_type *shared_data;
	/* cria e abre a zona de memória partilhada */
	fd = shm_open("/ex5", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
	
	/* em caso de erro no shm_open */
	if (fd == -1){
		perror("Error in shm_open().\n");
		return 1;
	}
	
	/* se não der erro no ftruncate define o tamanho da área de memória */
	if (ftruncate (fd, data_size)==-1){
		perror("Error in ftruncate().\n");
		return 1;
	}
	
	/* pointer para a memória partilhada */
	shared_data=(shared_data_type *)mmap(NULL,data_size,PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	shared_data->num1=8000;
	shared_data->num2=200;
	shared_data->times=100;

	printf("Initial first value: %d\n", shared_data->num1);
	printf("Initial second value: %d\n", shared_data->num2);
	
	/* faz o fork e verifica se deu erro ao fazê-lo */
	a = fork();
	if (a==-1){
		perror("Error. Couldn't create the process.\n");
		exit(0);
	}
	
	/* processo filho */
	if(a==0){
		/* decrementa o primeiro valor e incrementa o segundo */
		shared_data->num1--;
		shared_data->num2++;

	 }else{
		/* processo pai */
		/* incrementa o primeiro valor e decrementa o segundo */
		shared_data->num1++;
		shared_data->num2--;
	}
	
	printf("Final first value: %d\n", shared_data->num1);
	printf("Final second value: %d\n", shared_data->num2);
	
	/* se não der erro desconecta o pointer para da memória partilhada */
	if (munmap(shared_data, data_size) == -1){
        perror("Error in munmap().\n");
        exit(EXIT_FAILURE);
    }
    
    /* fecha zona de memória partilhada */
	if(close(fd) == -1) {
		perror("Error in munmap().\n");
		exit(EXIT_FAILURE);
	}
	
	return 0; 
}

