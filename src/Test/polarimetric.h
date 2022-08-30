/* Form the desired modulation pattern.
input: modulation={0,1,2,3} (2x2a,2x2b,3x2,4x2); */
int formugrid_double(double *s0, double *s1, double *s2, double *out_data, int width, int height, int modulation);
int formugrid_float(float *s0, float *s1, float *s2, float *out_data, int width, int height, int modulation);

/* Compute intensity images from Stokes.
input: in_data= ; out_data=height by width with depth of 5 (so,s1,s2,dolp,aop) */
void computeintensity_double(double *s0,double *s1, double *s2, double *out_data, int width, int height, double angle);
void computeintensity_float(float *s0,float *s1, float *s2, float *out_data, int width, int height, double angle);

/* Generate sub images from full size image. Assume image is 2-dimensional.
status: 0 = succes, 1 = success, but number of subimages generated < num_sub, 2 = success, but number of subimages generated > num_sub,
-1 overlap is too large, -2 subimage is too large */
int getNumberSubImages22(int image_y, int image_x, int sub_y, int sub_x, int overlap);
    

/* The stitching algroithim */
int stitchsubimages_double(double* image, int image_rows, int image_cols, double* sub_images, int sub_rows, int sub_cols, int overlap, int num_sub);
int stitchsubimages_float(float* image, int image_rows, int image_cols, float* sub_images, int sub_rows, int sub_cols, int overlap, int num_sub);


/* Generate sub images from full size image. Assume image is 2-dimensional.
status: 0 = succes, 1 = success, but number of subimages generated does not equal num_sub
-1 overlap is too large, -2 subimage is too large */
int formSubImage22_double(double* image, int image_rows, int image_cols, double* sub_images, int sub_rows, int sub_cols, int overlap, int num_sub);
int formSubImage22_float(float* image, int image_rows, int image_cols, float* sub_images, int sub_rows, int sub_cols, int overlap, int num_sub);

