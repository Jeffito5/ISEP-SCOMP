#include <stdio.h>  
#include <stdlib.h>    
#include <time.h>      

int random_number(){
	int random_n;
	random_n=1 + rand() % 19;
	return random_n;
}