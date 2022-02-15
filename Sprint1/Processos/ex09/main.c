#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void){
	int i;
	int* status;
	pid_t a;
	int y;
	for(i=0;i<10;i++){
		a=fork();
		if(a==0){
			for(y=0;y<100;y++){
				printf("%d\n",(i*100+y));
			}
		}else{
			waitpid(a, &status, 0);
		}
	}
	return EXIT_SUCCESS;
} 
// Não podemos garantir uma vez que não sabemos a ordem de cração de processos. 
// Sendo assim 2 processos podem estar a imprimir ao mesmo tempo.
