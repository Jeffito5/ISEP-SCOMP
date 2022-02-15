#include "header.h"

int seller(int client_id){
	printf("Seller: Attending client %d.\n", client_id);
	
	shared_data_type *shared_data;
	
	/* Abre a memória partilhada já criada (sem o O_CREAT) */
    int fd = shm_open(SHM_NAME, O_RDWR, S_IRUSR | S_IWUSR);
    
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
	
	/* Verifica se ainda existem bilhetes. Caso não existam, retorna -1 */
	int ticket_number;
	if(shared_data->sold_tickets == NUMBER_TICKETS){
		ticket_number = -1;
		printf("Seller: There's no more tickets.\n");
	}else{
		ticket_number = shared_data->tickets[shared_data->sold_tickets];
		printf("Seller: Sold the ticket %d.\n", ticket_number);
		shared_data->sold_tickets++;	
	}

	/* desconecta o área de memória partilhada do espaço do endereço do processo */ 
	if (munmap(shared_data, DATA_SIZE) == -1) {
		perror("Error in munmap.\n");
		exit(EXIT_FAILURE);
	}
	/* fecha o fd */
	if(close(fd) == -1) {
		perror("Error in close.\n");
		exit(EXIT_FAILURE);
	}
	
	return ticket_number;
}
