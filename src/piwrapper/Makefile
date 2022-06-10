test: test_piwrapper.cpp piwrapper.so
	g++ -o test_piwrapper test_piwrapper.cpp -L./ -lpiwrapper -fPIC
piwrapper.so: piwrapper.o PIO.o KeyValueParser.o
	g++ -shared -o libpiwrapper.so piwrapper.o PIO.o KeyValueParser.o
	cp libpiwrapper.so ../Test
	cp piwrapper.hpp ../Test
piwrapper.o: piwrapper.cpp piwrapper.hpp PIO.cpp PIO.h KeyValueParser.cpp KeyValueParser.h PIEnums.h PILib.h
	g++ -c piwrapper.cpp PIO.cpp KeyValueParser.cpp -fPIC
clean:
	rm test_piwrapper libpiwrapper.so *.o
