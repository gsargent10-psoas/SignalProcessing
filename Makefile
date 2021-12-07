all: signalprocessing.c signalprocessing.h
	gcc -shared -o libsignalprocessing.so signalprocessing.c -lm
	gcc test.c -lm -L. -lsignalprocessing