#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h> 
#include <semaphore.h>
#include <string.h>
#include <time.h>

#define STRING_NUMBER 50
#define STRING_SIZE 80
#define WAIT_TIME 12

typedef struct{
	char strings[STRING_NUMBER][STRING_SIZE];
	int stringNumber;
} shared_data_type;

int main(int argc, char *argv[]) {
	int fd, data_size = sizeof(shared_data_type);
	fd = shm_open("/shm_test", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
	shared_data_type *shared_data = (shared_data_type*) mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);
	
	if(fd == -1){
		perror("Error in shm.\n");
		return 1;
	}
	
	/* em caso de erro no ftruncate */
	if (ftruncate (fd, data_size)==-1){
		perror("Error.");
		return 1;
	}
	
	/* create semapore with value = 1 */
	sem_t *sem;
	if ((sem = sem_open("/sem_ex4", O_CREAT, 0644,1)) == SEM_FAILED) {
        perror("Error in sem_open().\n");
        exit(1);
    }
	
	time_t t;
	srand((unsigned) time(&t));

	if(sem_wait(sem)==-1){
		perror("Error in sem_wait().\n");
		exit(1);
	}       
	
	char string[STRING_SIZE];
	
	int index = shared_data -> stringNumber;
	shared_data -> stringNumber++;
	
	sprintf(string,"I’m the Father - with PID %d\n", (int)getpid());
	strncpy(shared_data -> strings[index], string, STRING_SIZE);
	
	printf("%s", string);
	printf("Write in index %d\n", index);
	
	if(sem_post(sem)==-1){
		perror("Error in sem_post().\n");
		exit(1);
	}
	
	int number_of_seconds = rand() % (WAIT_TIME+1);
	printf("Wait time: %d\n",number_of_seconds);
	sleep(number_of_seconds);
	
	return 0;
}

