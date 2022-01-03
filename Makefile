test: signalprocessing.o test.o
	gcc -shared -o libsignalprocessing.so signalprocessing.o -lm
	gcc -c test.c
	gcc -o test_signalprocessing test.o -lm -L./ -lsignalprocessing
	g++ -shared -o libpiwrapper.so piwrapper.o PIO.o KeyValueParser.o
	g++ -o test_piwrapper test_piwrapper.cpp -L./ -lpiwrapper -fPIC
sig_lib: signalprocessing.o
	gcc -shared -o libsignalprocessing.so signalprocessing.o -lm
pi_lib: piwrapper.o
	g++ -shared -o piwrapper.so piwrapper.o
signalprocessing.o: signalprocessing.c signalprocessing.h
	gcc -c signalprocessing.c
piwrapper.o: piwrapper.cpp piwrapper.hpp PIO.cpp PIO.h
	g++ -c piwrapper.cpp PIO.cpp KeyValueParser.cpp -fPIC
clean:
	rm test_signalprocessing test_piwrapper libsignalprocessing.so libpiwrapper.so *.o