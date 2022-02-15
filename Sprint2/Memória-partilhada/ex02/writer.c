#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 200
typedef struct{
	float price;
	int code;
	char description[BUFFER_SIZE];
} product_info;

int main(void){
	int fd;
	int size_product_info=sizeof(product_info);
	product_info *shared_product;
	
	fd=shm_open("/ex02",O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR);
	ftruncate(fd,size_product_info);
	
	shared_product = (product_info *)mmap(NULL,size_product_info,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	shared_product->code=85;
	shared_product->price=85.5;
	char msg[BUFFER_SIZE]="Chocolate: Belo dia";
	strcpy(shared_product->description,(msg));
	return 0;
}
