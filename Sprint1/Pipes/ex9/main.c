#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <time.h>  
#define ARRAY_SIZE 50000
#define CHILDREN 10

/* criação da estrutura */
typedef struct{
    int customer_code;
    int product_code;
    int quantity;
}sales;

int main(void){
	int fd[2];
	pid_t a;
	sales sales[ARRAY_SIZE];
	int status,i,j=0,k=0;
	int products[ARRAY_SIZE];
	srand((unsigned) time(NULL));
	
	/* preenchimento dos campos da estrutura */
	for(i=0;i<ARRAY_SIZE;i++){
		sales[i].customer_code = rand() % 10;
		sales[i].product_code = i;
		sales[i].quantity = rand() % 30;
	}
	
	/* cria o pipe e verifica se dá erro */
	if(pipe(fd) == -1){
		perror("Pipe failed.\n");
		return 1;
	}
	
	for(i=0; i<10; i++){
		/* faz o fork e verifica se deu erro ao fazê-lo */
		a=fork();
		if(a<0){
			perror("Error. Couldn't create the process.\n");
			return 1;
		}
		
		/* se for o processo filho então ele vai fechar a extremidade da leitura. 
		 * Se der erro retorna a mensagem informativa */
		if(a == 0){
			if (close(fd[0]) == -1){
				perror("Error. Couldn't close the reading end.\n");
				return 1;
			} 
			/* anda no for de 5000 em 5000. Se a quantidade for superior a 20 envia o código do produto ao pai */ 
			for(j = (ARRAY_SIZE/CHILDREN) * i; j < (ARRAY_SIZE/CHILDREN) * (i+1); j++){
				if(sales[j].quantity > 20){
					write(fd[1], &(sales[j].product_code), sizeof(sales[j].product_code));
				}
			}
			
			/* se não houve erros, após a escrita tenta fechar a extremidade. Retorna
			* a mensagem informativa se não conseguiu fechar */
			if (close(fd[1]) == -1){
				perror("Error. Couldn't close the writing end.\n");
				return 1;
			}  
			exit(0);
		}	
	}
	
	/* se não houve erros, após a escrita tenta fechar a extremidade. Retorna
	* a mensagem informativa se não conseguiu fechar */
	if (close(fd[1]) == -1){
			perror("Error. Couldn't close the writing end.\n");
			return 1;
		}  
	while(read(fd[0],&(sales[j].product_code),sizeof(sales[j].product_code)) > 0){
		products[k] = sales[j].product_code;
		k++;
	}
	
	/* se não houve erros, após a leitura tenta fechar a extremidade. Retorna
	* a mensagem informativa se não conseguiu fechar */
	if (close(fd[0]) == -1){
			perror("Error. Couldn't close the reading end.\n");
			return 1;
		}  
	for(i = 0; i < CHILDREN; i++){
		wait(&status);
	}
	printf("Products who have sold more than 20 units in a single sale:\n");
	for(i = 0; i < k; i++){
		printf("%d.\n",i);
		printf("Customer Code:%d\n", products[i]);
	}

	return 0;
}
