#include "piwrapper.hpp"
#include "polarimetric.h"
#include <iostream>
#include <string>


using namespace std;

void formImage(double* TestImage);

int main()
{
	double* TestImage = new double[2040*2048]; //rows * columns
	formImage(TestImage);


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
	
	
	for (int i = 0 ; i < 2040*2048; i++){
		 cout << TestImage[i] << ", ";
	}
	
	}


	

