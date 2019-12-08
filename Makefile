CC = gcc #-mavx2
CFLAGS = -Wall -pedantic -Wextra
VECTORIZE = -ftree-vectorize -fopt-info-vec-all #-O3 -ftree-vectorizer-verbose=2 -msse2 -ffast-math
MPFLAGS = ${CFLAGS} -fopenmp
LIBS = -lm

all: PlainC/main PlainC/mainv #AVX2/main

	
PlainC/main: PlainC/o/main.o testfunctions/o/testfunctions.o
	${CC} PlainC/o/main.o testfunctions/o/testfunctions.o -o  PlainC/main ${CFLAGS} ${LIBS}

PlainC/mainv: PlainC/o/main.o testfunctions/o/testfunctions.o
	${CC} PlainC/o/main.o testfunctions/o/testfunctions.o -o  PlainC/mainv ${CFLAGS} ${LIBS} ${VECTORIZE}
 

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
	rm -f PlainC/main PlainC/mainv AVX2/main

clean: clean_intermediate clean_bin
