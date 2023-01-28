/* Scale in array in the range of [0,1].
type: 0=minmax; 1=statistical; 2=absolute.
return: 0=no error; 1=type not found; */
extern int imgscale_double(double* img, int size, double param1, double param2, int type);
extern int imgscale_float(float* img, int size, double param1, double param2, int type);

/* Perform kmeans clustering with k++ initialization on an array.
size = x+y, where in is a 2d array of points and out is the classification for each point.
return: 0>=number of iterations; -1=k invalid; -2=invalid iterations; */
extern int kmeans_double(double* in, int* out, int size, int k, int iterations);
extern int kmeans_float(float* in, int* out, int size, int k, int iterations);

