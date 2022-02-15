#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char const *argv[]) {
	int vec1[1000], vec2[1000], fd[5][2], result[1000];
	pid_t a;
	int i, j, x, total = 0, soma = 0;

	/*Preenchimento dos vetores*/
	for (i = 0; i < 1000; ++i)	{
		vec1[i] = rand() % 10;
		vec2[i] = rand() % 10;
	}
	
	/* cria o pipe e verifica se dá erro */
	for (i = 0; i < 5; i++)	{
		if (pipe(fd[i]) == -1)	{	
			printf("Pipe failed.\n");
			return 0;
		}
		
		/* faz o fork e verifica se deu erro ao fazê-lo */
		a = fork();
		if (a == -1){
			printf("Error. Couldn't create the process.\n");
			return 0;
		}
	
		/* filho */
		/* vai fazer a soma dos elementos de 200 em 200 */
		if (a == 0)	{
			for (j = (i*200); j < 200 + (i*200); j++){
				total = vec1[j] + vec2[j];
				write(fd[i][1], &total, sizeof(int));
			}
			
			/* se não houve erros, após a escrita tenta fechar a extremidade. Retorna
			* a mensagem informativa se não conseguiu fechar */
			if (close(fd[i][1]) == -1){
				perror("Error. Couldn't close the writing end.\n");
				return 1;
			}  
			exit(1);
			
			/* pai */
			} else { 
			wait(&x);	
			
			/* se não houve erros, após a escrita tenta fechar a extremidade. Retorna
			* a mensagem informativa se não conseguiu fechar */
			if (close(fd[i][1]) == -1){
				perror("Error. Couldn't close the writing end.\n");
				return 1;
			}  
			
			/* lê os valores que vieram do filho e escreve-os no vetor final imprimindo-os de 200 em 200 */
			for (j = (i*200); j < 200 + (i*200); j++){
				read(fd[i][0], &soma, sizeof(int));
				result[i] = soma;
				printf("Total: %d\n", result[i]);	
			}
			printf("\n");
		}
		
		/* se não houve erros, após a leitura tenta fechar a extremidade. Retorna
		* a mensagem informativa se não conseguiu fechar */
		if (close(fd[i][0])== -1){
			perror("Error. Couldn't close the reading end.\n");
			return 1;
		}	
	}
	return 0;
}
