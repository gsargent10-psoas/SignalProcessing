#include "piwrapper.hpp"
#include "polarimetric.h"
#include <iostream>
#include <vector>
#include <string>


using namespace std;

void formImage(double* TestImage, double* Sub, vector<double*> SubImages);

int main()
{
	double* TestImage = new double[2040*2048]; //rows * columns TestImage
	double* Sub = new double[64*64]; //rows * columns SubImage
	vector<double*> SubImages;
	formImage(TestImage, Sub, SubImages);


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
	
	void formImage(double* TestImage, double* Sub, vector<double*> SubImages){ 

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

	for (int i = 0; i < 2040*2048; i+=2){ //rows x columns
	count+=2;
	
		if (count >= 2048){	

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
	for (int i = 0 ; i < 2040*2048; i++){
		 TestImageValues++;
		 //cout << TestImage[i] << ", ";

	}
	cout <<"there are " << TestImageValues << " values in the Test Image" << endl;
	
	//Generating SubImages portion
	
	//Sub count should go up to 64 for now, will make it changable based off of desired sub image size
	int SubCount = 0;
	int SubIndex = 0;
	int LastIndex = 0;
	int RowCount = 0;
	int SubVariable = 0;
	
	for (int i = 0; i < 2040*2048; i++){
		SubVariable++;
		if (SubVariable >= (64*64)){
			SubVariable = 0;
		}
		if (SubCount <= 64){
			Sub[SubVariable] = TestImage[i];
			SubCount++;
			} else {
				SubCount = 0;
				SubIndex = i+1; //Sets new starting index of next iteration 65 positions up. 
				i+= 2040; // sets current index 2048 positions up to get the next pattern of the subimage
				RowCount++; 
			} if (RowCount >= 65) {
			RowCount = 0;
			i = SubIndex;
			SubImages.push_back(Sub);
			
			} 
		}
	for (int i =0; i < 64*64; i++){
		cout << Sub[i] << ", " ;
	}
	cout << endl;
	}
	
	


	

