#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define ARRAY_SIZE 5
#define NAME_GRADE_ARRAY_SIZE 100

typedef struct {
    int number;
    char name[NAME_GRADE_ARRAY_SIZE];
    int grade;
} shared_data_type;

void* print(void* arg) {
    shared_data_type* argument=(shared_data_type*)arg;
    printf("Number: %d\n", argument->number);
    printf("Name: %s\n", argument->name);
    printf("Grade: %d\n", argument->grade);
    printf("\n");
	/* garante que os dados já foram escritos no disco */
    fflush(stdout);
    pthread_exit((void*)NULL);
}

/* método que sorteia um número do aluno */
int random_number(){
	int num;
	num=(rand()%10)+50;
	return num;
}

/* método que sorteia uma nota */
int random_grade(){
	int grade;
	grade=(rand()%20)+1;
	return grade;
}

int main(int argc, char* agrv[]) {
	/* array de estrutura */
    shared_data_type array[ARRAY_SIZE];
    pthread_t threads[ARRAY_SIZE];
	int i;
	/* array auxiliar para guardar o nome */
	char name2[10];
	/* pointer que aponta para o primeiro elemento do array */
	char *ptr=&name2[0];
	
    for(i=0;i<ARRAY_SIZE;i++){
        array[i].number=random_number();
        printf("Write a name (0-10 letters):");
        scanf("%s", ptr);
        printf("\n");
        strcpy(array[i].name, ptr);
		array[i].grade=random_grade();
    }
	
	/* cria 5 threads que vão executar a função print sendo o argumento o array de estrutura com os 3 elementos */
    for (i=0;i<ARRAY_SIZE;i++) {
        pthread_create(&threads[i], NULL, print, (void*)&array[i]);
    }
	
	/* espera que cada thread termine */
    for (i=0;i<ARRAY_SIZE;i++) {
        pthread_join(threads[i], NULL);
    }

    return EXIT_SUCCESS;
}
