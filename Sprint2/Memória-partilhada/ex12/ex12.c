#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>
#define STR_SIZE 50
#define NR_DISC 10
typedef struct {
	int numero;
	char nome[STR_SIZE];
	int disciplinas[NR_DISC];
	int dataWrite;
}aluno;

int main(void){
	pid_t p;
	int fd,status, i, size_shared=sizeof(aluno);
	aluno *aluno_dados;
	fd = shm_open("/aluno_data",O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR);
	ftruncate(fd,size_shared);
	aluno_dados =(aluno *) mmap(NULL,size_shared,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	aluno_dados->dataWrite=0;
	p=fork();
	if(p<0){
		perror("Fork doesn't work!\n");
		return EXIT_FAILURE;
	}
	if(p==0){
		int max=0,min=20,sum=0;
		while(aluno_dados->dataWrite!=1);
		for(i=0;i<NR_DISC;i++){
			if(aluno_dados->disciplinas[i]>max){
				max=aluno_dados->disciplinas[i];
			}
			if(aluno_dados->disciplinas[i]<min){
				min=aluno_dados->disciplinas[i];
			}
			sum+=aluno_dados->disciplinas[i];
		}
		float average=(float)sum/NR_DISC;
		printf("Name: %s\nNumber: %d\n",aluno_dados->nome,aluno_dados->numero);
		printf("The highest grade: %d\n",max);
		printf("The lowest grade: %d\n",min);
		printf("The average of the grades: %.2f\n",average);
		exit(EXIT_SUCCESS);
		
		
	}else{
		printf("Insert the student's number please: \n");
		scanf("%d",&aluno_dados->numero);
		printf("Insert the student's name: \n");
		char name[30];
		scanf("%s",name);
		strcpy(aluno_dados->nome,name);
		for(i=0;i<NR_DISC;i++){
			
			printf("Write the grade(and press enter): ");
			scanf("%d",&aluno_dados->disciplinas[i]);
					
		}
		aluno_dados->dataWrite=1;
		wait(&status);
	}
	close(fd);
	shm_unlink("/aluno_data");
	
	return 0;
}
