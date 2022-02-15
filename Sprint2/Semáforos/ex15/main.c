#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>

#define NUM_VISITORS 5
#define TIMES 20

typedef struct {
	int visitors;
	int enter;
} shared_data_type;

int main(){
	pid_t a;
	/* cria o semáforo com o valor = 1 */
    sem_t* sem;
    if((sem = sem_open("/sem_ex15", O_CREAT | O_EXCL, 0644, 1))== SEM_FAILED){
        perror("Error in sem_open().\n");
        exit(EXIT_FAILURE);
    }
	
    shared_data_type* shared_data;
	int fd, data_size = sizeof(shared_data_type);
	/* criar e abre uma zona de memória partilhada */
	fd = shm_open("/shm_ex15", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
	
	/* erro se o file descriptor retornado deu erro */
    if (fd == -1) {
        printf("Error in shm_open.\n");
        exit(EXIT_FAILURE);
    }
    
    /* em caso de erro no ftruncate */
    if (ftruncate(fd, data_size) == -1) {
        printf("Error.\n");
        exit(EXIT_FAILURE);
    }
    
    /* mapeia um objeto da zona de memória partilhada */
    shared_data = (shared_data_type*) mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    /* se der erro no mmap */
    if (shared_data == MAP_FAILED) {
        printf("Error in mmap.\n");
        exit(EXIT_FAILURE);
    }

	srand (time (NULL)*getpid());
    shared_data->enter=1;
    shared_data->visitors=0;

    a = fork();
    if(a == -1){
        printf("Couldn't create the process.\n");
        exit(EXIT_FAILURE);
    }
    
    int cont=0;
    if(a==0){
        cont=0;
        while(cont<TIMES){
            sleep(rand()%4 + 1);
            printf("Show %d\n", cont+1);
            
            /* bloqueia a memória partilhada ao decrementar o semáforo */
            sem_wait(sem); 
            printf("Visitors: %d\n", shared_data->visitors);
            /* retirar os visitantes da room */
            while (shared_data->visitors>0) {
                shared_data->visitors--;
                printf("Left 1 visitor\tRemaining visitors:%d\n", shared_data->visitors);
            };
            printf("\n");
            /* liberta a memória partilhada ao incrementar ao semáforo */
            sem_post(sem); 
            cont++;
        }
        sem_wait(sem);
        shared_data->enter=0;
        sem_post(sem);
        exit(EXIT_SUCCESS);
    }
    int flag=1;
    do {
		/* verifica se ainda podem entrar visitantes e se o show room esta aberto ao decrementar o semáforo */
        sem_wait(sem);
        if(shared_data->visitors < NUM_VISITORS && shared_data->enter==1){ 
            shared_data->visitors++;
            printf("Entered 1 visitor\tTotal visitors: %d\n",shared_data->visitors);
        }
        if(!shared_data->enter){ 
            flag=0;
            printf("Show already closed.\n");
        }
        /* incrementa o semáforo */
        sem_post(sem);
    } while (flag);

	/* fecha o descritor */
    if(sem_close(sem)==-1){
        printf("Error in sem_close()%s.\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    /* verifica se deu erro ao fechar o semáforo */
    if(sem_unlink("/sem_ex15")==-1){
        printf("Error in sem_unlink(): %s.\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* desconecta o área de memória partilhada do espaço do endereço do processo */ 
    if (munmap(shared_data, data_size) < 0){
        perror("Error in munmap.\n");
        exit(EXIT_FAILURE);
    }
    
    /* verifica se deu erro ao fechar a memória partilhada */
    if (shm_unlink("/shm_ex15") < 0) {
        perror("Error in shm_unlink().\n");
        exit(EXIT_FAILURE);
    }
    return 0;
}
