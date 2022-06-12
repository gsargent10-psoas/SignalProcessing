#include "piwrapper.hpp"
#include <iostream>
#include <string>
#include "polarimetric.h"

using namespace std;

int main()
{
	//Create A large Test Image with a 2x2 Pattern
	//Create 64x64 SubImages

	//Stitch Subimages back into Large Image
	//compare input(Image) large image and Output(stitch image) large Image to make sure its the same.

	//Stitching algroithim is dependent on the Integer Pattern
	//but if the Integer pattern is changed it impacts The entire Stitching and subimage process
	
	int TestImage[20][24];
	bool pattern;
	
	for (int i = 0; i < 20; i += 2){
		for (int j = 0; j< 24; j++){
			pattern = j % 2;
			//cout << pattern << endl;
			if (pattern == 0){
				TestImage[i][j] = 90;
				TestImage[i+1][j] = 135;
			} else {
				TestImage[i][j] = 45;
				TestImage[i+1][j] = 0;
		}	
			
		}	
	}
	
	
	for (int i = 0 ; i < 20; i++){
		for (int j = 0; j < 24; j++){
		cout << TestImage[i][j] << ", ";
		}
	cout <<endl;
	}
	

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
