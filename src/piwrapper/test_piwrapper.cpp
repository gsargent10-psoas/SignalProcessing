#include "piwrapper.hpp"
#include <iostream>
#include <string>

using namespace std;

int main()
{
	string filenamedouble = "./testdouble.pi";
	double datadouble[4] = {1.0, 2.0, 3.0, 4.0};
	string filenamefloat = "./testfloat.pi";
	float datafloat[4] = {1.0, 2.0, 3.0, 4.0};
	bool ret = openpiwriterdouble((char*) filenamedouble.c_str(),2,2,4,true);
	cout << "openpiwrite() return: " << ret << endl;
	int ret2 = writepidouble(&datadouble[0],1);
	cout << "writepi() return: " << ret2 << endl;
	closewriter();
	ret = openpiwriterfloat((char*) filenamefloat.c_str(),2,2,4,true);
	cout << "openpiwrite() return: " << ret << endl;
	ret2 = writepifloat(&datafloat[0],1);
	cout << "writepi() return: " << ret2 << endl;
	closewriter();
	ret = openpiread((char*) filenamefloat.c_str());
	cout << "openpiread() return: " << ret << endl;
	cout << "Width: " << getwidth() << ", Height: " << getheight() << endl;
	closereader();
	ret = openpiread((char*) filenamedouble.c_str());
	cout << "openpiread() return: " << ret << endl;
	cout << "Width: " << getwidth() << ", Height: " << getheight() << endl;
	closereader();
	return 0;




	
}
