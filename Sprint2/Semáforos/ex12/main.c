#include "header.h"

int create_childs(int num){
    pid_t a;
    int i;
    for (i = 0; i < num; i++){
        a = fork();
        if (a < 0)
            return -1;
        else if (a == 0)
            return i + 1;
    }
    return 0;
}

void close_all(sem_t *sem, int fd, shared_data_type *shared_data){
	/* fecha o descritor */
    if(sem_close(sem)==-1){
        perror("Error in sem_close.\n");
        exit(EXIT_FAILURE);
    }
    
    /* desconecta o área de memória partilhada do espaço do endereço do processo */ 
    if (munmap(shared_data, DATA_SIZE) < 0){
        perror("Error in munmap.\n");
        exit(EXIT_FAILURE);
    }
	
	/* fecha o fd */
	if(close(fd) == -1) {
		perror("Error in munmap.\n");
		exit(EXIT_FAILURE);
	}
}

void clear_all(){
	 /* verifica se deu erro ao fechar a memória partilhada */
	if(shm_unlink(SHM_NAME) == -1){
		perror("Error in shm_unlink().\n");
		exit(EXIT_FAILURE);
	}
	
	/* verifica se deu erro ao fechar o semáforo */
    if(sem_unlink(SEMAPHORE_NAME)==-1){
		perror("Error in sem_unlink().\n");
		exit(EXIT_FAILURE);
	}

}

int main(){
	const int CHILD_NUM = 3;
	shared_data_type *shared_data;
	/* cria e abre uma zona de memória partilhada */
    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    
    /* erro se o file descriptor retornado deu erro */
    if(fd == -1){
		perror("Error in shm.\n");
		exit(EXIT_FAILURE);
	}

	/* Define o tamanho, inicializa a 0 e verifica se dá erro */
    if(ftruncate(fd, DATA_SIZE) == -1){
		perror("Error.\n");
	}

    /* mapeia um objeto da zona de memória partilhada */
    shared_data = (shared_data_type *)mmap(NULL, DATA_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    
    /* se der erro no mmap */
	if(shared_data == MAP_FAILED){
		perror("Error in mmap().\n");
		exit(EXIT_FAILURE);
	}
	
	/* Número de bilhetes vendidos=0 */
	shared_data->sold_tickets = 0;
	
	/* Bilhetes inicializados com a partir do número 500 */
	int i;
	for(i = 0; i < NUMBER_TICKETS; i++){
		shared_data->tickets[i] = 500+i+1;
	}
	
	/* verifica se o semáforo criado deu erro */
    sem_t *sem;
    if ((sem = sem_open(SEMAPHORE_NAME, O_CREAT, 0644, 1)) == SEM_FAILED){
        perror("Error in sem_open().\n");
        exit(EXIT_FAILURE);
    }
    
	int id = create_childs(CHILD_NUM);
	
	if(id > 0){
		client(id);
		close_all(sem, fd, shared_data);
		return 0;
	}
	
	/* Espera que os filhos terminem */
	for (i = 0; i < CHILD_NUM; i++){
		wait(NULL);
	}
	
	close_all(sem, fd, shared_data);
	clear_all();
	
	return 0;
}
