son: ex06
	gcc -Wall -o son son.c -lrt

ex06: pipes
	gcc -Wall -o ex06 ex06.c -lrt
	
pipes: pipes.o 
	gcc -Wall pipes.o -o pipes

pipes.o: pipes.c
	gcc -g -c -Wall pipes.c	
	
clean:
	rm *.o -f ex06 son pipes
	
run: son
	./ex06

run2:
	./son
	
runPipe: pipes
	./pipes
