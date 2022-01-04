#include "piwrapper.hpp"
#include "PIO.h"

/*** Global Variables ***/
static CPIWriter writer;
static CPReader reader;

/*** Internal Functions ***/


/*** External Functions ***/

bool openpiread(char *filename)
{
	std::string cpp_filename((const char*) filename);
	return reader.openFile(cpp_filename, PI_FILE_FORMAT::PIFILE);
}

int openpiwriter(char *filename, int width, int height, int type, bool overwrite)
{
	std::string cpp_filename((const char*) filename);
	return writer.openFile(cpp_filename, (size_t) width, (size_t) height, ENVI_DATA_TYPE::ENVI_DOUBLE, true, PI_PRODUCT_TYPE::INTENSITY, overwrite);
}

/* return: 0=no error; -1=width, height, and/or frame number are incorrect; */ 
int readpi(double *out, int width, int height, int index)
{
	if (getwidth() != width || getheight() != height || getdepth() < index)
	{
		return -1;
	}
	vector<double> frame;
	reader.readFrame(frame, index);
	int tmp = 0;
	for(auto it = std::begin(frame); it != std::end(frame); ++it)
	{
		out[tmp] = *it;
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

int writepi(double* data, int num_frames)
{
	
	writer.writeFrames(data, (size_t) num_frames);
	return 0;
}

int getwidth()
{
	return (int) reader.getImageWidth();
}

int getheight()
{
	return (int) reader.getImageHeight();
}

int getdepth()
{
	return reader.getNumFrames();
}