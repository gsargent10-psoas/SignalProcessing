#include "piwrapper.hpp"
#include "polarimetric.h"
#include <iostream>
#include <string>


using namespace std;

void formImage(double* TestImage, int image_rows, int image_cols);
//void formSubImage22(double* image, int image_rows, int image_cols, double* sub_images, int sub_rows, int sub_cols, int overlap, int num_sub, int* error);

int getNumberSubImages22(int image_y, int image_x, int sub_y, int sub_x, int overlap){
		 return (image_x*image_y)/((sub_x-2*overlap)*(sub_y-2*overlap)); //round up eventually
		

	
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
	
	formImage(TestImage, image_x, image_y);

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
	
	// status: 0 = succes, 1 = success, but number of subimages generated does not equal num_sub
	//-1 overlap is too large, -2 subimage is too large

	//*status = 0;

	//make Num_Sub large enough to hold all the subimages for now

	
	int xs = 0; int xe = sub_cols -1;
	int ys = 0; int ye = sub_rows -1;
	
	int index = 0;
	int count = 0;
	
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
						//cout << Test_Image[index] << ", ";
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
						//index = c+r*image_cols;
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
	
	/*
	
	int SubCount = 0;
	int LastIndex = 0;
	int RowCount = 0;
	int SubVariable = 0;
	int j = 0;

	bool FirstLine = true;
	
	int sub_colCount = 0;
	int sub_rowCount = 0;

    double Dsub_colAmount = double (image_cols) / double (sub_cols-2*overlap);  
	double Dsub_rowAmount = double (image_rows) / double (sub_rows-2*overlap);
	int Isub_colAmount = image_cols / sub_cols-2*overlap;
	int Isub_rowAmount = image_rows / sub_rows-2*overlap;
	
	cout << Dsub_colAmount << endl;
	cout << Dsub_rowAmount << endl;

	for (int i = 0; i < image_cols*image_rows; i++){
		
		j++;
		cout << j << ", ";
	 	
			if (SubVariable >= (64*64)){
			
			sub_colCount++;
			SubVariable = 0;
			
			j = LastIndex;
			//cout << j << ", ";
			
			
		
		}

		if (SubCount < 64){
			//Sub_Images[i] = Test_Image[j];
			SubCount++;
			
		}
		if (SubCount >= 64){
			if (FirstLine == true){
				LastIndex = i + 1; //sets next starting point 65 positions up, if this is the first run through
				FirstLine = false;
				}

			j+=image_rows-sub_rows; // sets current index 2448-64 positions up to get the next pattern of the subimage //Possible subtract the overlap from this
				
			RowCount++; //counts up to row 65 for the pattern
			SubCount = 0;
		} 	
		if (RowCount >= 65) {
			RowCount = 0;
			FirstLine = true;
			
		}

		if (sub_colCount >= Dsub_colAmount && Dsub_colAmount - Isub_colAmount < 1){
			double Dec = Dsub_colAmount - double(Isub_colAmount);
			cout << "Dec " << Dec;
	
			sub_colCount = 0;
			RowCount = 0;
			SubCount = 0;
		}

	SubVariable++; 
	}
	

	for (int i = 0; i < sub_rows*sub_cols*num_sub ; i++){
		//cout << Sub_Images[i] << ", ";
		}
		
	
	*/
	}
	
	
	
	


	

