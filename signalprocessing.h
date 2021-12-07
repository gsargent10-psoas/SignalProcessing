// type: 0=minmax; 1=statistical; 2=absolute.
extern void imgscale(double* img, int size, double param1, double param2, int type);

extern void kmeans(double* in, double* out, int size);

extern void knn(double* in, double* out, int size, int k);