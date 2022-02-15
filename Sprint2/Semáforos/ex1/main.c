#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h> 
#include <semaphore.h>
#include <string.h>
#include <time.h>

#define NUMBERS 200
#define PROCESSES 8

int main(int argc, char *argv[]) {
	FILE *input, *output;
	char filePermissionRead[] = "r"; 
	char filePermissionWrite[] = "a";
	sem_t *sem;
	pid_t a;
	int linha=0, read_numbers=0, i, num=0;
	
	/* cria o semáforo com o valor = 1 */
	if ((sem = sem_open("/sem_ex1", O_CREAT, 0644,1)) == SEM_FAILED) {
        perror("Error in sem_open()");
		exit(1);
    }
    /* abre os ficheiros (Numbers.txt and Output.txt) usando o filePermissionRead e filePermissionWrite. */
    input = fopen("Numbers.txt", filePermissionRead);
    output = fopen("Output.txt", filePermissionWrite);
	
	/* Verifica se consegue abrir o ficheiro */
    if (input == NULL || output == NULL){
		printf("Couldn't open the file.\n");
        exit(1);
    }
	
    for (i=0;i<PROCESSES;i++){
		/* faz o fork e verifica se deu erro ao fazê-lo */
		a = fork();
		if (a < 0){
			perror("Error. Couldn't create the process.\n");
			exit(1);
		}
		if (a == 0){
			/* verifica se deu erro ao decrementar o semáforo */
			if(sem_wait(sem)==-1){
				perror("Error in sem_wait().\n");
				exit(1);
			}       
			/* continua a ler enquanto não for end of file */
            while(!feof(input) && read_numbers<(i+1)*NUMBERS){
                fscanf(input, "%d", &num);
                fprintf(output, "%d\n", num);
                read_numbers++;
            }
			
			/* fecha a stream- int fclose(FILE *stream) */
            fclose(output);
            fclose(input);
			
			/* verifica se deu erro a incrementar o semáforo */
            if(sem_post(sem)==-1){
				perror("Error in sem_post().\n");
				exit(1);
			}
            exit(0);
        }
    }
	
	/* espera pelos outros filhos */
	for (i=0;i<PROCESSES;i++){
        wait(NULL);            
    }
    
    /* abre o ficheiro */
    output = fopen("Output.txt", filePermissionRead);
	
	/* enquanto não for end of file coloca os números nesse ficheiro */
    while(!feof(output)){
        fscanf(output, "%d", &num);
        printf("%d -> %d\n",linha,num);
        linha++;
    }
	
	/* fecha */
    fclose(output);
	
	/* verifica se deu erro ao fechar o semáforo */
	if(sem_unlink("/sem_ex1")==-1){
		perror("Error in sem_unlink().\n");
		exit(1);
	}
    return 0;
}
