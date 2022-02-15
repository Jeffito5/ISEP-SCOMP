#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void) {
		int fd[8];
		int i, number_machines=4, pieces=0,piecesReceived=0, 
		currentPieces1=0, currentPieces2=0, piecesTransfered1=0,piecesTransfered2=0,
		piecesTransfered3=0,piecesTransfered4=0;
		pid_t childs[4];
		
		/* cria o pipe e verifica se dá erro */
		for(i = 0; i < number_machines; i++){
			if(pipe(&fd[2*i]) == -1) {
				perror("Pipe failed.\n");
			}
		}
		
		/* faz o fork e verifica se deu erro ao fazê-lo */
		childs[0] = fork();
		if (childs[0] == -1){
			printf("Error. Couldn't create the process.\n");
			return 0;
		}
		
		/* filho */
		if(childs[0] == 0){ 
			/* enquanto as peças não forem 1000 irá transferir 5 de cada vez */
			while(piecesTransfered1 != 1000){
				pieces = 5;
				piecesTransfered1 +=5;
				close(fd[2*0]);
				write(fd[2*0+1], &pieces,sizeof(int));
				printf("M1 cut 5. Sending to M2.\n");
			}
			exit(0);
		}
		
		/* faz o fork e verifica se deu erro ao fazê-lo */
		childs[1] = fork();
		if (childs[1] == -1){
			printf("Error. Couldn't create the process.\n");
			return 0;
		}
		
		if(childs[1] == 0){
			/* enquanto as peças não forem 1000 irá transferir 5 de cada vez */
			while(piecesTransfered2 != 1000){
				close(fd[2*0+1]);
				read(fd[2*0],&piecesReceived,sizeof(piecesReceived));
					
				piecesTransfered2 += piecesReceived;
					
				printf("M2 received %d pieces.\n", piecesReceived);
					
				close(fd[2*1]);
				write(fd[2*1+1],&piecesReceived ,sizeof(int));
					
				printf("M2 folded. Sending to M3.\n");
			}	
				
			exit(0);
		}
		
		/* faz o fork e verifica se deu erro ao fazê-lo */
		childs[2] = fork();
		if (childs[2] == -1){
			printf("Error. Couldn't create the process.\n");
			return 0;
		}
		
		if(childs[2] == 0){
			/* enquanto as peças não forem 1000 irá transferir 10 de cada vez */
			while(piecesTransfered3 != 1000){
				close(fd[2*1+1]);
				read(fd[2*1],&piecesReceived,sizeof(piecesReceived));
					
				piecesTransfered3 += piecesReceived;
					
				printf("M3 received %d pieces.\n", piecesReceived);
					
				currentPieces1 += piecesReceived;
						
				if(currentPieces1 == 10){
					close(fd[2*2]);
					write(fd[2*2+1],&currentPieces1 ,sizeof(currentPieces1));
					printf("M3 welded. Sending to M4.\n");
					currentPieces1 -= 10;
				}
						
			}	
			exit(0);
		}
		
		/* faz o fork e verifica se deu erro ao fazê-lo */
		childs[3] = fork();
		if (childs[3] == -1){
			printf("Error. Couldn't create the process.\n");
			return 0;
		}		

		if(childs[3] == 0){
			/* enquanto as peças não forem 1000 irá transferir 100 de cada vez */
			while(piecesTransfered4 != 1000){
				close(fd[2*2+1]);
				read(fd[2*2],&piecesReceived,sizeof(piecesReceived));
					
				piecesTransfered4 += piecesReceived;
					
				printf("M4 received %d pieces.\n", piecesReceived);
				
				currentPieces2 += piecesReceived;
					
				if(currentPieces2 == 100){
					close(fd[2*3]);
					write(fd[2*3+1],&currentPieces2 ,sizeof(currentPieces2));
					printf("M4 packed. Transfering to A1\n.");
					currentPieces2 -= 100;
					}
			}	
					
			exit(0);
		}
		
		close(fd[2*3+1]);
		read(fd[2*3],&piecesReceived,sizeof(piecesReceived));
		printf("A1 received %d pieces.\n", piecesReceived);

		return 0; 
}
