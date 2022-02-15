#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

int main() {
	int num_pid, qualquer_num;
	int num;
	printf("Digite o PID do processo\n");
	scanf("%d", &num_pid);
	printf("Pretende continuar (1), suspender (2), encerrar o processo (3) ou sair (0)?\n");
	scanf("%d", &num); 
	while(num!=0){
		switch(num){
			case 1: qualquer_num=kill(num_pid, 18);
						break;
			case 2: qualquer_num=kill(num_pid, 19);
						break;
			case 3: qualquer_num=kill(num_pid, 9);
						break;
		}
		if(qualquer_num==-1){
			printf("Erro.\n");
		}
		printf("Pretende continuar (1), suspender (2), encerrar o processo (3) ou sair (0)?\n");
		scanf("%d", &num); 
	}	
	return 0;
} 
