#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#define STRING_SIZE 200
#define ARRAY_SIZE 5
typedef struct {
	int number;
	char name[STRING_SIZE];
	int grade;
} aluno;



void thread_func(){
	
}


int main(void){
	int i;
	srand(time(NULL));
	pthread_t thread_id[ARRAY_SIZE];
	aluno alunos[ARRAY_SIZE];
	strcpy(alunos[0].name,"Danilton");
	strcpy(alunos[1].name,"Luis");
	strcpy(alunos[2].name,"Marisa");
	strcpy(alunos[3].name,"Delvis");
	strcpy(alunos[4].name,"Victor");
	
	for(i=0;i<ARRAY_SIZE;i++){
		alunos[i].number=i+1;
		alunos[i].grade=rand()%20;
	}
	
	//int num[]={1,2,3,4,5};
	
	
	
	for(i=0;i<ARRAY_SIZE;i++){
		pthread_create(&thread_id[i],NULL,(void *)thread_func,NULL);
		printf("Aluno #%d\n",i+1);
		printf("Num: %d\n",alunos[i].number);
		printf("Nome: %s\n",alunos[i].name);
		printf("Nota: %d\n",alunos[i].grade);
		pthread_join(thread_id[i],NULL);
	}
	
	
	return 0;
}
