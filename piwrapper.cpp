#include "piwrapper.hpp"
#include "PIO.h"

/*** Global Variables ***/
static CPIWriter writer();
static CPReader reader();

/*** Internal Functions ***/


/*** External Functions ***/

int openpiread(char *filename)
{
	std::string cpp_filename(filename);
	reader.openFile(cpp_filename, PI_FILE_FORMAT::PIFILE);
}

int openpiwriter(char *filename, int width, int height, int type, bool overwrite)
{
	std::string cpp_filename(filename);
	writer.openFile(cpp_filename, width, height, ENVI_DATA_TYPE::ENVI_DOUBLE, true, type, overwrite);
}

/* return: 0=no error; -1=width, height, and/or frame number are incorrect; */ 
int readpi(double *data, int width, int height, int index)
{
	if (getwidth() != width || getHeight() != height || getdepth() < index)
	{
		return -1;
	}
	vector<double> frame;
	reader.readFrame(frame, index);
	int tmp = 0;
	for(auto it = std::begin(v); it != std::end(v); ++it)
	{
		data[tmp] = *it;
		tmp = tmp + 1;
	}
	return 0;
}

int closereader()
{
	reader.closeFile();
	return 0;
}

int closewriter()
{
	return writer.closeFile();
}

int writepi(char *filename, double* data, int width, int height, int depth, int type, char *description, bool overwrite)
{
	
	writer(data, depth);
	closewriter();
	return 0;
}

int getwidth()
{
	return reader.getImageWidth();
}

int getheight()
{
	return reader.getImageHeight();
}

int getdepth()
{
	return reader.getNumFrames();
}