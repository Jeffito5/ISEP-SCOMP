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

#define CHILD_NUMBER 3
#define SEMAPHORE_NUMBER 3
#define PROCESS_NUMBER 2
int main(void) {
	int i;
	/* coloca os processos em array de chars */
	char* p1[PROCESS_NUMBER] = {"Sistemas ","a "};
	char* p2[PROCESS_NUMBER] = {"de ","melhor "};
	char* p3[PROCESS_NUMBER] = {"Computadores -","disciplina!\n"};
	
	/* cria 3 semáforos para os 3 processos */
	sem_t *sem[SEMAPHORE_NUMBER];
	char* phrase[SEMAPHORE_NUMBER] =  {"sem_ex7P1","sem_ex7P2","sem_ex7P3"};
	int values[SEMAPHORE_NUMBER] = {1,0,0};
	
	for(i = 0; i < SEMAPHORE_NUMBER; i++){
		/* cria um semáforo com o valores do array values verifica se deu erro */
		if ((sem[i] = sem_open(phrase[i], O_CREAT, 0644,values[i])) == SEM_FAILED) {
			perror("Error in sem_open().\n");
			exit(1);
		}
    }
    pid_t a;
    
    for(i = 0; i < CHILD_NUMBER; i++){
		a = fork();
		int j;
		/* processo filho */
		if(a == 0 && i == 0){
			for(j = 0; j < PROCESS_NUMBER; j++){
				/* verifica se deu erro ao decrementar o semáforo no primeiro processo sendo este 0 */
				if(sem_wait(sem[0])==-1){
					perror("Error in sem_wait().\n");
					exit(1);
				}
				/* imprime os elementos do processo p1 */
				printf("%s\n",p1[j]);
				/* verifica se deu erro a incrementar o semáforo P2 sendo este 1*/
				if(sem_post(sem[1])==-1){
					perror("Error in sem_post().\n");
					exit(1);
				}
			}
			exit(0);
		}
		
		if(a == 0 && i == 1){
			for(j = 0; j < PROCESS_NUMBER; j++){
				/* verifica se deu erro ao decrementar o semáforo no segundo processo.
				 * É possível decrementá-lo uma vez que P1 incrementou-o anteriormente */
				if(sem_wait(sem[1])==-1){
					perror("Error in sem_wait().\n");
					exit(1);
				}
				/* imprime os elementos do processo p2 */
				printf("%s\n",p2[j]);
				/* verifica se deu erro a incrementar o semáforo p3 */
				if(sem_post(sem[2])==-1){
					perror("Error in sem_post().\n");
					exit(1);
				}
			}
			exit(0);
		}
		
		if(a == 0 && i == 2){
			for(j = 0; j < PROCESS_NUMBER; j++){
				/* verifica se deu erro ao decrementar o semáforo no terceiro processo.
				 * É possível decrementá-lo uma vez que P2 incrementou-o anteriormente */
				if(sem_wait(sem[2])==-1){
					perror("Error in sem_wait().\n");
					exit(1);
				}
				/* imprime os elementos do processo p3 por ordem */
				printf("%s\n",p3[j]);
				/* verifica se deu erro a incrementar o semáforo o semáforo p1 para
				 * todo este processo voltar a realizar-se */
				if(sem_post(sem[0])==-1){
					perror("Error in sem_post().\n");
					exit(1);
				}
			}
			exit(0);
		}
	}
	
	/* espera pelos outros filhos */
	for(i = 0; i < CHILD_NUMBER; i++){
		wait(NULL);
	}
	
	/* verifica se deu erro ao fechar os semáforos */	
	for(i = 0; i < CHILD_NUMBER; i++){	
		if(sem_unlink(phrase[i])==-1){
			perror("Error in sem_unlink().\n");
			exit(1);
		}
	}
	
	return 0;
}
