#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>

void spawn_childs(int n){
	pid_t a;
	int i;
	int status;
	for(i=0;i<n;i++){
			a=fork();
			if(a==0)
			exit((i+1));
		else{
			waitpid(a,&status,0);
			printf("ID=%d\n",WEXITSTATUS(status));
		}
	}
	printf("ID=0\n");
}

int main(void){
	
	spawn_childs(6);
	return EXIT_SUCCESS;

}
