# Makefile
# Variables
SOURCES = reader.c writer.c
OBJFILES = reader.o writer.o
EXEC = ex1

all: writer reader

# Suffix rules
.SUFFIXES : .c .o
.SUFFIXES : .s .o

# Transform .c into .o
.c.o:
	gcc -Wall -g -c $<

# Transform .s into .o

.s.o:
	gcc -Wall -g -c $<	
	
writer.o: writer.c 
	gcc -lrt -lm -Wall -g -c writer.c

writer: writer.o
	gcc -lrt -lm -Wall -g -o writer writer.o

reader.o: reader.c
	gcc -lrt -lm -Wall -g -c reader.c
	
reader: reader.o
	gcc -lrt -lm -Wall -g -o reader reader.o

run_writer: writer
	./writer
	
run_reader: reader
	./reader
	
debug_reader: reader
	gdb -tui reader

debug_writer: writer
	gdb -tui writer
	
	
test_reader: reader
	valgrind ./reader

test_writer: writer
	valgrind ./writer

#	valgrind ./${EXEC}
	
clean:
	rm -f writer reader *~ *.o
