test: signalprocessing.o test.o
	gcc -shared -o libsignalprocessing.so signalprocessing.o -lm
	gcc -c test.c
	gcc -o test test.o -lm -L. -lsignalprocessing
lib: signalprocessing.o
	gcc -shared -o libsignalprocessing.so signalprocessing.o -lm
signalprocessing.o: signalprocessing.c signalprocessing.h
	gcc -c signalprocessing.c
clean:
	rm test libsignalprocessing.so *.o