extern "C"
{
#include "polarimetric.h"
}
#include <iostream>
#include <string>
#include <cmath>


using namespace std;

void formugrid22(float* image, int image_rows, int image_cols);
void formugrid32(float* image, int image_rows, int image_cols);
void formugrid42(float* image, int image_rows, int image_cols);

int main()
{
	// Configuration: select image size, subimage size, and overlap factor
	int image_y = 2048; int image_x = 2448; int sub_x = 100; int sub_y = 100; int overlap_factor = 1; // int image_y = 2048; int image_x = 2448;

	// Initialize variables needed throughout testing
	int error = -99; // -99 should never be returned by a function
	float* test_image22; float* test_image32; float* test_image42;float* sub_images; float* stitched_image;

	// Get the number of subimages for the current configuration and write results
	int sub_depth = getNumberSubImages(image_y,image_x,sub_y,sub_x,overlap_factor);
	cout << "Number of subimages: " << sub_depth << endl;

	// Allocate memory to variables
	test_image22 = new float[image_x*image_y];
	test_image32 = new float[image_x*image_y];
	test_image42 = new float[image_x*image_y];
	for (int i = 0; i < image_y*image_x; i++){
		test_image22[i] = 0.0;
		test_image32[i] = 0.0;
		test_image42[i] = 0.0;
	}

	sub_images = new float[sub_x*sub_y*sub_depth];
	for (int i = 0; i < sub_x*sub_y*sub_depth; i++){
			sub_images[i] = 0.0;
	}

	stitched_image = new float[image_x*image_y];
	for (int i = 0; i < image_y*image_x; i++){
			stitched_image[i] = 0.0;
	}

	// Form ugrid patterns
	formugrid22(test_image22, image_y, image_x);
	formugrid32(test_image32, image_y, image_x);
	formugrid42(test_image42, image_y, image_x);
	
	cout << "FORMING SUBIMAGE: " << endl;
	error = formSubImage_float(test_image22, image_y, image_x, sub_images, sub_y, sub_x, overlap_factor, sub_depth);
	cout << "Form subimage error: " << error << endl << endl;
	error = -99;
	
	cout << "STITCHING SUBIMAGES: " << endl;
	error = stitchsubimages_float(stitched_image, image_y, image_x, sub_images, sub_y, sub_x, overlap_factor, sub_depth);
	cout << "Stitching error: " << error << endl << endl;
	error = -99;

	float stitched_check = 0.0;
	for (int i = 0; i < image_x*image_y; i++){
		stitched_check = stitched_check + abs((int)test_image22[i] - (int)stitched_image[i]);
		/*if(stitched_image[i] != test_image22[i]){
			cout << " Stitch index " << StitchImage[i] << " TestImage " << TestImage[i] << endl << " index is " << i;
		}*/
	} 
	cout << "Image difference check: " << stitched_check << endl;
	
	delete[] test_image22;
	test_image22=NULL;
	delete[] test_image32;
	test_image32=NULL;
	delete[] test_image42;
	test_image42=NULL;
	delete[] sub_images;
	sub_images=NULL;
	delete[] stitched_image;
	stitched_image=NULL;

	return 0;
	
}

void formugrid22(float* image, int image_rows, int image_cols){
	int indx=0;
	for (int r = 0; r < image_rows; r+=2){
		for (int c = 0; c < image_cols; c++){
			indx = c + r*image_rows;
			if (indx == 0){
				image[indx] = 90.0;
			}
			else if (indx == 1){
				image[indx] = 135.0;
			}
			else {
				image[indx] = indx % 2 == 0 ? 90.0 : 135.0; // condition ? value_if_true : value_if_false
			}
		}
	}
	for (int r = 1; r < image_rows; r+=2){
		for (int c = 0; c < image_cols; c++){
			indx = c + r*image_rows;
			image[indx] = indx % 2 == 0 ? 45.0 : 0.0; // condition ? value_if_true : value_if_false
		}
	}
}

void formugrid32(float* image, int image_rows, int image_cols){ 
	int indx=0;
	for (int r = 0; r < image_rows; r+=2){
		for (int c = 0; c < image_cols; c++){
			indx = c + r*image_rows;
			if (indx == 0){
				image[indx] = 90.0;
			}
			else if (indx == 1){
				image[indx] = 135.0;
			}
			else {
				image[indx] = indx % 2 == 0 ? 90.0 : 135.0; // condition ? value_if_true : value_if_false
			}
		}
	}
	for (int r = 1; r < image_rows; r+=2){
		for (int c = 0; c < image_cols; c++){
			indx = c + r*image_rows;
			image[indx] = indx % 2 == 0 ? 45.0 : 0.0; // condition ? value_if_true : value_if_false
		}
	}
}

void formugrid42(float* image, int image_rows, int image_cols){ 
	int indx=0;
	for (int r = 0; r < image_rows; r+=2){
		for (int c = 0; c < image_cols; c++){
			indx = c + r*image_rows;
			if (indx == 0){
				image[indx] = 90.0;
			}
			else if (indx == 1){
				image[indx] = 135.0;
			}
			else {
				image[indx] = indx % 2 == 0 ? 90.0 : 135.0; // condition ? value_if_true : value_if_false
			}
		}
	}
	for (int r = 1; r < image_rows; r+=2){
		for (int c = 0; c < image_cols; c++){
			indx = c + r*image_rows;
			image[indx] = indx % 2 == 0 ? 45.0 : 0.0; // condition ? value_if_true : value_if_false
		}
	}
}
	
	
	
	


	

