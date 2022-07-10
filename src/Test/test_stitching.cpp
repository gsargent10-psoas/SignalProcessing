#include "piwrapper.hpp"
extern "C"
{
#include "polarimetric.h"
}
#include <iostream>
#include <string>


using namespace std;

void formImage(double* TestImage, int image_rows, int image_cols);

int main()
{
	int image_x = 12; int image_y = 12; int sub_x = 10; int sub_y = 10; int overlap = 2;
	int error = -99;
	int* _error = &error; 
	
	double* TestImage = new double[image_x*image_y]; //rows * columns TestImage

	
	
	int sub_depth = getNumberSubImages22(image_y,image_x,sub_y,sub_x,overlap);
	cout << sub_depth << endl;
	cout << sub_x*sub_y*sub_depth << endl;
	double* SubImages = new double[sub_x*sub_y*sub_depth]; //rows * columns SubImage
	
	formImage(TestImage, image_y, image_x);

	formSubImage22(TestImage, image_y, image_x, SubImages, sub_y, sub_x, overlap, sub_depth, _error);

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
	cout << endl << "error: " << error <<endl;

/*
	string filename = "./test.pi";
	double data[4] = {1.0, 2.0, 3.0, 4.0};
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
	return 0;
	Stitching();
}
	void formImage(double* TestImage, int image_rows, int image_cols){ 

	
	//Create A large Test Image with a 2x2 Pattern
	//Create 64x64 SubImages

	//Stitch Subimages back into Large Image
	//compare input(Image) large image and Output(stitch image) large Image to make sure its the same.

	//Stitching algroithim is dependent on the Integer Pattern
	//but if the Integer pattern is changed it impacts The entire Stitching and subimage process
	
	//Generating Main Image portion

	
	int pattern = 0;
	int count = 0;
	bool change = true;

	for (int i = 0; i < image_rows*image_cols; i+=2){ //rows x columns
	count+=2;
	
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
			TestImage[i] = 90;
			TestImage[i+1] = 135;
		}
		if (pattern == 1){
			TestImage[i] = 45;
			TestImage[i+1] = 0;
		}
		
		
		}
	
	int TestImageValues = 0; 
	for (int i = 0 ; i < image_rows*image_cols; i++){
		 TestImageValues++;
		 //cout << TestImage[i] << ", ";

	}
	
	
	
	cout <<"there are " << TestImageValues << " values in the Test Image" << endl;
	}
	
	
	
	


	

