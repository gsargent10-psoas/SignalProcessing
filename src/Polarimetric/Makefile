polarimetric.so: polarimetric.o
	gcc -shared -o libpolarimetric.so polarimetric.o -lm -O2 -s -DNDEBUG
polarimetric.o: polarimetric.c polarimetric.h
	gcc -c polarimetric.c
clean:
	rm libpolarimetric.so *.o
