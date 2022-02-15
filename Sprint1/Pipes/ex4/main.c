#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main(void){
	int fd[2];
	FILE *file = NULL;
	char buffer[80];
	pid_t a;
	char filePermission[] = "r"; 
	
	/* cria o pipe e verifica se dá erro */
	if(pipe(fd) == -1){  	
		perror("Pipe failed.\n");
		return 1;
	}
	
	/* faz o fork e verifica se deu erro ao fazê-lo */
	a = fork();
	if (a < 0){
		perror("Error. Couldn't create the process.\n");
		exit(EXIT_FAILURE);
	}
	
	/* se for o processo pai então ele vai fechar a posição relativa à leitura 
	 * e abre o ficheiro. Se não houver ficheiro com aquele nome envia uma mensagem
	 * informativa */
	if (a > 0){
		close(fd[0]);
        file = fopen("teste.txt",filePermission);
        if (file==NULL){
			printf("Nao existe ficheiro com esse nome.\n");
        }
        while(!feof(file)){
            fread(buffer, strlen(buffer)+1, sizeof(char), file);
            write(fd[1],buffer,strlen(buffer)+1);
        }
        fclose(file);
        close(fd[1]);
        wait(NULL);
	}else{
        close(fd[1]);
        while(read(fd[0],buffer,strlen(buffer)+1)!=0){
            printf("%s\n",buffer);
        }
        fclose(file);
        close(fd[0]);
        exit(0);
    }
	return 0;
}
