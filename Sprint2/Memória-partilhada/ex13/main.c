#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <string.h>

#define CHILDREN 10
#define LINE_SIZE 100
#define WORD_SIZE 50

typedef struct{
	char path[CHILDREN][LINE_SIZE];
	char word[CHILDREN][WORD_SIZE];
	int occurrences[CHILDREN];
} shared_data_type;

int main(void) {
	int i, fd, data_size = sizeof(shared_data_type);
	shared_data_type *shared_data;
	/* cria e abre a zona de memória partilhada */
	fd = shm_open("/ex13", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
	
	/* em caso de erro no shm_open */
	if (fd == -1){
		perror("Error.");
		return 1;
	}
	
	/* se não der erro no ftruncate define o tamanho da área de memória */
	if (ftruncate (fd, data_size)==-1){
		perror("Error.");
		return 1;
	}
	
	/* pointer para a memória partilhada */
	shared_data = (shared_data_type*) mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);
	
	for(i = 0; i < CHILDREN; i++){
		printf("Insert the path to the file.");
		scanf("%s", shared_data->path[i]);
		
		printf("Insert the word to be found.");
		scanf("%s", shared_data->word[i]);
		
		printf("\n");
	}

	pid_t a;
	
	for(i = 0; i < CHILDREN; i++){
		a = fork();
		
		if (a < 0){
			perror("Error in creating the process.\n");
			return 1;
		}
		
		/* processo filho */
		if (a == 0){
			/* abre o ficheiro partilhado na memória partilhada com a permissão de "read" */
			FILE *file = fopen(shared_data->path[i], "r");
			
			if(file != NULL){
				/* array para a linha */
				char line[LINE_SIZE];
				/* pointer para a posição */
				char *pos;
				int index;
				shared_data->occurrences[i] = 0;
				
				/*fgets(char *str, int n, FILE *stream)- reads a line from the 
				 * specified stream and stores it into the string pointed to by str */
				while(fgets(line, LINE_SIZE, file) != NULL){
					index = 0;
					/* enquanto que o pointer pos que é igual ao pointer devolvido
					 * por strstr (shared_data->word[i] encontrada em line) não for nulo */
					while((pos = strstr(line + index, shared_data->word[i])) != NULL){
						/* tenta encontrar outra palavra na mesma frase */
						index = (pos - line) + 1;
						shared_data->occurrences[i]++;
					}	
				}
			} else {
				printf("File not found.\n");
				exit(0);
			}
			
			exit(0);
		}
	}
	
	/* espera pelo resto dos filhos */
	for(i = 0; i < CHILDREN; i++){
		wait(NULL);
	}
	
	/* imprime as vezes que a palavra foi encontrada */
	for(i = 0; i < CHILDREN; i++){
		if(shared_data->occurrences[i] > 1){
			printf("Child %d found the word %s %d times.\n",i+1,
			shared_data->word[i], shared_data->occurrences[i]);
		} else {
			printf("Child %d found the word %s %d time.\n",i+1,
			shared_data->word[i], shared_data->occurrences[i]);
		} 
	} 
	
	/* verifica se deu erro ao fechar a memória partilhada */
    if(shm_unlink("/ex13")==-1){
        perror("Error in shm_unlink().\n");
        exit(-1);
    }

	return 0; 
}
