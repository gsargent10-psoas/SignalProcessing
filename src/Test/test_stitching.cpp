#include "piwrapper.hpp"
#include "polarimetric.h"
#include <iostream>
#include <string>


using namespace std;

void formImage(double* TestImage);
void formSubImage22(double* image, int image_rows, int image_cols, double* sub_images, int sub_rows, int sub_cols, int overlap, int num_sub, int* error);

int getNumberSubImages22(int image_y, int image_x, int sub_y, int sub_x, int overlap){
	return (image_y * image_x) / ((sub_x - overlap) * (sub_y - overlap));
	
	}

int main()
{
	int image_x = 2448; int image_y = 2048; int sub_x = 64; int sub_y = 64; int overlap = 2;
	int* error = 0; 
	
	double* TestImage = new double[image_x*image_y]; //rows * columns TestImage

	
	
	int sub_depth = getNumberSubImages22(image_y,image_x,sub_y,sub_x,overlap);
	cout <<sub_depth << endl;
	cout << sub_x*sub_y*sub_depth << endl;
	double* SubImages = new double[sub_x*sub_y*sub_depth]; //rows * columns SubImage
	
	formImage(TestImage);

	formSubImage22(TestImage, image_y, image_x, SubImages, sub_y, sub_x, overlap, sub_depth, error);


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
	return 0;


	Stitching();
}
	void formImage(double* TestImage){ 

	
	//Create A large Test Image with a 2x2 Pattern
	//Create 64x64 SubImages

	//Stitch Subimages back into Large Image
	//compare input(Image) large image and Output(stitch image) large Image to make sure its the same.

	//Stitching algroithim is dependent on the Integer Pattern
	//but if the Integer pattern is changed it impacts The entire Stitching and subimage process
	
	//Generating Main Image portion

	// status: 0 = succes, 1 = success, but number of subimages generated does not equal num_sub
	//-1 overlap is too large, -2 subimage is too large

	//*status = 0;
	
	int pattern = 0;
	int count = 0;
	bool change = true;

	for (int i = 0; i < 2048*2448; i+=2){ //rows x columns
	count+=2;
	
		if (count >= 2448){	

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
	for (int i = 0 ; i < 2048*2448; i++){
		 TestImageValues++;
		 //cout << TestImage[i] << ", ";

	}
	
	
	
	cout <<"there are " << TestImageValues << " values in the Test Image" << endl;
	}

	void formSubImage22(double* Test_Image, int image_rows, int image_cols, double* Sub_Images, int sub_rows, int sub_cols, int overlap, int num_sub, int* error){

	//Generating SubImages portion
	//check if any overlap is happening, if the Subimage is cut off fill in the values with the expected pattern

	//overlap example create edges of Subimages with negative values of (generated overlap<- the index edge amount of the bad values
	
	int SubCount = 0;
	int LastIndex = 0;
	int RowCount = 0;
	int SubVariable = 0;
	int j = 0;

	bool FirstLine = true;
	
	double sub_colCount = 0;
	double sub_rowCount = 0;

    double sub_colAmount = image_cols / sub_cols;
	double sub_rowAmount = image_rows / sub_rows;
	cout << sub_colAmount << endl;
	cout << sub_rowAmount << endl;

	for (int i = 0; i < 2448*2048; i++){
		j++; 

	 if (sub_colCount <= sub_colAmount){
		if (SubVariable >= (64*64)){
			
			sub_colCount++;
			SubVariable = 0;
			
			j = LastIndex;
			//cout << j << ", ";
			
			
		}
	}
	//this is bad, its hard coded and I need to change this to handle different subsizes 
	//but for now it can handle when the rows are not divisible by 64
	if (sub_colCount >= sub_colAmount){
			j+=image_cols/4;
			sub_colCount = 0;
			RowCount = 0;
			SubCount = 0;
		}

	if (SubCount < 64){
		Sub_Images[i] = Test_Image[j];
		SubCount++;
			
		}
	if (SubCount >= 64){
			if (FirstLine == true){
				LastIndex = i + 1; //sets next starting point 65 positions up, if this is the first run through
				FirstLine = false;
				}

			j+=image_rows-sub_rows; // sets current index 2448-64 positions up to get the next pattern of the subimage
				
			RowCount++; //counts up to row 65 for the pattern
			SubCount = 0;

	} if (RowCount >= 65) {
		RowCount = 0;
		FirstLine = true;
			
		}
	SubVariable++; 
	}
	

	
	for(int i =0; i < 2448*2048; i++){
		cout << Sub_Images[i] << ", " ;
	
		
	}
	
	}
	
	


	

