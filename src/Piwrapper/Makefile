test: test_piwrapper.cpp piwrapper.so
	g++ -o test_piwrapper test_piwrapper.cpp -L./ -lpiwrapper -fPIC -O2 -s -DNDEBUG
piwrapper.so: piwrapper.o PIO.o KeyValueParser.o
	g++ -shared -o libpiwrapper.so piwrapper.o PIO.o KeyValueParser.o -O2 -s -DNDEBUG
piwrapper.o: piwrapper.cpp piwrapper.hpp PIO.cpp PIO.h KeyValueParser.cpp KeyValueParser.h PIEnums.h PILib.h
	g++ -c piwrapper.cpp PIO.cpp KeyValueParser.cpp -fPIC
clean:
	rm test_piwrapper libpiwrapper.so *.o
copy:
	install --mode=444 ../../../../asl_library/KeyValueParser.h .
	install --mode=444 ../../../../asl_library/KeyValueParser.cpp .
	install --mode=444 ../../../../asl_library/PIO.h .
	install --mode=444 ../../../../asl_library/PIO.cpp .
	install --mode=444 ../../../../asl_library/PILib.h .
	install --mode=444 ../../../../asl_library/PIEnums.h .
