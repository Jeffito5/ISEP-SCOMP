#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <semaphore.h>
#include <string.h>

#define NUMBERS 200
#define PROCESSES 8

int main(int argc, char *argv[]) {
	FILE *input, *output;
	char filePermissionRead[] = "r"; 
	char filePermissionWrite[] = "a";
	sem_t *semaphores[PROCESSES];
	pid_t a;
	int linha=0, read_numbers=0, i, num=0;
	
	/* cria o semáforo com o valor = 1 */
	if (
	(semaphores[0] = sem_open("/S1", O_CREAT, 0644,1)) == SEM_FAILED||
	(semaphores[1] = sem_open("/S2", O_CREAT, 0644,0)) == SEM_FAILED||
	(semaphores[2] = sem_open("/S3", O_CREAT, 0644,0)) == SEM_FAILED||
	(semaphores[3] = sem_open("/S4", O_CREAT, 0644,0)) == SEM_FAILED||
	(semaphores[4] = sem_open("/S5", O_CREAT, 0644,0)) == SEM_FAILED||
	(semaphores[5] = sem_open("/S6", O_CREAT, 0644,0)) == SEM_FAILED||
	(semaphores[6] = sem_open("/S7", O_CREAT, 0644,0)) == SEM_FAILED||
	(semaphores[7] = sem_open("/S8", O_CREAT, 0644,0)) == SEM_FAILED) 
	{
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
			if(sem_wait(semaphores[i])==-1){
				perror("Error in sem_wait().\n");
				exit(1);
			} 
			printf("Son %d\n",i+1);      
			/* continua a ler enquanto não for end of file */
            while(!feof(input) && read_numbers<NUMBERS){
                fscanf(input, "%d", &num);
                fprintf(output, "%d\n", num);
                read_numbers++;
            }
			
			/* fecha a stream- int fclose(FILE *stream) */
            fclose(output);
            fclose(input);
			
			/* verifica se deu erro a incrementar o semáforo */
            if(i!=7){
				if(sem_post(semaphores[i+1])==-1){
					perror("Error in sem_post().\n");
					exit(1);
				}
			}
            exit(0);
        }
	}
    
	
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
	if(sem_unlink("/S1")==-1||sem_unlink("/S2")==-1||sem_unlink("/S3")==-1||
	sem_unlink("/S4")==-1||sem_unlink("/S5")==-1||sem_unlink("/S6")==-1||
	sem_unlink("/S7")==-1||sem_unlink("/S8")==-1)
	{
		perror("Error in sem_unlinkh().\n");
		exit(1);
	}

	return 0;
}
