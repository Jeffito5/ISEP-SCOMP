#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h> 
#include <semaphore.h>
#include <string.h>
#include <time.h>

#define s1 "I'm the Father - with PID "
#define STRING_NUMBER 50
#define STRING_SIZE 80
#define WAIT_TIME 5
#define NUMBER_PROCESSES 3

typedef struct{
	char strings[STRING_NUMBER][STRING_SIZE];
	int stringNumber;
} shared_data_type;

int main(int argc, char *argv[]) {
	int fd, i, data_size = sizeof(shared_data_type);
	pid_t a;
	
	/* criar e abre uma zona de memória partilhada */
	fd = shm_open("/shm_ex3", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
	shared_data_type *shared_data = (shared_data_type*) mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);
	
	/* erro se o file descriptor retornado deu erro */
	if(fd == -1){
		perror("Error in shm.\n");
		return 1;
	}
	
	/* em caso de erro no ftruncate */
	if (ftruncate (fd, data_size)==-1){
		perror("Error.");
		return 1;
	}
	
	/* cria um semáforo com o valor 1 */
	sem_t *sem;
	if ((sem = sem_open("/sem_ex3", O_CREAT, 0644,1)) == SEM_FAILED) {
        perror("Error in sem_open().\n");
        exit(1);
    }
	
	time_t t;
	srand((unsigned) time(&t));
		
	/* Criação de 3 processos */
    for (i = 0; i < NUMBER_PROCESSES; i++){
        a=fork();
        /*processo filho */
        if (a == 0){
            /* Decrementa e bloqueia o semáforo */
            sem_wait(sem);

            /* Escreve na memória partilhada a frase */
            sprintf(shared_data->strings[shared_data->stringNumber], "%s%d", s1, getpid());
            /* Avança o índice */
			shared_data -> stringNumber++;
			
			/* verifica se ainda nã chegou às 50 strings */
            if (shared_data->stringNumber >= STRING_NUMBER){
                shared_data->stringNumber = 0;
            }

            /* Incrementa o semáforo */
            sem_post(sem);

            /* Espera entre 1 a 5 segundos */
            int number_of_seconds = rand() % (WAIT_TIME+1);
            printf("Wait time: %d\n",number_of_seconds);
			sleep(number_of_seconds);

            /* Fecha o semáforo */
            sem_close(sem);

            exit(0);
        }
         else
        {
            wait(NULL);
        }
    }
	/* Imprime na memória partilhada */
    for (i = 0; i < shared_data->stringNumber; i++){
        printf("%s\n", shared_data->strings[i]);
    }
    
     /* verifica se deu erro ao fechar a memória partilhada */
    if (shm_unlink("shm_ex3")==-1){
        perror("Error in shm_unlink().\n");
        exit(-1);
    }
    
    /* desconecta o área de memória partilhada do espaço do endereço do processo */ 
    if (munmap(shared_data, data_size) < 0){
        perror("Error in munmap.\n");
        exit(EXIT_FAILURE);
    }
    
    /* fecha o descritor */
    if(close(fd)==-1){
        perror("Error in sem_close.\n");
        exit(EXIT_FAILURE);
    }

    /* verifica se deu erro ao fechar o semáforo */
    if(sem_unlink("sem_ex3")==-1){
		perror("Error in sem_unlink().\n");
		exit(EXIT_FAILURE);
	}

	
	return 0;
}
