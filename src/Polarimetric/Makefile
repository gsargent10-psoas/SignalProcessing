test: test_stitching.cpp libpolarimetric.so
	g++ -o test_stitching test_stitching.cpp -L./  -lpolarimetric -fPIC -O2 -s -DNDEBUG
polarimetric.so: polarimetric.o
	gcc -shared -o libpolarimetric.so polarimetric.o -lm -O2 -s -DNDEBUG
polarimetric.o: polarimetric.c polarimetric.h
	gcc -c polarimetric.c
clean:
	rm test_stitching libpolarimetric.so *.o
