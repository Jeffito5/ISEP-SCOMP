#include <string.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int valor_desponivel;
int v_pode=1;
int v_npode=0;

int main(void){
	pid_t a;
	int fd1[2],fd2[2],fd3[2];
	int betpai,tmp,status;
	pipe(fd1);
	pipe(fd2);
	pipe(fd3);
	srand((unsigned)time(NULL)*getpid());
	a=fork();
	if(a>0){
		close(fd1[0]);
		close(fd2[1]);
		close(fd3[0]);
		valor_desponivel=20;
			while(valor_desponivel>0){
				int betfilho;
				write(fd1[1],&v_pode,sizeof(int));
				close(fd1[1]);
				betpai=1+rand()%5;
				read(fd2[0],&betfilho,sizeof(int));
				printf("Bet Pai: %d; Bet Filho: %d\n",betpai,betfilho);
				close(fd2[0]);
					if(betpai==betfilho){
						valor_desponivel+=10;
					} else {
						valor_desponivel-=5;
					}
				//printf("Valor: %d\n",valor_desponivel);
				write(fd3[1],&valor_desponivel,sizeof(int));	
				close(fd3[1]);
			}
		write(fd3[1],&valor_desponivel,sizeof(int));
		close(fd3[1]);
		write(fd1[1],&v_npode,sizeof(int));
		close(fd1[1]);
		wait(&status);
	} else {
		close(fd1[1]);
		close(fd2[0]);
		close(fd3[1]);
		read(fd1[0],&tmp,sizeof(int));
		close(fd1[0]);
			if(tmp==1){
				int pode[100];
				int i=0;
				pode[i]=tmp;
					while(pode[i]==1){
						int msg,valor_novo;
						msg=1+rand()%5;
						write(fd2[1],&msg,sizeof(int));
						close(fd2[1]);
						read(fd3[0],&valor_novo,sizeof(int));
						printf("Crédito: %d\n",valor_novo);
						close(fd3[0]);
						i++;
						read(fd1[0],&pode[i],sizeof(int));
					}
			}
		close(fd1[0]);
		close(fd2[1]);
		close(fd3[0]);
		exit(EXIT_SUCCESS);		
	}
	
	return EXIT_FAILURE;
}
