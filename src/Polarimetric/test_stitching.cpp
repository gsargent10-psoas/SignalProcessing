extern "C"
{
#include "polarimetric.h"
}
#include <iostream>
#include <string>
#include <cmath>


using namespace std;

void formImage(float* TestImage, int image_rows, int image_cols);

int main()
{
	int image_y = 2048; int image_x = 2448; int sub_x = 192; int sub_y = 192; int overlap_factor = 2;
	int error = -99;
	float* TestImage; float* SubImages; float* StitchImage;

	int sub_depth = getNumberSubImages(image_y,image_x,sub_y,sub_x,overlap_factor);
	cout << "Number of subimages: " << sub_depth << endl;

	TestImage = new float[image_x*image_y];
	for (int i = 0; i < image_y*image_x; i++){
		TestImage[i] = 0.0;
	}

	SubImages = new float[sub_x*sub_y*sub_depth];
	for (int i = 0; i < sub_x*sub_y*sub_depth; i++){
			SubImages[i] = 0.0;
	}

	StitchImage = new float[image_x*image_y];
	for (int i = 0; i < image_y*image_x; i++){
			StitchImage[i] = 0.0;
	}

	
	formImage(TestImage, image_y, image_x);
	
	
	error = formSubImage_float(TestImage, image_y, image_x, SubImages, sub_y, sub_x, overlap_factor, sub_depth);
	cout << "Form subimage error: " << error <<endl;
	error = -99;
	

	error = stitchsubimages_float(StitchImage, image_y, image_x, SubImages, sub_y, sub_x, overlap_factor, sub_depth); //Passing in the empty Stitch image
	cout << "Stitching error: " << error <<endl;
	error = -99;
	
	
	/*
	for (int s = 0; s < sub_depth; s++){
		for (int r = 0; r < sub_y; r++){
			for (int c = 0; c < sub_x; c++){
				cout << SubImages[c+r*sub_x+s*sub_y*sub_x] << ", ";
				}
			cout <<endl;
		}
		cout <<endl;
		cout <<endl;
	}

	*/

	float check = 0.0;
	for (int i = 0; i < image_x*image_y; i++){
		//cout << StitchImage[i] << ", ";
		check = check + abs((int)TestImage[i] - (int)StitchImage[i]);
		if(isnan(check)){
			//cout << StitchImage[i] << ", " << TestImage[i] <<endl;
			//cout << "i = " << i << endl;
			break;
		}
		if(StitchImage[i] != TestImage[i]){
			//cout << " Stitch index " << StitchImage[i] << " TestImage " << TestImage[i] << endl << " index is " << i;

		}
	} 
	cout << "Image difference check: " << check << endl;
	

	
/*
	string filename = "./test.pi";
	float data[4] = {1.0, 2.0, 3.0, 4.0};
	bool ret = openpiwriter((char*) filename.c_str(),2,2,4,true);
	cout << "openpiwrite() return: " << ret << endl;
	int ret2 = writepi(&data[0],1);
	cout << "writepi() return: " << ret2 << endl;
	closewriter();
	ret = openpiread((char*) filename.c_str());
	cout << "openpiread() return: " << ret << endl;
	cout << "Width: " << getwidth() << ", Height: " << getheight() << endl;
	closereader();
	
*/


	delete[] TestImage;
	TestImage=NULL;
	delete[] SubImages;
	SubImages=NULL;
	delete[] StitchImage;
	StitchImage=NULL;
	
	

	
	

	return 0;
	
}
	void formImage(float* TestImage, int image_rows, int image_cols){ 

	//generating a test Image 
	for (int i = 0; i < image_rows*image_cols; i++){
		TestImage[i] = rand() % 255 + 1;
	}

	int pattern = 0;
	int count = 0;
	bool change = true;
/*
	for (int i = 0; i < image_rows*image_cols; i+=2){ //rows x columns
	
		if (count >= image_cols){	

			while (change == true ){
	
				if (pattern == 0){
				pattern = 1;
				break;
				}
		
				if (pattern == 1){
				pattern = 0;
				break;
				}
			}		
		count = 0;
		change = true;
		
	}
		if (pattern == 0){
			TestImage[i] = rand() % 255 + 1;
			TestImage[i+1] = rand() % 255 + 1;
		}
		if (pattern == 1){
			TestImage[i] = rand() % 255 + 1;
			TestImage[i+1] = rand() % 255 + 1;
		}
		count+=2;
		}

	int TestImageValues = 0; 
	*/

	//for (int i = 0 ; i < image_rows*image_cols; i++){
		 //TestImageValues++;
		 //cout << TestImage[i] << ", ";

	//}
	

	//cout <<"there are " << TestImageValues << " values in the Test Image" << endl;
	}
	
	
	
	


	

