/* type: 0=minmax; 1=statistical; 2=absolute.
   return: 0=no error; 1=type not found; */
extern int imgscale(double* img, int size, double param1, double param2, int type);

/* size = x+y, where in is a 2d array of points and out is the classification for each point.
   return: 0>=number of iterations; -1=k invalid; -2=invalid iterations; */
extern int kmeans(double* in, int* out, int size, int k, int iterations);

extern int knn(double* in, double* out, int size, int k);
