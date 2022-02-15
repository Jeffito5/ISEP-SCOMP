#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>

typedef struct{
    int num;
}processes;

int main(){
    sem_t *semParent;
    sem_t *semChild;
    processes *ptr;
    /* criar e abre uma zona de memória partilhada */
    int fd=shm_open("/ex9", O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR);
    
    /* erro se o file descriptor retornado deu erro */
    if(fd==-1){
        perror("Error in shm.\n");
        fd = shm_open("/ex9", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
    }
    
    /* em caso de erro no ftruncate */
    if(ftruncate(fd, sizeof(int))){
        perror("Error.\n");
        exit(EXIT_FAILURE);
    }
    
    /* mapeia um objeto da zona de memória partilhada */
    ptr = (processes*) mmap(NULL,sizeof(int),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    
    /* se der erro no mmap */
    if (ptr == MAP_FAILED) {
        perror("Error in mmap().\n");
        exit(EXIT_FAILURE);
    }
    
    /* verifica se o semáforo do pai criado com o valor 0 deu erro */
    if ((semParent = sem_open("sem_ex9Parent", O_CREAT | O_EXCL, 0777,0)) == SEM_FAILED){
        perror("Error in sem_open().\n");
        sem_unlink("sem_ex9Parent");
        exit(EXIT_FAILURE);
    }
    
    /* verifica se o semáforo do filho criado com o valor 1 deu erro */
    if ((semChild = sem_open("sem_ex9Child", O_CREAT | O_EXCL, 0777,1)) == SEM_FAILED){
        perror("Error in sem_open().\n");
        sem_unlink("sem_ex9Child");
        exit(EXIT_FAILURE);
    }
    
    ptr->num=0;
    
    /* processo filho */
    if (fork() == 0){
		/* decrementa o valor do semáforo do filho */
		sem_wait(semChild);
		ptr->num++;
		printf("Buy Chips.\n");
		
		if(ptr->num >= 2){
			/* se ambos já compraram então incrementa para o semáforo do pai */
			sem_post(semParent);
		}
		/* incrementa para o semáforo do filho */
		sem_post(semChild);
		/* decrementa o do pai */
		sem_wait(semParent);
		printf("Eat and Drink.\n");
		/* incrementa o do pai */
		sem_post(semParent);
		exit(0);
	}else{
		/* decrementa o do filho */
		sem_wait(semChild);
		ptr->num++;
        printf ("Buy Beer.\n");
        
		if(ptr->num >= 2){
			/* se ambos já compraram então incrementa para o semáforo do pai */
			sem_post(semParent);
		}
		/* incrementa o do filho */
		sem_post(semChild);
		/* decrementa o do pai */
		sem_wait(semParent);
		printf("Eat and Drink.\n");
		/* incrementa o do pai */
		sem_post(semParent);
		wait(NULL);
	}
	
	/* fecha o descritor */
    if(sem_close(semParent)==-1){
        perror("Error in sem_close.\n");
        exit(EXIT_FAILURE);
    }
    
    /* verifica se deu erro ao fechar o semáforo */
    if(sem_unlink("sem_ex9Parent")==-1){
		perror("Error in sem_unlink().\n");
		exit(EXIT_FAILURE);
	}
	
	/* fecha o descritor */
    if(sem_close(semChild)==-1){
        perror("Error in sem_close.\n");
        exit(EXIT_FAILURE);
    }
    
    /* verifica se deu erro ao fechar o semáforo */
    if(sem_unlink("sem_ex9Child")==-1){
		perror("Error in sem_unlink().\n");
		exit(EXIT_FAILURE);
	}
	
	/* desconecta o área de memória partilhada do espaço do endereço do processo */ 
    if (munmap(ptr, sizeof(processes)) < 0){
        perror("Error in munmap.\n");
        exit(EXIT_FAILURE);
    }
    
    /* verifica se deu erro ao fechar a memória partilhada */
    if(shm_unlink("/ex9")==-1){
        perror("Error in shm_unlink().\n");
        exit(-1);
    }
    return 0;
}

