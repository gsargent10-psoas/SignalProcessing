#include "polarimetric.h"
#include<math.h>
#include<stdlib.h>

/*** Internal Functions ***/


/*** External Functions ***/

/* Form the desired modulation pattern.
input: modulation={0,1,2,3} (2x2a,2x2b,3x2,4x2); */
int formugrid(double *s0, double *s1, double *s2, double *out_data, int width, int height, int modulation)
{
	if (modulation > 3 || modulation < 0)
		return -1;

	double a22a[4] = {0.0,45.0,135.0,90.0};
	double a22b[4] = {22.5,67.5,112.5,157.5};
	double a32[6] = {0.0,120.0,60.0,90.0,30.0,150.0};
	double a42[8] = {0.0,45.0,90.0,135.0,90.0,135.0,0.0,45.0};
	for(int k=0;k<4;k++)
	{
		a22a[k]=2.0*a22a[k]*(M_PI/180.0);
		a22b[k]=2.0*a22b[k]*(M_PI/180.0);
	}
	for(int k=0;k<6;k++)
	{
		a32[k]=2.0*a32[k]*(M_PI/180.0);
	}
	for(int k=0;k<8;k++)
	{
		a42[k]=2.0*a42[k]*(M_PI/180.0);
	}
	
	int indx=0;	
	for(int r=0;r<height;r++)
	{
		for(int c=0;c<width;c++)
		{
			indx=c+r*width;
			if(modulation==0) // 2x2a, top-left=90, top-right=135, bottom-left=45, bottom-right=0
			{
				if(r%2==1 && c%2==1) // 0, bottom-right
					out_data[indx] = 0.5*(s0[indx]+cos(a22a[0])*s1[indx] + sin(a22a[0])*s2[indx]); 	
				if(r%2==1 && c%2==0) // 45, bottom-left
					out_data[indx] = 0.5*(s0[indx]+cos(a22a[1])*s1[indx] + sin(a22a[1])*s2[indx]); 	
				if(r%2==0 && c%2==1) // 135, top-right
					out_data[indx] = 0.5*(s0[indx]+cos(a22a[2])*s1[indx] + sin(a22a[2])*s2[indx]); 	
				if(r%2==0 && c%2==0) // 90, top-left
					out_data[indx] = 0.5*(s0[indx]+cos(a22a[3])*s1[indx] + sin(a22a[3])*s2[indx]); 	
			}
			else if(modulation==1) // 2x2b, top-left=22.5, top-right=67.5, bottom-left=112.5, bottom-right=157.5
			{
				if(r%2==1 && c%2==1) // 157.5, bottom-right
					out_data[indx] = 0.5*(s0[indx]+cos(a22b[3])*s1[indx] + sin(a22b[3])*s2[indx]); 	
				if(r%2==1 && c%2==0) // 112.5, bottom-left
					out_data[indx] = 0.5*(s0[indx]+cos(a22b[2])*s1[indx] + sin(a22b[2])*s2[indx]); 	
				if(r%2==0 && c%2==1) // 67.5, top-right
					out_data[indx] = 0.5*(s0[indx]+cos(a22b[1])*s1[indx] + sin(a22b[1])*s2[indx]); 	
				if(r%2==0 && c%2==0) // 22.5, top-left
					out_data[indx] = 0.5*(s0[indx]+cos(a22b[0])*s1[indx] + sin(a22b[0])*s2[indx]); 	
			}
			else if(modulation==2) // 3x2
			{
				if(r%2==1 && c%3==1) // 30, bottom-right
					out_data[indx] = 0.5*(s0[indx]+cos(a32[4])*s1[indx] + sin(a32[4])*s2[indx]); 	
				if(r%2==1 && c%3==2) // 150, bottom-middle
					out_data[indx] = 0.5*(s0[indx]+cos(a32[5])*s1[indx] + sin(a32[5])*s2[indx]); 	
				if(r%2==1 && c%3==0) // 90, bottom-left
					out_data[indx] = 0.5*(s0[indx]+cos(a32[3])*s1[indx] + sin(a32[3])*s2[indx]); 	
				if(r%2==0 && c%3==1) // 120, top-right
					out_data[indx] = 0.5*(s0[indx]+cos(a32[1])*s1[indx] + sin(a32[1])*s2[indx]); 	
				if(r%2==0 && c%3==2) // 60, top-middle
					out_data[indx] = 0.5*(s0[indx]+cos(a32[2])*s1[indx] + sin(a32[2])*s2[indx]); 	
				if(r%2==0 && c%3==0) // 0, top-left
					out_data[indx] = 0.5*(s0[indx]+cos(a32[0])*s1[indx] + sin(a32[0])*s2[indx]); 	
			}
			else if(modulation==3) // 4x2
			{
				if(r%2==1 && c%4==1) // 135, bottom-middle-left
					out_data[indx] = 0.5*(s0[indx]+cos(a42[5])*s1[indx] + sin(a42[5])*s2[indx]); 	
				if(r%2==1 && c%4==2) // 0, bottom-middle-right
					out_data[indx] = 0.5*(s0[indx]+cos(a42[6])*s1[indx] + sin(a42[6])*s2[indx]); 	
				if(r%2==1 && c%4==3) // 45, bottom-right
					out_data[indx] = 0.5*(s0[indx]+cos(a42[7])*s1[indx] + sin(a42[7])*s2[indx]); 	
				if(r%2==1 && c%4==0) // 90, bottom-left
					out_data[indx] = 0.5*(s0[indx]+cos(a42[4])*s1[indx] + sin(a42[4])*s2[indx]); 	
				if(r%2==0 && c%4==1) // 45, top-middle-left
					out_data[indx] = 0.5*(s0[indx]+cos(a42[1])*s1[indx] + sin(a42[1])*s2[indx]); 	
				if(r%2==0 && c%4==2) // 90, top-middle-right
					out_data[indx] = 0.5*(s0[indx]+cos(a42[2])*s1[indx] + sin(a42[2])*s2[indx]); 	
				if(r%2==0 && c%4==3) // 135, top-right
					out_data[indx] = 0.5*(s0[indx]+cos(a42[3])*s1[indx] + sin(a42[3])*s2[indx]); 	
				if(r%2==0 && c%4==0) // 0.0, top-left
					out_data[indx] = 0.5*(s0[indx]+cos(a42[0])*s1[indx] + sin(a42[0])*s2[indx]); 	
			}
			else
			{
				return -1;
			}
		}
	}
	return modulation;
}

/* Compute intensity images from Stokes.
input: in_data= ; out_data=height by width with depth of 5 (so,s1,s2,dolp,aop) */
void computeintensity(double *s0,double *s1, double *s2, double *out_data, int width, int height, double angle)
{
	angle = 2.0*angle*(M_PI/180.0);
	for(int k=0;k<height*width;k++)
	{
		out_data[k] = 0.5*(s0[k]+cos(angle)*s1[k] + sin(angle)*s2[k]); 	
	}
}

/* Return the number of sub images based on full image size, desired sub image size, and overlap. */
int getNumberSubImages22(int image_x, int image_y, int sub_x, int sub_y, int overlap)
{
	return 0;
}

/* The stitching algroithim */
void Stitching(){}

/* Generate sub images from full size image. Assume image is 2-dimensional.
status: 0 = succes, 1 = success, but number of subimages generated does not equal num_sub
-1 overlap is too large, -2 subimage is too large */
void formSubImage22(double* image, int image_rows, int image_cols, double* sub_images, int sub_rows, int sub_cols, int overlap, int num_sub, int* error)
{

}
