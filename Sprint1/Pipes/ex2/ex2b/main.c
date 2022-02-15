#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 50

/* criação da estrutura */
typedef struct{
	int number;
	char string[BUFFER_SIZE];
} values;

int main(void) {
	int fd[2];
	pid_t a;
	values instance;

	/* cria o pipe e verifica se dá erro */
	if(pipe(fd) == -1){  	
		perror("Pipe failed.\n");
		return 1;
	}
	
	/* faz o fork e verifica se deu erro ao fazê-lo */
	a = fork();
	if (a < 0){
		perror("Error. Couldn't create the process.\n");
		return 1;
	}
	
	/* se for o processo pai então ele vai fechar a posição relativa à leitura
	* uma vez que irá escrever. Se der erro retorna a mensagem informativa */
	if (a > 0){
		if (close(fd[0]) == -1){
			perror("Error. Couldn't close the reading end.\n");
			return 1;
		}  
		
		/* lê o inteiro e armazena-o */
		printf("Please send an integer:");
		scanf ("%d", &instance.number);
		
		/* clean the buffer */
		while((getchar()) != '\n');

		/* lê a frase e armazena-a */
		printf("Please send a string:");
		fgets(instance.string,sizeof(instance.string), stdin); 
		
		/* retorna a mensagem informativa em caso de erro na escrita do número */		
		if(write(fd[1],&instance,sizeof(instance))==-1){
			perror("Error. Wrong number.\n");
			return 1;
		}
		
		/* se não houve erros, após a escrita tenta fechar a extremidade. Retorna
		* a mensagem informativa se não conseguiu fechar */
		if (close(fd[1]) == -1){
			perror("Error. Couldn't close the writing end.\n");
			return 1;
		}  
		
	}else {
		/* se não houve erros, após a escrita tenta fechar a extremidade. Retorna
		* a mensagem informativa se não conseguiu fechar */
		if (close(fd[1]) == -1){
			perror("Error. Couldn't close the writing end.\n");
			return 1;
		}  
		
		/* retorna a mensagem informativa em caso de erro na leitura do número */
		if(read(fd[0], &instance, sizeof(instance))==-1){
			perror("Error. Couldn't read the number.\n");
			return 1;
		}
		
		/* se não houveram erros então o filho leu a informação do pipe passada pelo pai */
		printf("Child process read the number %d\n",instance.number);
		printf("Child process read the string %s\n", instance.string);

		/* se não houve erros, após a leitura tenta fechar a extremidade. Retorna
		* a mensagem informativa se não conseguiu fechar */
		if (close(fd[0]) == -1){
			perror("Error. Couldn't close the reading end.\n");
			return 1;
		}
	}
	return 0;
}
