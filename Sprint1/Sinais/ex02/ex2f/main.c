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
	printf("Digite o número do sinal que pretende enviar.\n");
	scanf("%d", &num); 
	while(num!=0){
		qualquer_num=kill(num_pid, num);
		if(qualquer_num==-1){
			printf("Erro.\n");
		}
		printf("Digite o número do sinal que pretende enviar.\n");
		printf("Se pretender encerrar digite 0.\n");
		scanf("%d", &num); 
	}	
	return 0;
} 
