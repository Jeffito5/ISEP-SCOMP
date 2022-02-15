#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h> 

#define BUFFER_SIZE 100

typedef struct{
	int num;
	char name[BUFFER_SIZE];
} shared_data_type;

int main(int argc, char *argv[]) {
	int fd, data_size = sizeof(shared_data_type);
	shared_data_type *shared_data;
	/*abre a zona de memória partilhada já criada */
	fd = shm_open ("/ex1", O_RDWR,S_IRUSR|S_IWUSR);
	
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
	shared_data = (shared_data_type *)mmap(NULL,data_size,PROT_READ|PROT_WRITE, MAP_SHARED, fd,0);
	
	/* imprime os números lidos da zona de memória partilhada */
	printf("Number: %d\n",shared_data -> num);
	printf("Name: %s\n", shared_data -> name);
	
	/* se não der erro desconecta o pointer para da memória partilhada */
	if (munmap(shared_data, data_size) == -1){
        perror("Error in munmap().\n");
        exit(EXIT_FAILURE);
    }
    
    /* se não der erro remove a zona de memória partilhada */
	if (shm_unlink("/ex1") == -1) {
        perror("Error in shm_unlink().\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}
