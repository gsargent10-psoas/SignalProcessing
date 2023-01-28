test: test_stitching.cpp libpolarimetric.so libpiwrapper.so
	g++ -o test_stitching test_stitching.cpp -L./  -lpolarimetric -lpiwrapper -fPIC
clean:
	rm test_stitching *.o *.so
