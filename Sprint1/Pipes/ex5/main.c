#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_SIZE 256

void upper(char *str);

int main(void) {

	int fd1[2], fd2[2];
	
	pid_t a;
	
	/* cria o pipe e verifica se dá erro */
	if(pipe(fd1) == -1){  	
		perror("Pipe failed.\n");
		exit(0);
	}
	
	/* cria o pipe e verifica se dá erro */
	if(pipe(fd2) == -1){  	
		perror("Pipe failed.\n");
		exit(0);
	}
	
	/* faz o fork e verifica se deu erro ao fazê-lo */
	a = fork();
	if (a==-1){
		perror("Error. Couldn't create the process.\n");
		exit(0);
	}

	/* Processo filho -> Cliente */
	
	if(a == 0) { 
		
		/* se não houve erros, após a escrita tenta fechar a extremidade. Retorna
		* a mensagem informativa se não conseguiu fechar */
		if (close(fd1[1]) == -1){
			perror("Error. Couldn't close the writing end.\n");
			return 1;
		}  
		
		/* se não houve erros, após a leitura tenta fechar a extremidade. Retorna
		* a mensagem informativa se não conseguiu fechar */
		if (close(fd2[0]) == -1){
			perror("Error. Couldn't close the reading end.\n");
			return 1;
		}
		
		char vec[MAX_SIZE];
		
		printf("Introduza a frase:\n");
		
		/* retorna a mensagem informativa em caso de erro na leitura */
		if(read(fd1[0], &vec, MAX_SIZE * sizeof(char))==-1){
			perror("Error. Couldn't read.\n");
			return 1;
		}
		
		/* se não houve erros, após a leitura tenta fechar a extremidade. Retorna
		* a mensagem informativa se não conseguiu fechar */
		if (close(fd1[0]) == -1){
			perror("Error. Couldn't close the reading end.\n");
			return 1;
		}
		
		/* função para trocar o case da frase */
		upper(vec);
		
		/* retorna a mensagem informativa em caso de erro na escrita */
		if(write(fd2[1], &vec, MAX_SIZE * sizeof(char))==-1){
			perror("Error. Couldn't write.\n");
			return 1;
		}
		
		/* se não houve erros, após a escrita tenta fechar a extremidade. Retorna
		* a mensagem informativa se não conseguiu fechar */
		if (close(fd2[1]) == -1){
			perror("Error. Couldn't close the writing end.\n");
			return 1;
		}
		
	}
	/* Processo pai -> Server */
	if(a>0){
		
		/* se não houve erros, após a leitura tenta fechar a extremidade. Retorna
		* a mensagem informativa se não conseguiu fechar */
		if (close(fd1[0]) == -1){
			perror("Error. Couldn't close the reading end.\n");
			return 1;
		}
		
		/* se não houve erros, após a escrita tenta fechar a extremidade. Retorna
		* a mensagem informativa se não conseguiu fechar */
		if (close(fd2[1]) == -1){
			perror("Error. Couldn't close the writing end.\n");
			return 1;
		}
		
		char vec[MAX_SIZE];
		
		/* lê o que o utilizador escreveu */
		fgets(vec, MAX_SIZE * sizeof(char), stdin); 
		
		/* retorna a mensagem informativa em caso de erro na escrita */
		if(write(fd1[1], &vec, MAX_SIZE * sizeof(char))==-1){
			perror("Error. Couldn't write.\n");
			return 1;
		}
		
		/* se não houve erros, após a escrita tenta fechar a extremidade. Retorna
		* a mensagem informativa se não conseguiu fechar */
		if (close(fd1[1]) == -1){
			perror("Error. Couldn't close the writing end.\n");
			return 1;
		}
		
		/* retorna a mensagem informativa em caso de erro na leitura */
		if(read(fd2[0], &vec, MAX_SIZE * sizeof(char))==-1){
			perror("Error. Couldn't read.\n");
			return 1;
		}
		
		/* se não houve erros, após a leitura tenta fechar a extremidade. Retorna
		* a mensagem informativa se não conseguiu fechar */
		if (close(fd2[0]) == -1){
			perror("Error. Couldn't close the reading end.\n");
			return 1;
		}
		
		/* frase convertida */	
		printf("Converted string: %s\n", vec);
	
	}
	
	return 0;
}

/* método para trocar o case da frase */
void upper(char *str) {
	int i;
	for(i = 0; i < strlen(str); i++) {
		if(str[i] > 96 && str[i] < 123)
			str[i] -= 32;
	}
}
