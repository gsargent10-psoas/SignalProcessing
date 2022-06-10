polarimetric.so: polarimetric.o
	gcc -shared -o libpolarimetric.so polarimetric.o -lm
	cp libpolarimetric.so ../Test
	cp polarimetric.h ../Test
polarimetric.o: polarimetric.c polarimetric.h
	gcc -c polarimetric.c
clean:
	rm libpolarimetric.so *.o
