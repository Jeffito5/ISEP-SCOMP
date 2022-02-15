#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#include <fcntl.h>

#define ARRAY_SIZE 1000
#define MAX_VALUES 10
#define CHILDREN 10

int main(void) {
	int maxArray[MAX_VALUES];
	srand((unsigned) time(NULL));
	int i,j,max;
	int num[ARRAY_SIZE];
	
	for(i = 0; i < ARRAY_SIZE; i++){
		num[i]= rand() % (1000 + 1);
	}
	
	pid_t a[CHILDREN];
	int fd[2];
	
	/* cria o pipe e verifica se dá erro */
	if(pipe(fd) == -1){  	
		perror("Pipe failed.\n");
		return 1;
	}
	
	for(i = 0; i < CHILDREN; i++){
		/* faz o fork e verifica se deu erro ao fazê-lo */
		a[i] = fork();
		if (a[i] < 0){
			perror("Error. Couldn't create the process.\n");
			exit(EXIT_FAILURE);
		}
		
		/* filho */
		if(a[i] == 0){
			max = 0;
			for(j = i*100; j < (i*100+100); j++){
				if(num[j] > max){
					max = num[j];
				}
			}
			/* Vai fechar a posição relativa à leitura uma vez que irá escrever. 
			 * Se der erro retorna a mensagem informativa */
			if (close(fd[0]) == -1){
				perror("Error. Couldn't close the reading end.\n");
				return 1;
			}  
			
			write(fd[1], &max, sizeof(int));
	
			/* se não houve erros, após a escrita tenta fechar a extremidade. Retorna
			* a mensagem informativa se não conseguiu fechar */
			if (close(fd[1]) == -1){
				perror("Error. Couldn't close the writing end.\n");
				return 1;
			}  
			exit(0);
		}
	}
	
	/* Vai fechar a posição relativa à escrita uma vez que irá ler. 
	 * Se der erro retorna a mensagem informativa */
	if (close(fd[1]) == -1){
		perror("Error. Couldn't close the writing end.\n");
		return 1;
	}  
	
	for(i = 0; i < CHILDREN; i++){
		wait(NULL);
		read(fd[0], &maxArray[i],sizeof(int));
	}
	
	/* se não houve erros, após a leitura tenta fechar a extremidade. Retorna
	* a mensagem informativa se não conseguiu fechar */
	if (close(fd[0]) == -1){
		perror("Error. Couldn't close the reading end.\n");
		return 1;
	}  
	
	int biggest = 0;
	for(i = 0; i < MAX_VALUES; i++){
		if(maxArray[i] > biggest){
			biggest = maxArray[i];
		}
	}
	
	printf("Biggest: %d \n", biggest);
			
	return 0; 
}
