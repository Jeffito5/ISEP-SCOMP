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

int random_number(){
	int random_n;
	random_n=1 + rand() % 19;
	return random_n;
}

int main(int argc, char *argv[]) {
	int fd, i, random_num, data_size = sizeof(shared_data_type);
	shared_data_type *shared_data;
	int numbers2[ARRAY_SIZE];
	/* cria e abre a zona de memória partilhada */	
	fd = shm_open("/ex7", O_CREAT|O_RDWR,S_IRUSR|S_IWUSR);
	
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
	
	for(i = 0; i < ARRAY_SIZE; i++){
		//chamada da função de alto nível
		random_num=random_number();
		numbers2[i] = random_num;
		/* coloca o array com os números random preenchidos na memória partilhada */
		shared_data -> numbers[i] = numbers2[i];
	}
	
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



