#include <errno.h>
#include <fcntl.h>
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
#include <pthread.h>

#define SIZE 8
#define NUMBER_OF_THREADS 10

/* matrizes para dar fill e matriz da multiplicação */
int matrix1[SIZE][SIZE];
int matrix2[SIZE][SIZE];
int mul_matrix[SIZE][SIZE];

void *fill_matrix1(void *arg){
    int i, j;
    for (i=0; i<SIZE; i++){
        for(j=0; j<SIZE; j++){
            matrix1[i][j]=i+1;
        }
    }
    pthread_exit((void *)NULL);
}

void *fill_matrix2(void *arg){
	int i, j;	
    for (i=0; i<SIZE; i++){
        for(j=0; j<SIZE; j++){
            matrix2[i][j]=i+2;
        }
    }
    pthread_exit((void *)NULL);
}

void *multiply_row(void *arg){
	/* pointer line é um apontador para o array posições para ir da coluna 0 à 7 */
    int *line=(int *)arg;
    /* i será o pointer */
    int i=*line;
    int j, k;
    /* o i dentro dos arrays serve para fazer cumprir as 8 threads criadas */
    for (j=0; j<SIZE; j++){
        for (k=0; k<SIZE; k++){
            mul_matrix[i][j]+=matrix1[i][k]*matrix2[k][j];
        }
    }
    pthread_exit((void *)NULL);
}

void print_matrix1(){
    int i, j;
    for (i=0; i<SIZE; i++){
        for (j=0; j<SIZE; j++){
            printf("%d ", matrix1[i][j]);
        }
        printf("\n");
    }
}

void print_matrix2(){
    int i, j;
    for (i=0; i<SIZE; i++){
        for (j=0; j<SIZE; j++){
            printf("%d ", matrix2[i][j]);
        }
        printf("\n");
    }
}

void print_final_matrix(){
    int i, j;
    for (i=0; i<SIZE; i++){
        for (j=0; j<SIZE; j++){
            printf("%d ", mul_matrix[i][j]);
        }
        printf("\n");
    }
}

int main(){
    pthread_t thread_id[2];
	
	/* cria uma thread para preencher a matriz 1 */
    pthread_create(&thread_id[0], NULL, fill_matrix1, (void *)NULL);
    
	/* cria uma thread para preencher a matriz 2 */
    pthread_create(&thread_id[1], NULL, fill_matrix2, (void *)NULL);
	
	/* espera que a thread do preenchimento da matriz 1 termine */
    pthread_join(thread_id[0], NULL);
	
	/* espera que a thread do preenchimento da matriz 2 termine */
    pthread_join(thread_id[1], NULL);
	
	printf("Matrix 1:\n");
    print_matrix1();
    printf("\n");
    printf("Matrix 2:\n");
    print_matrix2();
    printf("\n");
	
    pthread_t thread_mul[SIZE];

    int i;
    int position[8];
    /* cria 8 threads para tratarem da multiplicação das matrizes sendo o argumento o array com 8 posições */
    for (i=0; i<SIZE; i++){
        position[i]=i;
        pthread_create(&thread_mul[i], NULL, multiply_row, (void *)&position[i]);
    }
	
	/* espera que cada thread termine */
    for (i=0; i<SIZE; i++){
        pthread_join(thread_mul[i], NULL);
    }
	
	printf("Final Matrix:\n");
    print_final_matrix();
    return 0;
}
