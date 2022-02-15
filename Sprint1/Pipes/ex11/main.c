#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#define MAX_SIZE 1000 

/* método para calcular o maior número */
int isGreater(int num1, int num2) {
	if(num1>=num2) {
		return num1;
	} else
		return num2;
}

int main(){
	pid_t a;
	int i, status, children = 5, number_processes = 6, parent_number, 
	child_number, read_number, biggest, num;
	int fd[number_processes][2];
	
	parent_number = 1+(rand() % 500);
	printf("Parent's PID: %d = %d\n", getpid(), parent_number);

	/* cria o pipe e verifica se dá erro */
	for(i = 0; i < number_processes; i++) {
		if(pipe(fd[i]) == -1) {
			perror("Pipe failed\n");
			return 1;
		}
	}
	
	for(i=0;i<children;i++){
		child_number = rand() % 500 + 1;
		
		/* faz o fork e verifica se deu erro ao fazê-lo */
		a = fork();
		if (a == -1){
			printf("Error. Couldn't create the process.\n");
			return 0;
		}
		
		/* filho */
		if(a==0){ 
			printf("Children's PID: %d = %d\n", getpid(), child_number);
			
			/* se não houve erros, após a escrita tenta fechar a extremidade. Retorna
			* a mensagem informativa se não conseguiu fechar */
			if (close(fd[i][1]) == -1){
				perror("Error. Couldn't close the writing end.\n");
				return 1;
			}  
			
			read(fd[i][0], &read_number, sizeof(read_number));
			
			/* se não houve erros, após a leitura tenta fechar a extremidade. Retorna
			* a mensagem informativa se não conseguiu fechar */
			if (close(fd[i][0])== -1){
				perror("Error. Couldn't close the reading end.\n");
				return 1;
			}
			
			/* verifica se é o maior número */	
			num = isGreater(child_number, read_number);
			
			write(fd[i+1][1], &num, sizeof(num));
			
			
			/* se não houve erros, após a escrita tenta fechar a extremidade. Retorna
			* a mensagem informativa se não conseguiu fechar */
			if (close(fd[i+1][1]) == -1){
				perror("Error. Couldn't close the writing end.\n");
				return 1;
			}  
			
			exit(0);
		}

	}
	write(fd[0][1], &parent_number, sizeof(parent_number));
	/* se não houve erros, após a escrita tenta fechar a extremidade. Retorna
	* a mensagem informativa se não conseguiu fechar */
	if (close(fd[0][1]) == -1){
		perror("Error. Couldn't close the writing end.\n");
		return 1;
	}  
	
	/* espera pelos filhos */
	wait(&status); 
	
	read(fd[5][0], &biggest, sizeof(biggest));
	
	/* se não houve erros, após a leitura tenta fechar a extremidade. Retorna
	* a mensagem informativa se não conseguiu fechar */
	if (close(fd[5][0])== -1){
		perror("Error. Couldn't close the reading end.\n");
		return 1;
	}

	printf("Biggest number= %d\n", biggest);

	return 0;
}
