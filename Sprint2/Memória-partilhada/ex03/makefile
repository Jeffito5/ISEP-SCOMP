pipes: pipes.o 
	gcc -Wall pipes.o -o pipes

pipes.o: shm
	gcc -g -c -Wall pipes.c	
	
clean:
	rm *.o -f pipes shm
	
shm:
	gcc -Wall -o shm shm.c -lrt
	
run:
	./pipes
	
run2: shm
	./shm
