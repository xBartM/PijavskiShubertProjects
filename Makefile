CC = gcc #-mavx2
CFLAGS = -Wall -pedantic -Wextra
VECTORIZE = -ftree-vectorize -fopt-info-vec-all #-O3 -ftree-vectorizer-verbose=2 -msse2 -ffast-math
MPFLAGS = -fopenmp
LIBS = -lm

all: PlainC/main PlainC/mainv PlainCNonRecursive/main OpenMP/main OpenMPNonRecursive/main 

	
PlainC/main: PlainC/o/main.o testfunctions/o/testfunctions.o
	${CC} PlainC/o/main.o testfunctions/o/testfunctions.o -o  PlainC/main ${CFLAGS} ${LIBS}

PlainC/mainv: PlainC/o/main.o testfunctions/o/testfunctions.o
	${CC} PlainC/o/main.o testfunctions/o/testfunctions.o -o  PlainC/mainv ${CFLAGS} ${LIBS} ${VECTORIZE}


PlainCNonRecursive/main: PlainCNonRecursive/o/main.o testfunctions/o/testfunctions.o
	${CC} PlainCNonRecursive/o/main.o testfunctions/o/testfunctions.o -o  PlainCNonRecursive/main ${CFLAGS} ${LIBS}


OpenMP/main: OpenMP/o/main.o testfunctions/o/testfunctions.o
	${CC} OpenMP/o/main.o testfunctions/o/testfunctions.o -o  OpenMP/main ${CFLAGS} ${LIBS} ${MPFLAGS}

OpenMPNonRecursive/main: OpenMPNonRecursive/o/main.o testfunctions/o/testfunctions.o
	${CC} OpenMPNonRecursive/o/main.o testfunctions/o/testfunctions.o -o  OpenMPNonRecursive/main ${CFLAGS} ${LIBS} ${MPFLAGS} 


PlainC/o/main.o: PlainC/main.c 
	${CC} PlainC/main.c -o PlainC/o/main.o -c ${CFLAGS} ${VECTORIZE}

PlainCNonRecursive/o/main.o: PlainCNonRecursive/main.c 
	${CC} PlainCNonRecursive/main.c -o PlainCNonRecursive/o/main.o -c ${CFLAGS} ${VECTORIZE}

OpenMP/o/main.o: OpenMP/main.c 
	${CC} OpenMP/main.c -o OpenMP/o/main.o -c ${CFLAGS} ${MPFLAGS}

OpenMPNonRecursive/o/main.o: OpenMPNonRecursive/main.c 
	${CC} OpenMPNonRecursive/main.c -o OpenMPNonRecursive/o/main.o -c ${CFLAGS} ${MPFLAGS}


testfunctions/o/testfunctions.o: testfunctions/testfunctions.c testfunctions/testfunctions.h
	${CC} testfunctions/testfunctions.c -o testfunctions/o/testfunctions.o -c ${CFLAGS}


clean_intermediate:
	rm -f PlainC/o/*.o AVX2/o/*.o testfunctions/o/*.o

clean_bin:
	rm -f PlainC/main PlainC/mainv PlainCNonRecursive/main OpenMP/main OpenMPNonRecursive/main AVX2/main

clean: clean_intermediate clean_bin
