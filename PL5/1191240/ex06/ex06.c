#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int check_prime(int num){
   int i;
   for (i=2;i<num;i++){
      if (num%i == 0){
		return 0;
	  }
   }
   if (i==num){
      return 1;
   }
   return 0;
}

void thread_func(void* arg){
	int *num =(int *)arg,i,num_prime;
	
	for(i=2;i<=*num;i++){
		num_prime=check_prime(i);
		if(num_prime){
			printf("Prime number: %d\n",i);
		}
	}
}


int main(void){
	int num;
	pthread_t thread_id;
	printf("Enter any positive number\n");
	do{
		scanf("%d",&num);
		if(num==1){
			printf("The number 1 is still subjective when it comes to prime number. \nPlease enter another number.\n");
		}
	}while(num==1);
	
	pthread_create(&thread_id,NULL,(void *)thread_func,(void*)&num);
	pthread_join(thread_id,NULL);
		
	return 0;
}
