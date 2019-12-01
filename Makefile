CC = gcc #-mavx2
CFLAGS = -Wall -pedantic -Wextra
LIBS = -lm

all: PlainC/main #AVX2/main

	
PlainC/main: PlainC/o/main.o testfunctions/o/testfunctions.o
	${CC} PlainC/o/main.o testfunctions/o/testfunctions.o -o  PlainC/main ${CFLAGS} ${LIBS}

# AVX2/main: AVX2/o/main.o
	# ${CC} AVX2/o/main.o -o  AVX2/main ${CFLAGS} ${LIBS}


PlainC/o/main.o: PlainC/main.c 
	${CC} PlainC/main.c -o PlainC/o/main.o -c ${CFLAGS}

# AVX2/o/main.o: AVX2/main.c 
	# ${CC} AVX2/main.c -o AVX2/o/main.o -c ${CFLAGS}


testfunctions/o/testfunctions.o: testfunctions/testfunctions.c testfunctions/testfunctions.h
	${CC} testfunctions/testfunctions.c -o testfunctions/o/testfunctions.o -c ${CFLAGS}


clean_intermediate:
	rm -f PlainC/o/*.o AVX2/o/*.o testfunctions/o/*.o

clean_bin:
	rm -f PlainC/main AVX2/main

clean: clean_intermediate clean_bin
