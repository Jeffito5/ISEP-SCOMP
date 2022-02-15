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

#define CHILDREN 2
#define BUFFER_SIZE 10
#define EXCHANGED 30

typedef struct {
    int array[BUFFER_SIZE];
    int write;
    int read;
    int aux;
} shared_data_type;


int main(int argc, char *argv[]) {
	sem_t *sem, *full, *empty;
	pid_t a;
	shared_data_type *ptr;
	int fd, data_size = sizeof(shared_data_type);
    int i, j, k;

	/* cria um semáforo com o valor 1 e verifica se dá erro ao criar */
	if ((sem = sem_open("/sem_ex13Sem", O_CREAT, 0600,1)) == SEM_FAILED) {
        perror("Error in sem_open().\n");
        exit(1);
    }
	
	/* cria um semáforo com o valor 0 e verifica se dá erro ao criar para o buffer full */
	if ((full = sem_open("/sem_ex13Full", O_CREAT, 0600,0)) == SEM_FAILED) {
        perror("Error in writer's sem_open().\n");
        exit(1);
    }
	
	/* cria um semáforo com o valor 10 e verifica se dá erro ao criar para o buffer vazio */
	if ((empty = sem_open("/sem_ex13Empty", O_CREAT, 0600,BUFFER_SIZE)) == SEM_FAILED) {
        perror("Error in reader's sem_open().\n");
        exit(1);
    }
	
	/* criar e abre uma zona de memória partilhada */
	fd = shm_open("/shm_ex13", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
	/* erro se o file descriptor retornado deu erro */
	if (fd == -1){
		perror("Error in shm_open.\n");
		return 1;
	}
	
	/* em caso de erro no ftruncate */
	if (ftruncate(fd,data_size)==-1){
		perror("Error.\n");
		return 1;
	}
		
	ptr = (shared_data_type*) mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);

    ptr->write = 0;
    ptr->read = 0;
	ptr->aux = 0;
		
	for (i = 0; i < CHILDREN; i++){
	
		a = fork();
		if (a < 0){
			perror("Error: couldn't create the process.\n");
			exit(1);
		}
		
		/* processo filho */
        if (a==0){
            for(j=0; j<15; j++){
				/* verifica se deu erro ao decrementar o semáforo */
				if(sem_wait(empty)==-1){
					perror("Error in producer's sem_wait.\n");
					exit(1);
				}
				/* verifica se deu erro ao decrementar o semáforo */
				if(sem_wait(sem)==-1){
					perror("Error in sem_wait().\n");
					exit(1);
				}

				if(ptr->write >= BUFFER_SIZE){
					ptr->write = 0;
				}

				ptr->array[ptr->write] = ptr->aux;
				printf("Producer(%d) wrote %d in %d.\n",i+1,ptr->array[ptr->write],ptr->write);
				ptr->write++;
				ptr->aux++;
				
				/* verifica se deu erro a incrementar o semáforo */
				if(sem_post(sem)==-1){
					perror("Error in sem_post().\n");
					exit(1);
				}
				/* verifica se deu erro a incrementar o semáforo */
				if(sem_post(full)==-1){
					perror("Error in sem_post().\n");
					exit(1);
				}
            }  
            exit(0);
        }
    }
    
	if (a >0){
		 
		for (k=0; k<EXCHANGED; k++){
			/* verifica se deu erro ao decrementar o semáforo */
			if(sem_wait(full)==-1){
				perror("Error in full's sem_wait().\n");
				exit(1);
			}
			/* verifica se deu erro ao decrementar o semáforo */
			if(sem_wait(sem)==-1){
				perror("Error in sem's sem_wait().\n");
				exit(1);
			}

            if(ptr->read >= BUFFER_SIZE){
                ptr->read = 0;
            }

            printf("Consumer: Read %d\n",ptr->array[ptr->read]);
            ptr->read++;
			
			/* verifica se deu erro a incrementar o semáforo */
			if(sem_post(sem)==-1){
				perror("Error in sem's sem_post().\n");
				exit(1);
			}
			/* verifica se deu erro a incrementar o semáforo */
			if(sem_post(empty)==-1){
				perror("Error in empty's sem_post().\n");
				exit(1);
			}
		}
    } 
    /* verifica se deu erro ao fechar o semáforo */ 
	if(sem_unlink("/sem_ex13Sem")==-1){
		perror("Error in sem_unlink().\n");
		exit(1);
	}
	/* verifica se deu erro ao fechar o semáforo */
	if(sem_unlink("/sem_ex13Full")==-1){
		perror("Error in sem_unlink().\n");
		exit(1);
	}
	/* verifica se deu erro ao fechar o semáforo */
	if(sem_unlink("/sem_ex13Empty")==-1){
		perror("Error in sem_unlink().\n");
		exit(1);
	}
	
    return 0;
}
