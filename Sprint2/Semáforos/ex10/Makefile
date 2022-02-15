server: consult
	gcc -Wall -g -o server server.c -lpthread -lrt

consult: insert
	gcc -Wall -g -o consult consult.c -lpthread -lrt

insert: consultall
	gcc -Wall -g -o insert insert.c -lpthread -lrt

consultall:
	gcc -Wall -g -o consultall consultall.c -lpthread -lrt

clean:
	rm *.o -f server insert consult consultall

run: server
	./server


