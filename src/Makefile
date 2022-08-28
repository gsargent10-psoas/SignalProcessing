test: test_signalprocessing.c signalprocessing.so
	gcc -o test_signalprocessing test_signalprocessing.c -lm -L./ -lsignalprocessing
signalprocessing.so: signalprocessing.o
	gcc -shared -o libsignalprocessing.so signalprocessing.o -lm
	cp libsignalprocessing.so ../../Deep-Learning-GUI/src/python
	cp libsignalprocessing.so ../../Deep-Learning-Infrastructure/src/python
	install --mode=444 signalprocessing.py ../../Deep-Learning-GUI/src/python
	install --mode=444 signalprocessing.py ../../Deep-Learning-Infrastructure/src/python
signalprocessing.o: signalprocessing.c signalprocessing.h
	gcc -c signalprocessing.c
clean:
	rm test_signalprocessing libsignalprocessing.so *.o
