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

#define BUFFER_SIZE 100

typedef struct{
	int num;
	char name[BUFFER_SIZE];
} shared_data_type;

int main(int argc, char *argv[]) {
	int fd, data_size = sizeof(shared_data_type);
	shared_data_type *shared_data;
	/* cria e abre a zona de memória partilhada */
	fd = shm_open ("/ex1", O_CREAT|O_RDWR,S_IRUSR|S_IWUSR);
	int num2;
	char name2[BUFFER_SIZE];
	
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
	
	printf("Write the number please:");
	scanf("%d", &num2);
	printf ("Write the name please:");
	scanf ("%s", name2);
	
	/* o número e nome são escritos na memória para serem lidos pelo reader */
	shared_data->num=num2;
	strcpy(shared_data->name, (name2));
	
	/* se não der erro desconecta o pointer para da memória partilhada */
	if (munmap(shared_data, data_size) == -1){
		perror("Error in munmap().\n");
		exit(EXIT_FAILURE);
    }
	
	/* fecha a zona de memória partilhada */
	if(close(fd) == -1) {
		perror("Error in munmap().\n");
		exit(EXIT_FAILURE);
	}
   
    return 0;
}

