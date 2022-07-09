#include "piwrapper.hpp"
extern "C"
{
#include "polarimetric.h"
}
#include <iostream>
#include <string>


using namespace std;

void formImage(double* TestImage, int image_rows, int image_cols);
void formSubImage22B(double* image, int image_rows, int image_cols, double* sub_images, int sub_rows, int sub_cols, int overlap, int num_sub, int* error);

int main()
{
	int image_x = 12; int image_y = 12; int sub_x = 10; int sub_y = 10; int overlap = 0;
	int error = 0;
	int* _error = &error; 
	
	double* TestImage = new double[image_x*image_y]; //rows * columns TestImage

	
	
	int sub_depth = getNumberSubImages22(image_y,image_x,sub_y,sub_x,overlap);
	cout << sub_depth << endl;
	cout << sub_x*sub_y*sub_depth << endl;
	double* SubImages = new double[sub_x*sub_y*sub_depth]; //rows * columns SubImage
	
	formImage(TestImage, image_y, image_x);

	formSubImage22B(TestImage, image_y, image_x, SubImages, sub_y, sub_x, overlap, sub_depth, _error);

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

	void formSubImage22B(double* Test_Image, int image_rows, int image_cols, double* Sub_Images, int sub_rows, int sub_cols, int overlap, int num_sub, int* error){


	//Generating SubImages portion
	//check if any overlap is happening, if the Subimage is cut off fill in the values with the expected pattern

	//overlap example create edges of Subimages with negative values of (generated overlap<- the index edge amount of the bad values
	
	// status: 0 = succes, 1 = success, but number of subimages generated does not equal num_sub
	//-1 overlap is too large, -2 subimage is too large

	//*status = 0;

	//make Num_Sub large enough to hold all the subimages for now

	
	int xs = 0; int xe = sub_cols -1;
	int ys = 0; int ye = sub_rows -1;
	
	int sxs = 0; 
	int sys = 0; 

	int index = 0;
	int count = 0;
	int subIndex = 0;

	for (; ys < image_rows-1;){
		xs = 0;
		for (; xs < image_cols-1;){
			//index = xs+ys*image_cols;
			//cout << ys+xs*image_cols << ", "; 

			if(xs >= image_cols-1 || ys >= image_rows-1){
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
							//cout << 0;
							Sub_Images[subIndex] = -49.0;
						}
						else if (c > image_cols-1){
							subIndex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
							//cout << 0;
							Sub_Images[subIndex] = -49.0;
						}
						else if(r > image_rows-1){
							subIndex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
							//cout << 0;
							Sub_Images[subIndex] = -49.0;
						}
						else {
							index = c+r*image_cols;
							subIndex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
							//cout << Test_Image[index] << ", ";
							Sub_Images[subIndex] = Test_Image[index]; 
							
						}
					}
				}

			//exceed pixel count on the right edge
			}
			else if(xe > image_cols-1){
				sxs = 0;
				sys = 0;
				for (int r = ys; r <= ye; r++, sys++){
					sxs = 0;
					for (int c = xs; c <= xe; c++, sxs++){
						if(c > image_cols-1){
							subIndex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
							//cout << 0;	
							Sub_Images[subIndex] = -99.0;
						}
						else{
							
							index = c+r*image_cols;
							subIndex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
							//cout << subIndex << ", ";
							//cout << Test_Image[index] << ", ";
							//cout << "(" << sxs << ", " << sys << ", " << count << ")" << ", ";
							Sub_Images[subIndex] = Test_Image[index]; 
						}
					}
				}
			//exceed pixel count on the bottom edge
			}
			else if(ye > image_cols-1){
				sxs = 0;
				sys = 0;
				for (int r = ys; r <= ye; r++, sys++){
					sxs = 0;
					for (int c = xs; c <= xe; c++, sxs++){
						if(r > image_rows-1){
							subIndex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
							//cout << 0;	
							Sub_Images[subIndex] = -99.0;
						}
						else{
							index = c+r*image_cols;
							subIndex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
							//cout << Test_Image[index] << ", ";
							Sub_Images[subIndex] = Test_Image[index]; 
						}
					}
				}
			}
			else {
				sxs = 0;
				sys = 0;
				for (int r = ys; r <= ye; r++, sys++){
					sxs = 0;
					for (int c = xs; c <= xe; c++, sxs++){
						index = c+r*image_cols;
						subIndex = sxs+sys*sub_cols+count*sub_cols*sub_rows;
						//cout << Test_Image[index] << ", ";
						Sub_Images[subIndex] = Test_Image[index]; 
					}
				}

			}
			count = count + 1;

			if(count >= num_sub){
				*error = 0;
				return;
			}

	 		xs = xe-(2*overlap)+1;
			if(xs % 2 != 0){
				xs = xs - 1;

			}
			
			xe = xs + sub_cols - 1;
		
		}
		ys = ye-(2*overlap)+1;	
		
		if(ys % 2 != 0){
				ys = ys - 1;

			}
			ye = ys + sub_rows - 1;
			}
		}
	
	
	
	


	

