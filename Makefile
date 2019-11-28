CC = gcc
CFLAGS = -Wall -pedantic -Wextra
LIBS = -lm

all: PlainC/main

	
PlainC/main: PlainC/o/main.o testfunctions/o/testfunctions.o
	${CC} PlainC/o/main.o testfunctions/o/testfunctions.o -o  PlainC/main ${CFLAGS} ${LIBS}


PlainC/o/main.o: PlainC/main.c 
	${CC} PlainC/main.c -o PlainC/o/main.o -c ${CFLAGS}

testfunctions/o/testfunctions.o: testfunctions/testfunctions.c testfunctions/testfunctions.h
	${CC} testfunctions/testfunctions.c -o testfunctions/o/testfunctions.o -c ${CFLAGS}


clean_intermediate:
	rm -f PlainC/o/*.o testfunctions/o/*.o

clean_bin:
	rm -f PlainC/main

clean: clean_intermediate clean_bin
