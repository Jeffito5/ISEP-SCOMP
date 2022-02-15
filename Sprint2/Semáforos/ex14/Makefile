closeServer: reader
	gcc -Wall -g -o closeServer closeServer.c -lpthread -lrt
	
reader: writer
	gcc -Wall -g -o reader reader.c -lpthread -lrt
	
writer: server
	gcc -Wall -g -o writer writer.c -lpthread -lrt

server: 
	gcc -Wall -g -o server server.c -lpthread -lrt
	
clean:
	rm *.o -f server closeServer writer reader
	
run_server: closeServer
	./server

run_close:
	./closeServer

run_reader:
	./reader
	
run_writer:
	./writer

