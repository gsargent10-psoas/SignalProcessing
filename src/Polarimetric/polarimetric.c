#include "polarimetric.h"
#include<math.h>
#include<stdlib.h>
#include<stdio.h>

#define STEP 12

/*** Internal Functions ***/

/* Increment the step count for sub image generation and stitching for 2x2 pattern. */
void stepgeneral(int *sp, int *ep, int width, int overlap_factor)
{
	/* sp = starting pixel, ep = ending pixel, width = width of image, overlap = desired overlap */
	*sp = *ep-(2*STEP*overlap_factor)+1;
	if(*sp % STEP != 0){ printf("Starting pixel: %d\n",*sp); } // need to always start on an even pixel (changed to +1 instead of -1)	
	*ep = *sp + width - 1;
}

/*** External Functions ***/

/* Form the desired modulation pattern.
input: modulation={0,1,2,3} (2x2a,2x2b,3x2,4x2); */
int formugrid_double(double *s0, double *s1, double *s2, double *out_data, int width, int height, int modulation)
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
/* Form the desired modulation pattern.
input: modulation={0,1,2,3} (2x2a,2x2b,3x2,4x2); */
int formugrid_float(float *s0, float *s1, float *s2, float *out_data, int width, int height, int modulation)
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
void computeintensity_double(double *s0,double *s1, double *s2, double *out_data, int width, int height, double angle)
{
	angle = 2.0*angle*(M_PI/180.0);
	for(int k=0;k<height*width;k++)
	{
		out_data[k] = 0.5*(s0[k]+cos(angle)*s1[k] + sin(angle)*s2[k]); 	
	}
}
/* Compute intensity images from Stokes.
input: in_data= ; out_data=height by width with depth of 5 (so,s1,s2,dolp,aop) */
void computeintensity_float(float *s0,float *s1, float *s2, float *out_data, int width, int height, double angle)
{
	angle = 2.0*angle*(M_PI/180.0);
	for(int k=0;k<height*width;k++)
	{
		out_data[k] = 0.5*(s0[k]+cos(angle)*s1[k] + sin(angle)*s2[k]); 	
	}
}

/* Return the number of sub images based on full image size, desired sub image size, and overlap. */
int getNumberSubImages(int image_y, int image_x, int sub_y, int sub_x, int overlap_factor)
{
	int x = (int)ceil(((double)image_x/(double)(sub_x-2*STEP*overlap_factor)));
	int y = (int)ceil(((double)image_y/(double)(sub_y-2*STEP*overlap_factor)));
	return x*y;
}

/* The stitching algroithim */
int stitchsubimages_float(float* image, int image_rows, int image_cols, float* sub_images, int sub_rows, int sub_cols, int overlap_factor, int num_sub){
	int xs = 0; int xe = sub_cols -1;
	int ys = 0; int ye = sub_rows -1;
	
	int sxs = 0; 
	int sys = 0; 

	int index = 0;
	int count = 0;
	int subindex = 0;
	int error=-99;

	for (; ys < image_rows-1;){
		xs = 0;
		xe = sub_cols -1;
		for (; xs < image_cols-1;){
			if(count >= num_sub){
				error = 2;
				return error;
			}
			else if(xs >= image_cols-1 || ys >= image_rows-1){
				error = 1;	
				return error;
			
			}
			
			//top left corner 
			else if (xs == 0 && ys == 0){
				sys = 0;
				for (int r = ys; r <= ye-STEP*overlap_factor; r++, sys++){
					sxs = 0;
					for (int c = xs; c <= xe-STEP*overlap_factor; c++, sxs++){
						index = c+r*image_cols;
						//subindex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
						subindex = sys*sub_rows*num_sub+sxs*num_sub+count;
						if (subindex >= sub_cols*sub_rows*num_sub)
						{
							error = subindex;	
							return error;
						}
						image[index] = sub_images[subindex]; 
					}
				}	
			}
			
			//top edge
			else if (ys == 0 && xe <= image_cols-1){
				sys = 0;
				for (int r = ys; r <= ye-STEP*overlap_factor; r++, sys++){
					sxs = 0+STEP*overlap_factor;
					for (int c = xs+STEP*overlap_factor; c <= xe-STEP*overlap_factor; c++, sxs++){
						index = c+r*image_cols;
						//subindex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
						subindex = sys*sub_rows*num_sub+sxs*num_sub+count;
						if (subindex >= sub_cols*sub_rows*num_sub)
						{
							error = subindex;	
							return error;
						}
						image[index] = sub_images[subindex]; 
					}
				}	
			}
			
			//top right corner
			else if(ys == 0 && xe > image_cols-1){
				sys = 0;
				for (int r = ys; r <= ye-STEP*overlap_factor; r++, sys++){
					sxs = 0+STEP*overlap_factor;
					for (int c = xs+STEP*overlap_factor; c <= xe-STEP*overlap_factor; c++, sxs++){
						if(c > image_cols-1){
							//do nothing
						}
						else{
							index = c+r*image_cols;
							//subindex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
							subindex = sys*sub_rows*num_sub+sxs*num_sub+count;
							if (subindex >= sub_cols*sub_rows*num_sub)
							{
								error = subindex;	
								return error;
							}
							image[index] = sub_images[subindex]; 
						}
					}
				}
			}
			//left edge
			else if(xs == 0 && ye <= image_rows-1){
				sys = 0+STEP*overlap_factor;
				for (int r = ys+STEP*overlap_factor; r <= ye-STEP*overlap_factor; r++, sys++){
					sxs = 0;
					for (int c = xs; c <= xe-STEP*overlap_factor; c++, sxs++){
						index = c+r*image_cols;
						//subindex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
						subindex = sys*sub_rows*num_sub+sxs*num_sub+count;
						if (subindex >= sub_cols*sub_rows*num_sub)
							{
								error = subindex;	
								return error;
							}
						image[index] = sub_images[subindex]; 
					}
				}
			}
			//bottom left corner
			else if(xs == 0 && ye > image_rows-1){
				sys = 0+STEP*overlap_factor;
				for (int r = ys+STEP*overlap_factor; r <= ye-STEP*overlap_factor; r++, sys++){
					sxs = 0;
					for (int c = xs; c <= xe-STEP*overlap_factor; c++, sxs++){
						if(r > image_rows-1){
							//do nothing
						}
						else{
							index = c+r*image_cols;
							//subindex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
							subindex = sys*sub_rows*num_sub+sxs*num_sub+count;
							if (subindex >= sub_cols*sub_rows*num_sub)
							{
								error = subindex;	
								return error;
							}
							image[index] = sub_images[subindex]; 
						}
					}
				}
			}
			
			//exceed pixel count on the bottom and right
			else if(xe > image_cols-1 && ye > image_rows-1){
				sys = 0+STEP*overlap_factor;
				for (int r = ys+STEP*overlap_factor; r <= ye-STEP*overlap_factor; r++, sys++){
					sxs = 0+STEP*overlap_factor;
					for (int c = xs+STEP*overlap_factor; c <= xe-STEP*overlap_factor; c++, sxs++){
						if(c > image_cols-1 && r > image_rows-1){
							//do nothing
						}
						else if (c > image_cols-1){
							//do nothing
						}
						else if(r > image_rows-1){
							//do nothing
						}
						else {
							index = c+r*image_cols;
							//subindex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
							subindex = sys*sub_rows*num_sub+sxs*num_sub+count;
							if (subindex >= sub_cols*sub_rows*num_sub)
							{
								error = subindex;	
								return error;
							}
							image[index] = sub_images[subindex]; 
						}
					}
				}		
			}
			
			//exceed pixel count on the right edge
			else if(xe > image_cols-1){
				sys = 0+STEP*overlap_factor;
				for (int r = ys+STEP*overlap_factor; r <= ye-STEP*overlap_factor; r++, sys++){
					sxs = 0+STEP*overlap_factor;
					for (int c = xs+STEP*overlap_factor; c <= xe-STEP*overlap_factor; c++, sxs++){
						if(c > image_cols-1){
							//do nothing
						}
						else{
							
							index = c+r*image_cols;
							//subindex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
							subindex = sys*sub_rows*num_sub+sxs*num_sub+count;
							if (subindex >= sub_cols*sub_rows*num_sub)
							{
								error = subindex;	
								return error;
							}
							image[index] = sub_images[subindex]; 
						}
					}
				}
			}
			//exceed pixel count on the bottom edge
			else if(ye > image_cols-1){
				sys = 0+STEP*overlap_factor;
				for (int r = ys+STEP*overlap_factor; r <= ye-STEP*overlap_factor; r++, sys++){
					sxs = 0+STEP*overlap_factor;
					for (int c = xs+STEP*overlap_factor; c <= xe-STEP*overlap_factor; c++, sxs++){
						if(r > image_rows-1){
							//do nothing
						}
						else{
							index = c+r*image_cols;
							//subindex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
							subindex = sys*sub_rows*num_sub+sxs*num_sub+count;
							if (subindex >= sub_cols*sub_rows*num_sub)
							{
								error = subindex;	
								return error;
							}
							image[index] = sub_images[subindex]; 
						}
					}
				}
			}	
			// all is good
			else {
				sys = 0+STEP*overlap_factor;
				for (int r = ys+STEP*overlap_factor; r <= ye-STEP*overlap_factor; r++, sys++){
					sxs = 0+STEP*overlap_factor;
					for (int c = xs+STEP*overlap_factor; c <= xe-STEP*overlap_factor; c++, sxs++){
						index = c+r*image_cols;
						if (index >= image_cols*image_rows)
						{
							// do nothing
						}
						else{
							//subindex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
							subindex = sys*sub_rows*num_sub+sxs*num_sub+count;
							if (subindex >= sub_cols*sub_rows*num_sub)
							{
								error = subindex;	
								return error;
							}
							image[index] = sub_images[subindex]; 
						}
					}
				}

			}
			count = count + 1;
			stepgeneral(&xs,&xe,sub_cols,overlap_factor);
		}
		stepgeneral(&ys,&ye,sub_rows,overlap_factor);
	}
	error = 0;	
	return error;
}
/* The stitching algroithim */
int stitchsubimages_double(double* image, int image_rows, int image_cols, double* sub_images, int sub_rows, int sub_cols, int overlap_factor, int num_sub){
		int xs = 0; int xe = sub_cols -1;
	int ys = 0; int ye = sub_rows -1;
	
	int sxs = 0; 
	int sys = 0; 

	int index = 0;
	int count = 0;
	int subindex = 0;
	int error=-99;

	for (; ys < image_rows-1;){
		xs = 0;
		xe = sub_cols -1;
		for (; xs < image_cols-1;){
			if(count >= num_sub){
				error = 2;
				return error;
			}
			else if(xs >= image_cols-1 || ys >= image_rows-1){
				error = 1;	
				return error;
			
			}
			
			//top left corner 
			else if (xs == 0 && ys == 0){
				sys = 0;
				for (int r = ys; r <= ye-STEP*overlap_factor; r++, sys++){
					sxs = 0;
					for (int c = xs; c <= xe-STEP*overlap_factor; c++, sxs++){
						index = c+r*image_cols;
						//subindex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
						subindex = sys*sub_rows*num_sub+sxs*num_sub+count;
						if (subindex >= sub_cols*sub_rows*num_sub)
						{
							error = subindex;	
							return error;
						}
						image[index] = sub_images[subindex]; 
					}
				}	
			}
			
			//top edge
			else if (ys == 0 && xe <= image_cols-1){
				sys = 0;
				for (int r = ys; r <= ye-STEP*overlap_factor; r++, sys++){
					sxs = 0+STEP*overlap_factor;
					for (int c = xs+STEP*overlap_factor; c <= xe-STEP*overlap_factor; c++, sxs++){
						index = c+r*image_cols;
						//subindex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
						subindex = sys*sub_rows*num_sub+sxs*num_sub+count;
						if (subindex >= sub_cols*sub_rows*num_sub)
						{
							error = subindex;	
							return error;
						}
						image[index] = sub_images[subindex]; 
					}
				}	
			}
			
			//top right corner
			else if(ys == 0 && xe > image_cols-1){
				sys = 0;
				for (int r = ys; r <= ye-STEP*overlap_factor; r++, sys++){
					sxs = 0+STEP*overlap_factor;
					for (int c = xs+STEP*overlap_factor; c <= xe-STEP*overlap_factor; c++, sxs++){
						if(c > image_cols-1){
							//do nothing
						}
						else{
							index = c+r*image_cols;
							//subindex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
							subindex = sys*sub_rows*num_sub+sxs*num_sub+count;
							if (subindex >= sub_cols*sub_rows*num_sub)
							{
								error = subindex;	
								return error;
							}
							image[index] = sub_images[subindex]; 
						}
					}
				}
			}
			//left edge
			else if(xs == 0 && ye <= image_rows-1){
				sys = 0+STEP*overlap_factor;
				for (int r = ys+STEP*overlap_factor; r <= ye-STEP*overlap_factor; r++, sys++){
					sxs = 0;
					for (int c = xs; c <= xe-STEP*overlap_factor; c++, sxs++){
						index = c+r*image_cols;
						//subindex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
						subindex = sys*sub_rows*num_sub+sxs*num_sub+count;
						if (subindex >= sub_cols*sub_rows*num_sub)
							{
								error = subindex;	
								return error;
							}
						image[index] = sub_images[subindex]; 
					}
				}
			}
			//bottom left corner
			else if(xs == 0 && ye > image_rows-1){
				sys = 0+STEP*overlap_factor;
				for (int r = ys+STEP*overlap_factor; r <= ye-STEP*overlap_factor; r++, sys++){
					sxs = 0;
					for (int c = xs; c <= xe-STEP*overlap_factor; c++, sxs++){
						if(r > image_rows-1){
							//do nothing
						}
						else{
							index = c+r*image_cols;
							//subindex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
							subindex = sys*sub_rows*num_sub+sxs*num_sub+count;
							if (subindex >= sub_cols*sub_rows*num_sub)
							{
								error = subindex;	
								return error;
							}
							image[index] = sub_images[subindex]; 
						}
					}
				}
			}
			
			//exceed pixel count on the bottom and right
			else if(xe > image_cols-1 && ye > image_rows-1){
				sys = 0+STEP*overlap_factor;
				for (int r = ys+STEP*overlap_factor; r <= ye-STEP*overlap_factor; r++, sys++){
					sxs = 0+STEP*overlap_factor;
					for (int c = xs+STEP*overlap_factor; c <= xe-STEP*overlap_factor; c++, sxs++){
						if(c > image_cols-1 && r > image_rows-1){
							//do nothing
						}
						else if (c > image_cols-1){
							//do nothing
						}
						else if(r > image_rows-1){
							//do nothing
						}
						else {
							index = c+r*image_cols;
							//subindex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
							subindex = sys*sub_rows*num_sub+sxs*num_sub+count;
							if (subindex >= sub_cols*sub_rows*num_sub)
							{
								error = subindex;	
								return error;
							}
							image[index] = sub_images[subindex]; 
						}
					}
				}		
			}
			
			//exceed pixel count on the right edge
			else if(xe > image_cols-1){
				sys = 0+STEP*overlap_factor;
				for (int r = ys+STEP*overlap_factor; r <= ye-STEP*overlap_factor; r++, sys++){
					sxs = 0+STEP*overlap_factor;
					for (int c = xs+STEP*overlap_factor; c <= xe-STEP*overlap_factor; c++, sxs++){
						if(c > image_cols-1){
							//do nothing
						}
						else{
							
							index = c+r*image_cols;
							//subindex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
							subindex = sys*sub_rows*num_sub+sxs*num_sub+count;
							if (subindex >= sub_cols*sub_rows*num_sub)
							{
								error = subindex;	
								return error;
							}
							image[index] = sub_images[subindex]; 
						}
					}
				}
			}
			//exceed pixel count on the bottom edge
			else if(ye > image_cols-1){
				sys = 0+STEP*overlap_factor;
				for (int r = ys+STEP*overlap_factor; r <= ye-STEP*overlap_factor; r++, sys++){
					sxs = 0+STEP*overlap_factor;
					for (int c = xs+STEP*overlap_factor; c <= xe-STEP*overlap_factor; c++, sxs++){
						if(r > image_rows-1){
							//do nothing
						}
						else{
							index = c+r*image_cols;
							//subindex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
							subindex = sys*sub_rows*num_sub+sxs*num_sub+count;
							if (subindex >= sub_cols*sub_rows*num_sub)
							{
								error = subindex;	
								return error;
							}
							image[index] = sub_images[subindex]; 
						}
					}
				}
			}	
			// all is good
			else {
				sys = 0+STEP*overlap_factor;
				for (int r = ys+STEP*overlap_factor; r <= ye-STEP*overlap_factor; r++, sys++){
					sxs = 0+STEP*overlap_factor;
					for (int c = xs+STEP*overlap_factor; c <= xe-STEP*overlap_factor; c++, sxs++){
						index = c+r*image_cols;
						if (index >= image_cols*image_rows)
						{
							// do nothing
						}
						else{
							//subindex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
							subindex = sys*sub_rows*num_sub+sxs*num_sub+count;
							if (subindex >= sub_cols*sub_rows*num_sub)
							{
								error = subindex;	
								return error;
							}
							image[index] = sub_images[subindex]; 
						}
					}
				}

			}
			count = count + 1;
			stepgeneral(&xs,&xe,sub_cols,overlap_factor);
		}
		stepgeneral(&ys,&ye,sub_rows,overlap_factor);
	}
	error = 0;	
	return error;
}

/* Return the image case. */
int casenumber(int image_rows, int image_cols,int sub_rows, int sub_cols, int num_sub, int count, int xs, int xe, int ys, int ye)
{
	if(count >= num_sub) { return 1; }// Case 1, more subimages available than allocated
	else if(xs >= image_cols-1 || ys >= image_rows-1) { return 2; } // Case 2, starting pixel exceeds image dimensions
	else if(xe > image_cols-1 && ye > image_rows-1) { return 3; } // Case 3, exceed pixel count on the bottom and right
	else if(xe > image_cols-1) { return 4; } // exceed pixel count on the right edge
	else if(ye > image_cols-1) { return 5; } // exceed pixel count on the bottom edge
	else { return 0; } // all is good
}
/* Generate sub images from full size image. Assume image is 2-dimensional.
status: 0 = succes, 1 = success, but more subimages available than allocated, 2 = starting pixel exceeds image dimensions,
-1 overlap is too large, -2 subimage is too large */
int formSubImage_float(float* image, int image_rows, int image_cols, float* sub_images, int sub_rows, int sub_cols, int overlap_factor, int num_sub)
{	
	int xs = 0; // starting column image pixel 
	int xe = sub_cols -1; // ending column pixel
	int ys = 0; // starting row image pixel 
	int ye = sub_rows -1; // ending row pixel
	int sxs = 0; // starting column subimage pixel
	int sys = 0; // starting row subimage pixel
	int index = 0; // index into the image
	int count = 0; // number of subimages created
	int subindex = 0; // index into the subimage
	int casenum= 0; // place holder for the image case

	for (; ys < image_rows-1;){ // iterate over the rows last in order to keep C convention
		xs = 0; // each time we start a new row, start on the first column
		xe = sub_cols -1; // move along the columns until you reach the end of the sub image boundary
		for (; xs < image_cols-1;){ // iterate over columns first, then rows in order to keep C convention
			//casenum = casenumber(image_rows,image_cols,sub_rows,sub_cols,num_sub,count,xs,xe,ys,ye);
			switch(casenum) 
			{
				case 1: // Case 1, more subimages available than allocated
					return 1;
				case 2: // Case 2, starting pixel exceeds image dimensions
					return 2;
				case 3: // Case 3, exceed pixel count on the bottom and right
				
					sys = 0; // always start in top-left corner of subimage
					for (int r = ys; r <= ye; r++, sys++){ // iterate over rows last
						sxs = 0; // start on far left
						for (int c = xs; c <= xe; c++, sxs++){ // iterate over columns first
							if(c > image_cols-1 && r > image_rows-1){ // exceed the image in both columns and rows
								//subindex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
								subindex = sys*sub_rows*num_sub+sxs*num_sub+count;
								if (subindex >= sub_cols*sub_rows*num_sub) { return subindex; }  // segmentation fault
								sub_images[subindex] = 0.0; // outside of image bounds so just fill with zeros
							}
							else if (c > image_cols-1){ // exceed the image in columns
								//subindex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
								subindex = sys*sub_rows*num_sub+sxs*num_sub+count;
								if (subindex >= sub_cols*sub_rows*num_sub) { return subindex; }  // segmentation fault
								sub_images[subindex] = 0.0; // outside of image bounds so just fill with zeros
							}
							else if(r > image_rows-1){ // exceed the image in rows
								//subindex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
								subindex = sys*sub_rows*num_sub+sxs*num_sub+count;
								if (subindex >= sub_cols*sub_rows*num_sub) { return subindex; }  // segmentation fault
								sub_images[subindex] = 0.0; // outside of image bounds so just fill with zeros
							}
							else { // within bounds of image
								index = c+r*image_cols;
								//subindex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
								subindex = sys*sub_rows*num_sub+sxs*num_sub+count;
								if (subindex >= sub_cols*sub_rows*num_sub) { return subindex; } // segmentation fault
								sub_images[subindex] = image[index]; // good value
							}
						}
					}
					
					break;
				case 4: // exceed pixel count on the right edge
				
					sys = 0; // always start in top-left corner of subimage
					for (int r = ys; r <= ye; r++, sys++){ // iterate over rows last
						sxs = 0; 
						for (int c = xs; c <= xe; c++, sxs++){ // iterate over columns first
							if(c > image_cols-1){ // exceed the image in columns
								//subindex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
								subindex = sys*sub_rows*num_sub+sxs*num_sub+count;
								if (subindex >= sub_cols*sub_rows*num_sub) { return subindex; }  // segmentation fault
								sub_images[subindex] = 0.0; // outside of image bounds so just fill with zeros
							}
							else{ // within bounds of image
								index = c+r*image_cols;
								//subindex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
								subindex = sys*sub_rows*num_sub+sxs*num_sub+count;
								if (subindex >= sub_cols*sub_rows*num_sub) { return subindex; }  // segmentation fault
								sub_images[subindex] = image[index]; // good value
							}
						}
					}
					
					break;
				case 5: // exceed pixel count on the bottom edge
				
					sys = 0; // always start in top-left corner of subimage
					for (int r = ys; r <= ye; r++, sys++){ // iterate over rows last
						sxs = 0;
						for (int c = xs; c <= xe; c++, sxs++){ // iterate over columns first
							if(r > image_rows-1){ //exceed the image in rows
								//subindex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
								subindex = sys*sub_rows*num_sub+sxs*num_sub+count;	
								if (subindex >= sub_cols*sub_rows*num_sub) { return subindex; }  // segmentation fault
								sub_images[subindex] = 0.0; // outside of image bounds so just fill with zeros
							}
							else{ // within bounds of image
								index = c+r*image_cols;
								//subindex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
								subindex = sys*sub_rows*num_sub+sxs*num_sub+count;
								if (subindex >= sub_cols*sub_rows*num_sub) { return subindex; }  // segmentation fault
								sub_images[subindex] = image[index]; // good value
							}
						}
					}
					
					break;
				default: // all is good
					sys = 0; // always start in top-left corner of subimage
					
					for (int r = ys; r <= ye; r++, sys++){ // iterate over rows last
						sxs = 0;
						for (int c = xs; c <= xe; c++, sxs++){ // iterate over columns first
							index = c+r*image_cols;
							if (index >= image_cols*image_rows) {  } // segmentation fault (NEED TO FIX)
							else{
								//subindex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
								subindex = sys*sub_rows*num_sub+sxs*num_sub+count;
								if (subindex >= sub_cols*sub_rows*num_sub) { return subindex; }  // segmentation fault
								sub_images[subindex] = image[index]; // good value
							}
						}
					}
			}
			count = count + 1; // increment subimage count
			stepgeneral(&xs,&xe,sub_cols,overlap_factor);
		}
		stepgeneral(&ys,&ye,sub_rows,overlap_factor);
	}
	
	return 0;	
}
/* Generate sub images from full size image. Assume image is 2-dimensional.
status: 0 = succes, 1 = success, but number of subimages generated < num_sub, 2 = success, but number of subimages generated > num_sub,
-1 overlap is too large, -2 subimage is too large */
int formSubImage_double(double* image, int image_rows, int image_cols, double* sub_images, int sub_rows, int sub_cols, int overlap_factor, int num_sub)
{
	int xs = 0; // starting column image pixel 
	int xe = sub_cols -1; // ending column pixel
	int ys = 0; // starting row image pixel 
	int ye = sub_rows -1; // ending row pixel
	int sxs = 0; // starting column subimage pixel
	int sys = 0; // starting row subimage pixel
	int index = 0; // index into the image
	int count = 0; // number of subimages created
	int subindex = 0; // index into the subimage
	int casenum= 0; // place holder for the image case

	for (; ys < image_rows-1;){ // iterate over the rows last in order to keep C convention
		xs = 0; // each time we start a new row, start on the first column
		xe = sub_cols -1; // move along the columns until you reach the end of the sub image boundary
		for (; xs < image_cols-1;){ // iterate over columns first, then rows in order to keep C convention
			casenum = casenumber(image_rows,image_cols,sub_rows,sub_cols,num_sub,count,xs,xe,ys,ye);
			switch(casenum) 
			{
				case 1: // Case 1, more subimages available than allocated
					return 1;
				case 2: // Case 2, starting pixel exceeds image dimensions
					return 2;
				case 3: // Case 3, exceed pixel count on the bottom and right
					sys = 0; // always start in top-left corner of subimage
					for (int r = ys; r <= ye; r++, sys++){ // iterate over rows last
						sxs = 0; // start on far left
						for (int c = xs; c <= xe; c++, sxs++){ // iterate over columns first
							if(c > image_cols-1 && r > image_rows-1){ // exceed the image in both columns and rows
								//subindex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
								subindex = sys*sub_rows*num_sub+sxs*num_sub+count;
								if (subindex >= sub_cols*sub_rows*num_sub) { return subindex; }  // segmentation fault
								sub_images[subindex] = 0.0; // outside of image bounds so just fill with zeros
							}
							else if (c > image_cols-1){ // exceed the image in columns
								//subindex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
								subindex = sys*sub_rows*num_sub+sxs*num_sub+count;
								if (subindex >= sub_cols*sub_rows*num_sub) { return subindex; }  // segmentation fault
								sub_images[subindex] = 0.0; // outside of image bounds so just fill with zeros
							}
							else if(r > image_rows-1){ // exceed the image in rows
								//subindex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
								subindex = sys*sub_rows*num_sub+sxs*num_sub+count;
								if (subindex >= sub_cols*sub_rows*num_sub) { return subindex; }  // segmentation fault
								sub_images[subindex] = 0.0; // outside of image bounds so just fill with zeros
							}
							else { // within bounds of image
								index = c+r*image_cols;
								//subindex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
								subindex = sys*sub_rows*num_sub+sxs*num_sub+count;
								if (subindex >= sub_cols*sub_rows*num_sub) { return subindex; } // segmentation fault
								sub_images[subindex] = image[index]; // good value
							}
						}
					}
					break;
				case 4: // exceed pixel count on the right edge
					sys = 0; // always start in top-left corner of subimage
					for (int r = ys; r <= ye; r++, sys++){ // iterate over rows last
						sxs = 0; 
						for (int c = xs; c <= xe; c++, sxs++){ // iterate over columns first
							if(c > image_cols-1){ // exceed the image in columns
								//subindex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
								subindex = sys*sub_rows*num_sub+sxs*num_sub+count;
								if (subindex >= sub_cols*sub_rows*num_sub) { return subindex; }  // segmentation fault
								sub_images[subindex] = 0.0; // outside of image bounds so just fill with zeros
							}
							else{ // within bounds of image
								index = c+r*image_cols;
								//subindex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
								subindex = sys*sub_rows*num_sub+sxs*num_sub+count;
								if (subindex >= sub_cols*sub_rows*num_sub) { return subindex; }  // segmentation fault
								sub_images[subindex] = image[index]; // good value
							}
						}
					}
					break;
				case 5: // exceed pixel count on the bottom edge
					sys = 0; // always start in top-left corner of subimage
					for (int r = ys; r <= ye; r++, sys++){ // iterate over rows last
						sxs = 0;
						for (int c = xs; c <= xe; c++, sxs++){ // iterate over columns first
							if(r > image_rows-1){ //exceed the image in rows
								//subindex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
								subindex = sys*sub_rows*num_sub+sxs*num_sub+count;	
								if (subindex >= sub_cols*sub_rows*num_sub) { return subindex; }  // segmentation fault
								sub_images[subindex] = 0.0; // outside of image bounds so just fill with zeros
							}
							else{ // within bounds of image
								index = c+r*image_cols;
								//subindex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
								subindex = sys*sub_rows*num_sub+sxs*num_sub+count;
								if (subindex >= sub_cols*sub_rows*num_sub) { return subindex; }  // segmentation fault
								sub_images[subindex] = image[index]; // good value
							}
						}
					}
					break;
				default: // all is good
					sys = 0; // always start in top-left corner of subimage
					for (int r = ys; r <= ye; r++, sys++){ // iterate over rows last
						sxs = 0;
						for (int c = xs; c <= xe; c++, sxs++){ // iterate over columns first
							index = c+r*image_cols;
							if (index >= image_cols*image_rows) { /* do nothing */ } // segmentation fault
							else{
								//subindex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
								subindex = sys*sub_rows*num_sub+sxs*num_sub+count;
								if (subindex >= sub_cols*sub_rows*num_sub) { return subindex; }  // segmentation fault
								sub_images[subindex] = image[index]; // good value
							}
						}
					}
			}
			count = count + 1; // increment subimage count
			stepgeneral(&xs,&xe,sub_cols,overlap_factor);
		}
		stepgeneral(&ys,&ye,sub_rows,overlap_factor);
	}
	return 0;
}
