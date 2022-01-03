test: signalprocessing.so test_signalprocessing.c test_piwrapper.cpp piwrapper.so
	gcc -o test_signalprocessing test_signalprocessing.c -lm -L./ -lsignalprocessing
	g++ -o test_piwrapper test_piwrapper.cpp -L./ -lpiwrapper -fPIC
signalprocessing.so: signalprocessing.o
	gcc -shared -o libsignalprocessing.so signalprocessing.o -lm
piwrapper.so: piwrapper.o PIO.o KeyValueParser.o
	g++ -shared -o libpiwrapper.so piwrapper.o PIO.o KeyValueParser.o
piwrapper.o: piwrapper.cpp piwrapper.hpp PIO.cpp PIO.h KeyValueParser.cpp KeyValueParser.h PIEnums.h PILib.h
	g++ -c piwrapper.cpp PIO.cpp KeyValueParser.cpp -fPIC
signalprocessing.o: signalprocessing.c signalprocessing.h
	gcc -c signalprocessing.c
clean:
	rm test_signalprocessing test_piwrapper libsignalprocessing.so libpiwrapper.so *.o