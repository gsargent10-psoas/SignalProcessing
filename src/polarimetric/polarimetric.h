/* Form the desired modulation pattern.
input: modulation={0,1,2,3} (2x2a,2x2b,3x2,4x2); */
int formugrid(double *s0, double *s1, double *s2, double *out_data, int width, int height, int modulation);

/* Compute intensity images from Stokes.
input: in_data= ; out_data=height by width with depth of 5 (so,s1,s2,dolp,aop) */
void computeintensity(double *s0,double *s1, double *s2, double *out_data, int width, int height, double angle);

/* Return the number of sub images based on full image size, desired sub image size, and overlap. */
int getNumberSubImages22(int image_y, int image_x, int sub_y, int sub_x, int overlap);
    

/* The stitching algroithim */
void Stitching();

//ONLY EDIT THE VERSION IN POLARIMETIC FOLDER
//ONLY EDIT THE VERSION IN POLARIMETIC FOLDER
//ONLY EDIT THE VERSION IN POLARIMETIC FOLDER
//DOUBLE CHECK


//code


//ONLY EDIT THE VERSION IN POLARIMETIC FOLDER
//ONLY EDIT THE VERSION IN POLARIMETIC FOLDER
//ONLY EDIT THE VERSION IN POLARIMETIC FOLDER
//DOUBLE CHEC


/* Generate sub images from full size image. Assume image is 2-dimensional.
status: 0 = succes, 1 = success, but number of subimages generated does not equal num_sub
-1 overlap is too large, -2 subimage is too large */

void formSubImage22(double* image, int image_rows, int image_cols, double* sub_images, int sub_rows, int sub_cols, int overlap, int num_sub, int* error);

