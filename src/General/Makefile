test: test_signalprocessing.c signalprocessing.so
	gcc -o test_signalprocessing test_signalprocessing.c -lm -L./ -lsignalprocessing -O2 -s -DNDEBUG
signalprocessing.so: signalprocessing.o
	gcc -shared -o libsignalprocessing.so signalprocessing.o -lm -O2 -s -DNDEBUG
signalprocessing.o: signalprocessing.c signalprocessing.h 
	gcc -c signalprocessing.c
clean:
	rm test_signalprocessing libsignalprocessing.so *.o
