CC = gcc #-mavx2
CFLAGS = -Wall -pedantic -Wextra
VECTORIZE = -ftree-vectorize -fopt-info-vec-all #-O3 -ftree-vectorizer-verbose=2 -msse2 -ffast-math
MPFLAGS = -fopenmp
LIBS = -lm

all: PlainC/main PlainC/mainv OpenMP/main #AVX2/main

	
PlainC/main: PlainC/o/main.o testfunctions/o/testfunctions.o
	${CC} PlainC/o/main.o testfunctions/o/testfunctions.o -o  PlainC/main ${CFLAGS} ${LIBS}

PlainC/mainv: PlainC/o/main.o testfunctions/o/testfunctions.o
	${CC} PlainC/o/main.o testfunctions/o/testfunctions.o -o  PlainC/mainv ${CFLAGS} ${LIBS} ${VECTORIZE}


OpenMP/main: OpenMP/o/main.o testfunctions/o/testfunctions.o
	${CC} OpenMP/o/main.o testfunctions/o/testfunctions.o -o  OpenMP/main ${CFLAGS} ${LIBS} ${MPFLAGS}


PlainC/o/main.o: PlainC/main.c 
	${CC} PlainC/main.c -o PlainC/o/main.o -c ${CFLAGS} ${VECTORIZE}

OpenMP/o/main.o: OpenMP/main.c 
	${CC} OpenMP/main.c -o OpenMP/o/main.o -c ${CFLAGS} ${MPFLAGS}


testfunctions/o/testfunctions.o: testfunctions/testfunctions.c testfunctions/testfunctions.h
	${CC} testfunctions/testfunctions.c -o testfunctions/o/testfunctions.o -c ${CFLAGS}


clean_intermediate:
	rm -f PlainC/o/*.o AVX2/o/*.o testfunctions/o/*.o

clean_bin:
	rm -f PlainC/main PlainC/mainv OpenMP/main AVX2/main

clean: clean_intermediate clean_bin
