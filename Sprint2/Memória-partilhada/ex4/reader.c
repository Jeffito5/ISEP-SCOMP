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

#define ARRAY_SIZE 100

typedef struct{
	int num;
	char chars[ARRAY_SIZE];
} shared_data_type;

int main(int argc, char *argv[]) {
	int fd, data_size = sizeof(shared_data_type);
	shared_data_type *shared_data;
	/*abre a zona de memória partilhada já criada */
	fd = shm_open ("/ex4", O_RDWR, S_IRUSR|S_IWUSR);
	int i, sum=0;
	
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
	
	/* loop infinito até um break statement. Vai somar o código ASCII dos caracteres */
	while(!shared_data->num);
		for(i=0;i<ARRAY_SIZE;i++){
			sum+=shared_data->chars[i];
		}
		
		printf("Sum: %d\n", sum);
		printf("Average: %d\n", sum/ARRAY_SIZE);
		for(i=0;i<ARRAY_SIZE;i++){
			printf("Char: %d\n", shared_data->chars[i]);
		}
	
	/* se não der erro desconecta o pointer para da memória partilhada */	
	if (munmap(shared_data, data_size) == -1){
        perror("Error in munmap().\n");
        exit(EXIT_FAILURE);
    }
    
    /* se não der erro remove a zona de memória partilhada */
	if (shm_unlink("/ex4") == -1) {
        perror("Error in shm_unlink().\n");
        exit(EXIT_FAILURE);
    }	
	return 0;
}
