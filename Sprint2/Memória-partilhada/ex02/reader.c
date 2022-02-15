#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define BUFFER_SIZE 100
typedef struct{
	float price;
	int code;
	char description[BUFFER_SIZE];
} product_info;


int main(void){
	int fd;
	int size_product_info=sizeof(product_info);
	product_info *shared_product;
	fd=shm_open("/ex02",O_RDWR,S_IRUSR|S_IWUSR);
	if(fd==-1){
		perror("Erro ao abrir o ficheiro.");
		return -1;
	}
	if((ftruncate(fd,size_product_info))==-1){
		perror("Eror ao fazer ftruncate.");
	}
	shared_product=(product_info *) mmap(NULL, size_product_info,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	printf("Code: %d\n",shared_product->code);
	printf("Description: %s\n",shared_product->description);
	printf("Price %.2f\n",shared_product->price);
	return 0;
}
