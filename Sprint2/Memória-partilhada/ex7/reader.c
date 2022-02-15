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

#define ARRAY_SIZE 10

typedef struct{
	int numbers[ARRAY_SIZE];
} shared_data_type;

int main(int argc, char *argv[]) {
	int fd, i, aux=0, num, data_size = sizeof(shared_data_type);
	shared_data_type *shared_data;
	/*abre a zona de memória partilhada já criada */
	fd = shm_open("/ex7", O_RDWR,S_IRUSR|S_IWUSR);

	/* em caso de erro no shm_open */
	if (fd == -1){
		perror("Error.");
		return 1;
	}

	/* em caso de erro no ftruncate */
	if (ftruncate (fd, data_size)==-1){
		perror("Error.");
		return 1;
	}

	/* pointer para a memória partilhada */
	shared_data=(shared_data_type *)mmap(NULL,data_size,PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

	for(i = 0; i < ARRAY_SIZE; i++){
		num = shared_data -> numbers[i];
		/* soma os números para depois calcular a média */
		aux=aux+num;
		/* imprime os números que estavam na memória partilhada */
		printf("%d",shared_data -> numbers[i]);
		printf("\n");
	}

	float average = (float) aux / ARRAY_SIZE;
	printf("\nAverage: %.2f\n", average);

	/* se não der erro desconecta o pointer para da memória partilhada */
	if (munmap(shared_data, data_size) == -1){
      perror("Error in munmap().\n");
      exit(EXIT_FAILURE);
    }

    /* se não der erro remove a zona de memória partilhada */
	if (shm_unlink("/ex7") == -1) {
        perror("Error in shm_unlink().\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}
