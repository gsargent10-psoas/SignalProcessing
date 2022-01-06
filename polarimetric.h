/* input: modulation={0,1,2,3} (2x2a,2x2b,3x2,4x2); */
int formugrid(double *s0, double *s1, double *s2, double *out_data, int width, int height, int modulation);

void computeintensity(double *s0,double *s1, double *s2, double *out_data, int width, int height, double angle);
