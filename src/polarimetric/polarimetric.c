#include "polarimetric.h"
#include<math.h>
#include<stdlib.h>

/*** Internal Functions ***/

/* Increment the step count for sub image generation and stitching. */
void step22(int *sp, int *ep, int width, int overlap)
{
	/* sp = starting pixel, ep = ending pixel, width = width of image, overlap = desired overlap */
	*sp = *ep-(2*overlap)+1;
	if(*sp % 2 != 0){ // need to always start on an even pixel
		*sp = *sp - 1;
	}	
	*ep = *sp + width - 1;
}

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
int getNumberSubImages22(int image_y, int image_x, int sub_y, int sub_x, int overlap)
{
	int x = (int)ceil(((double)image_x/(double)(sub_x-2*overlap)));
	int y = (int)ceil(((double)image_y/(double)(sub_y-2*overlap)));
	return x*y;
}

/* The stitching algroithim */
void Stitching(){
	/*
	for (; ys < image_rows-1;){
		xs = 0;
		for (; xs < image_cols-1;){
			//index = xs+ys*image_cols;
			//cout << ys+xs*image_cols << ", "; 

			//the first pixel
			if (xs == 0 && ys == 0){
				for (int r = ys; r <= ye; r++){
					for (int c = xs; c <= xs; c++){
						index = c+r*image_cols;
						cout << Test_Image[index] << ", ";
						//cout << c+r*image_cols << ", ";
						//Sub_Images[] = Test_Image[index]; 
					}
				}
			} 

			//left edge
			else if(xs == 0){

			}

			//top edge
			else if(ys == 0){

			//exceed pixel count on the bottom and right
			}else if(xe > image_cols-1 && ye > image_rows-1){

			//exceed pixel count on the right edge
			}else if(xe > image_cols-1){

			//exceed pixel count on the bottom edge
			}else if(ye > image_cols-1){
			
			}else if(xs >= image_cols-1 && ys >= image_rows-1){
				*error = 1;	
			
			//middle of the image
			}else {
				//for (int r = ys; r <= ye; r++){
					//for (int c = xs; c <= xs; c++){
					//	index = c+r*image_cols;
						//cout << Test_Image[index] << ", ";
						//Sub_Images[] = Test_Image[index]; 
					//}
				//}
			}
		
	 		xs = xe-(2*overlap);
			if(xs % 2 != 0){
				xs = xs - 1;

			}
			
			xe = xs + sub_cols - 1;
			count = count+1;
		
		}
		ys = ye-(2*overlap);	
		
		if(ys % 2 != 0){
				ys = ys - 1;

			}
			ye = ys + sub_rows - 1;
	}
	*/
}

/* Generate sub images from full size image. Assume image is 2-dimensional.
status: 0 = succes, 1 = success, but number of subimages generated < num_sub, 2 = success, but number of subimages generated > num_sub,
-1 overlap is too large, -2 subimage is too large */
void formSubImage22(double* image, int image_rows, int image_cols, double* sub_images, int sub_rows, int sub_cols, int overlap, int num_sub, int* error)
{
	int xs = 0; int xe = sub_cols -1;
	int ys = 0; int ye = sub_rows -1;
	
	int sxs = 0; 
	int sys = 0; 

	int index = 0;
	int count = 0;
	int subIndex = 0;

	for (; ys < image_rows-1;){
		xs = 0;
		xe = sub_cols -1;
		for (; xs < image_cols-1;){
			if(count >= num_sub){
				*error = 2;
				return;
			}
			else if(xs >= image_cols-1 || ys >= image_rows-1){
				*error = 1;	
				return;
			
			}
			//exceed pixel count on the bottom and right
			else if(xe > image_cols-1 && ye > image_rows-1){
				sxs = 0;
				sys = 0;
				for (int r = ys; r <= ye; r++, sys++){
					sxs = 0;
					for (int c = xs; c <= xe; c++, sxs++){
						if(c > image_cols-1 && r > image_rows-1){
							subIndex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
							sub_images[subIndex] = 0.0;
						}
						else if (c > image_cols-1){
							subIndex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
							sub_images[subIndex] = 0.0;
						}
						else if(r > image_rows-1){
							subIndex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
							sub_images[subIndex] = 0.0;
						}
						else {
							index = c+r*image_cols;
							subIndex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
							sub_images[subIndex] = image[index]; 
						}
					}
				}		
			}
			//exceed pixel count on the right edge
			else if(xe > image_cols-1){
				sxs = 0;
				sys = 0;
				for (int r = ys; r <= ye; r++, sys++){
					sxs = 0;
					for (int c = xs; c <= xe; c++, sxs++){
						if(c > image_cols-1){
							subIndex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
							sub_images[subIndex] = 0.0;
						}
						else{
							
							index = c+r*image_cols;
							subIndex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
							sub_images[subIndex] = image[index]; 
						}
					}
				}
			}
			//exceed pixel count on the bottom edge
			else if(ye > image_cols-1){
				sxs = 0;
				sys = 0;
				for (int r = ys; r <= ye; r++, sys++){
					sxs = 0;
					for (int c = xs; c <= xe; c++, sxs++){
						if(r > image_rows-1){
							subIndex = sxs+sys*sub_cols+count*sub_cols*sub_rows;	
							sub_images[subIndex] = 0.0;
						}
						else{
							index = c+r*image_cols;
							subIndex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
							sub_images[subIndex] = image[index]; 
						}
					}
				}
			}
			// all is good
			else {
				sxs = 0;
				sys = 0;
				for (int r = ys; r <= ye; r++, sys++){
					sxs = 0;
					for (int c = xs; c <= xe; c++, sxs++){
						index = c+r*image_cols;
						subIndex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
						sub_images[subIndex] = image[index]; 
					}
				}

			}
			count = count + 1;
			step22(&xs,&xe,sub_cols,overlap);
		}
		step22(&ys,&ye,sub_rows,overlap);
	}
	*error = 0;	
}
