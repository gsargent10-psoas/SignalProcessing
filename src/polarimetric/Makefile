polarimetric.so: polarimetric.o
	gcc -shared -o libpolarimetric.so polarimetric.o -lm
	cp libpolarimetric.so ../Test
	install --mode=444 polarimetric.h ../Test
	cp libpolarimetric.so ../../../Deep-Learning-Infrastructure/src/python
	install --mode=444 polarimetric.py ../../../Deep-Learning-Infrastructure/src/python
	cp libpolarimetric.so ../../../Deep-Learning-GUI/src/python
	install --mode=444 polarimetric.py ../../../Deep-Learning-GUI/src/python
polarimetric.o: polarimetric.c polarimetric.h
	gcc -c polarimetric.c
clean:
	rm libpolarimetric.so *.o
