#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "KeyValueParser.h"
#include "PIEnums.h"
//#include "PIO.h"
#include <math.h>
#include <string.h>

using namespace std;
#pragma once


//CPIFile Class -- This class holds members for all PI File header variables along with access functions to
//get and set them. Also contained within are filename and IO parameters that keep track of file handle,
//whether we are reading/writing to the header file, etc. The class also contains many utility functions for
//parsing PI header strings, etc.
class CPIFile{
public:
	//Class Constructors
	CPIFile(); //Creates an empty CPIFile object
	CPIFile(std::string& filename); //Creates CPIFile object for reading and reads in passed Header File
	//  CPIFile(CPIFile object); //Creates a CPIFile object for writing, but initializes all parameter values to those from the passed object

	void zeroOut(); //Initializes all member variables to default empty values.

	//Header Read/Write Functions
	int writePIHeaderFile();
	int readPIHeaderFile(std::string& filename);


	////////////////////////
	//Set Methods
	//Required header parameters
	void setWidth(size_t width){ m_width = width; m_width_valid = true; }
	void setHeight(size_t height){ m_height = height; m_height_valid = true; }
	void setNumberOfFrames(size_t frames){ m_frames = frames; m_frames_valid = true; }
	void setDataType(ENVI_DATA_TYPE dtype);
	void setByteOrder(bool byte_order){ m_byte_order = byte_order; m_byte_order_valid = true; }
	void setProductType(PI_PRODUCT_TYPE product_type){ m_product_type = product_type; m_product_type_valid = true; }
	//Optional Header Parameters
	void setDescription(std::string desc){ m_description = desc; }
    //-->Camera Parameters
    void setLensFocalLength(double fl){ m_lens_focal_length = fl; }
    void setLensFNumber(double fn){ m_lens_fnumber = fn; }
    void setPixelPitch(double pp) { m_pixel_pitch = pp; }
    void setNumberSpectralChannels(size_t nc){ m_spectral_channels = nc; }
    void setNumberPolarimetricChannels(size_t nc){ m_polarimetric_channels = nc; }
    void setADCBitDepth(size_t bd){ m_adc_bit_depth = bd; }
    void setExposureTime(long et){ m_exposure_time = et; }
    void setCameraGain(double g){ m_gain = g; }
    void setCameraGamma(double g){ m_gamma = g; }
    void setNumberFramesAveraged(size_t nf){ m_nframes_averaged = nf; }
    void setTimeStamp(string ts) { m_timestamp = ts; }
    void setSensorType(PI_SENSOR_TYPE sensor_type){ m_sensor_type = sensor_type; }
    void setSensorName(PI_SENSOR_NAME sensor_name){ m_sensor_name = sensor_name; }
    void setMicrogridSuperpixelSize(size_t numcols, size_t numrows);
    bool setMicrogridSuperpixelSize(std::string superpixelsize);
    bool setPolarizerOrientations(std::vector<double> &porientations);
    bool setPolarizerOrientations(std::string orientations);
    void setPolarizerSpectralOrientations(std::string sorientations) { m_polarizer_spectral_orientations = sorientations;}
    //-->Calibration Parameters
    void setFlatFieldValues(std::vector<double> vals){ m_flatfield_values = vals; }
	bool setFlatFieldValues(std::string values);
	void setFlatFieldUnit(RADIOMETRIC_UNIT unit){ m_flatfield_unit = unit; }
	void setFlatFieldSourceType(bool isEmissive){ m_flatfield_source_type = isEmissive; }
	void setFlatFieldCenterWavelength(double val, WAVELENGTH_UNIT unit){ m_flatfield_center_wavelength = val; m_flatfield_wavelength_unit = unit; }
	void setBlackbodyEmissivity(double val) { if (val < 0) m_blackbody_emissivity = 0.0; else if (val>1.0) m_blackbody_emissivity = 1.0; else m_blackbody_emissivity = val; }
    void setCameraAzimuth(double val) { m_camera_azimuth = val;}
    void setCameraElevation(double val) { m_camera_elevation = val;}
    void setTurntableAzimuth(double val) { m_turntable_azimuth = val;}
    void setSolarAzimuth(double val) { m_solar_azimuth = val;}
    void setSolarElevation(double val) { m_solar_elevation = val;}
	
	//Unknown Parameter List
	bool addPIParameter(std::string key, std::string value);
	void setUnknownParameters(std::vector<keyValuePair> &unknown_params){ m_unknown_parameters = unknown_params; }
	void appendUnknownParameter(std::string key, std::string value);
	//Internal Required Parameters	
	void setSwapData(bool swap_data){ m_swap_data = swap_data; }
	void setOutputFilenames(std::string data_file);
	void setInputHeaderValid(bool valid) { m_valid_header = valid; }

	////////////////////////
	//Get Methods
	size_t getWidth() const{ return m_width; }
	size_t getHeight() const{ return m_height; }
	size_t getNumberOfFrames() const{ return m_frames; }
	size_t getNumberOfBytesPerPixel() { if (m_data_type_valid) return m_bytes_per_pixel; else return 0; }
	size_t getNumberOfBytesPerFrame(){ if (m_width_valid & m_height_valid & m_data_type_valid) return m_width*m_height*m_bytes_per_pixel; else return 0; }
	size_t getNumberOfBytesPerFile(){ if (m_frames_valid) return m_frames*getNumberOfBytesPerFrame(); else return 0; }
	void getFileDimensions(size_t& width, size_t& height, size_t& numframes) const;
	void getFileDimensions(size_t& width, size_t& height, size_t& numframes, size_t& numbbp) const;
	ENVI_DATA_TYPE getDataType() const{ return m_data_type; }
	bool getByteOrder() const{ return m_byte_order; }
	bool GetSwapData() const{ return m_swap_data; }
    PI_PRODUCT_TYPE getProductType() const { return m_product_type; }
	std::string getDescription() const { return m_description; }
    double getLensFocalLength() const { return m_lens_focal_length; }
    double getLensFNumber() const { return m_lens_fnumber; }
    double getPixelPitch() const { return m_pixel_pitch; }
    size_t getNumberSpectralChannels() const { return m_spectral_channels; }
    size_t getNumberPolarimetricChannels() const { return m_polarimetric_channels; }
    size_t getADCBitDepth() const { return m_adc_bit_depth; }
    long getExposureTime() const { return m_exposure_time; }
    double getCameraGain() const { return m_gain; }
    double getCameraGamma() const { return m_gamma; }
    size_t getNumberFramesAveraged() { return m_nframes_averaged; }
    string getTimeStamp() {return m_timestamp; }
	PI_SENSOR_TYPE getSensorType() const{ return m_sensor_type; }
	PI_SENSOR_NAME getSensorName() const{ return m_sensor_name; }
    void getMicrogridSuperpixelSize(std::vector<size_t> &sps) { sps = m_ugrid_superpixel_size; }
    void getPolarizerOrientations(std::vector<double> &sps) { sps = m_polarizer_orientations; }
    std::string getPolarizerSpectralOrientations() { return m_polarizer_spectral_orientations; }
	void getFlatFieldValues(std::vector<double> &vals) const{ vals = m_flatfield_values; }
	RADIOMETRIC_UNIT getFlatFieldUnit() const{ return m_flatfield_unit; }
	bool getFlatFieldSourceType() { return m_flatfield_source_type; }
	void getFlatFieldCenterWavelength(double &val, WAVELENGTH_UNIT &unit){ val = m_flatfield_center_wavelength; unit = m_flatfield_wavelength_unit; }
	double getBlackbodyEmissivity() { return m_blackbody_emissivity; }
    double getCameraAzimuth() { return m_camera_azimuth;}
    double getCameraElevation() { return m_camera_elevation;}
    double getTurntableAzimuth() { return m_turntable_azimuth;}
    double getSolarAzimuth() { return m_solar_azimuth;}
    double getSolarElevation() { return m_solar_elevation;}

	void getUnknownParameters(std::vector<keyValuePair> &unknownParams) const { unknownParams = m_unknown_parameters; }
	std::string findUnknownParameterValue(std::string &key){ int idx = findUnknownParameterIndex(key); if (idx >= 0) return m_unknown_parameters[idx].value; else return ""; }
	std::string getDataFilename() const{ return m_filename; }
	std::string getHeaderFilename() const{ return m_header_filename; }
	bool isInputHeaderValid() const { return m_valid_header; }

	////////////////////////
	//Public Utility Functions
	bool doDataAndHeaderFilesExist(std::string &fname);

private:

	//Utility Functions
	int  findUnknownParameterIndex(std::string key);
	bool parseUnknownParameterValueFieldToString(std::string valuestring, std::vector<std::string> &values);
	bool parseUnknownParameterValueFieldToDouble(std::string valuestring, std::vector<double> &values);
	bool parseUnknownParameterValueFieldToInt(std::string valuestring, std::vector<int> &values);
	bool parseUnknownParameterValueFieldToSizeT(std::string valuestring, std::vector<size_t> &values);
	bool parseUnknownParameterValueFieldToBool(std::string valuestring, std::vector<bool> &values);
	bool deleteUnknownParameter(std::string key);
	bool isKnownParameter(std::string);

	//File Utility Functions
	void setDataFilename(std::string data_file){ m_filename = data_file; }
	void setHeaderFilename(std::string header_file){ m_header_filename = header_file; }
	bool fileExists(std::string &filename);
	bool verifyOutputFilenames();
	bool verifyValidPIHeaderFile(std::string &fname);
	bool wereCriticalHeaderParametersSet();
	bool areCriticalHeaderParametersSetForWriting();	
	bool isMachineLittleEndian();

	//PI Required Header Parameters
	size_t m_frames;									//Number of frames
	size_t m_width;         							//Image width
	size_t m_height;									//Image height    
	size_t m_bytes_per_pixel;							//Bytes per pixel
	bool m_byte_order;									//Byte Order (0=intel, 1=other)
	ENVI_DATA_TYPE m_data_type; 						//File Type (e.g., 12=short)
	PI_PRODUCT_TYPE m_product_type;						//PI data product type

	//PI Non-required, but known parameters    
	std::string m_description;       					//Description field
    //-->camera parameters<--
    double m_lens_focal_length;                         //Camera lens focal length
    double m_lens_fnumber;                              //Camera lens f/number
    double m_pixel_pitch;                               //Camera pixel pitch
    size_t m_spectral_channels;                         //Number of Image Spectral Channels
    size_t m_polarimetric_channels;                     //Number of Image Polarimetric Channels
    size_t m_adc_bit_depth;                             //ADC bit depth
    long m_exposure_time;                               //Exposure time in microseconds
    double m_gain;                                      //Camera gain value
    double m_gamma;                                     //Camera gamma value
    size_t m_nframes_averaged;                          //Number of frames averaged
    string m_timestamp;                                 //Time stamp of when data was collected
	PI_SENSOR_TYPE m_sensor_type;						//PI sensor type
	PI_SENSOR_NAME m_sensor_name;						//PI sensor name
    std::vector<size_t> m_ugrid_superpixel_size;		//This is a 2 element size_t vector specifying the size of a superpixel within a microgrid sensor.
    std::vector<double> m_polarizer_orientations;       //This is an N element vector array of doubles that specify the polarizer orientations of each polarized intensity channel
                                                            //This is used for all polarization sensors. For ugrid sensors, the values are specified row-wise
    std::string m_polarizer_spectral_orientations;      //This is an N-length string that specifies the color of each microgrid pixel (M=mono,R=red,G=green,B=blue)
                                                        //location within the microgrid superpixel (specified row-wise)

    //Camera Geometry Parameters
    double m_camera_azimuth;                            //This specifies the azimuthal angle of the sensor (0 is north)
    double m_camera_elevation;                          //This specifies the elevation angle of the sensor (90 is zenith, look downward)

    //Scene Geometry Parameters
    double m_turntable_azimuth;                         //This specifies the azimuthal angle of the laboratory object positioning turntable

    //Solar Geometry Parameters
    double m_solar_azimuth;                             //This specifies the azimuthal angle of the sun (0 is north) or lab solar source
    double m_solar_elevation;                           //This specifies the elevation angle of the sun (90 is zenith) or lab solar source

    //-->Calibration parameters<--
	RADIOMETRIC_UNIT m_flatfield_unit;					//If the product type is a flatfield file, this is a string describing the units
	std::vector<double> m_flatfield_values;				//If the product type is a flatfield file, this is a value specifying the flat field values
														// If this is a single flat-field images this vector will have a single value. If it is a flat-field file
														// containing multiple averaged flat field images this vector will contain a value for each flat field image
	bool m_flatfield_source_type;						// True = emissive source, false = reflective source
	double m_flatfield_center_wavelength;				//Center wavelength of the flat field souce (for radiometry calculations)
	WAVELENGTH_UNIT m_flatfield_wavelength_unit;		//Unit of wavelength
	double m_blackbody_emissivity;						//Emissivity value for an emissive blackbody



	//Other PI File Parameters
	std::string m_filename;            					//PI file associated with this object
	std::string m_header_filename;						//PI header file associated with filename
	std::vector<std::string> m_known_parameters;		//Names of Known Parameter List (i.e., these are expected and parsed into local variables)
	std::vector<keyValuePair> m_unknown_parameters;		//Remaining PI Header Fields are Stored Here
	bool m_swap_data;									//Whether data bytes need to be swapped

	//Critical Valid Header Parameter Flags
	bool m_valid_header;		//Used in read mode when a header file opened and read in successfully
	bool m_frames_valid;		//required header parameter
	bool m_width_valid;			//required header parameter
	bool m_height_valid;		//required header parameter
	bool m_byte_order_valid;	//required header parameter
	bool m_data_type_valid;		//required header parameter
	bool m_product_type_valid;	//required header parameter

	bool debug_mode;
};







/*--------------------------------------------------------------------------------------------
BEGIN - CPReader Class
This class is for reading polarimetric data files in various file formats. The file formats
specified in PI_FILE_FORMAT are currently supported at various levels:

	PIFILE			.pi files and associated header files. This file type is fully supported.
	AFRL_PIRATE		PIRATE sensor .raw files. This file type is fully supported.
	POLARIS_PYXIS	Basic support is provided for reading these files currently. As of now the
					file header and frame headers are bypassed (needs to be added for serious
					support of these files).

*/
class CPReader{
public:

	//--------------------------------------------
	// Methods

	//********************************
	//Constructors
	//Create empty class with all variables initialized to null.
	CPReader(); 
	//Instantiate class and open passed filename for reading (file type is implied by file extension)
	// .pi extensions are assumed to be PIFILE data files
	// .raw extensions are assumed to be PIRATE data files (conflicts with PYXIS raw data)
	CPReader(string& filename);	
	//Instantiate class and open passed filename for reading (file type is specified regardless of file extension)
	CPReader(string& filename, PI_FILE_FORMAT fileformat);
	~CPReader();

	//********************************
	//Public methods for reading data
	bool openFile(string& filename, PI_FILE_FORMAT fileformat);
	template <class T> int readFrame(vector<T> &frame, size_t fnumber);

	//********************************
	//Get Methods
	bool   isFileOpen() const { return m_fileopen; }
	bool   areDimensionsSet() const { return m_dimensions_set; }
	bool   isNumFramesSet() const { return m_numframes_set; }
	size_t getImageWidth() const { return m_width; }
	size_t getImageHeight() const { return m_height; }
	size_t getNumFrames() const { return m_numframes; }
	size_t getBytesPerPixel() const { return m_bpp; }
	size_t getBytesPerFrame() const { return m_bpf; }
	size_t getNumFileHeaderBytes() const { return m_header_bytes; }
	size_t getNumFrameHeaderBytes() const { return m_frameheader_bytes; }
	ENVI_DATA_TYPE getDataType() const { return m_datatype; }
	bool getByteOrder() const { return m_byte_order; }
	PI_FILE_FORMAT getFileFormat() const { return m_fileformat; }
	PI_FILE_FORMAT getFileFormat(std::string &filename);
	size_t getCurrentFrame() const { return m_currentframe; }
	void   getFilename(string& filename) const { filename = m_filename; }

	//Additional PI File get methods. Only will return defined values when the file format is a PI File.
	std::string getPIFileDescription() const { if (m_fileformat == PIFILE) return m_pifile.getDescription(); else return ""; }
    PI_PRODUCT_TYPE getPIFileProductType() { if (m_fileformat == PIFILE) return m_pifile.getProductType(); else return UNKNOWN_PRODUCT_TYPE; }
    PI_SENSOR_TYPE getPIFileSensorType() const{ if (m_fileformat == PIFILE) return m_pifile.getSensorType(); else return UNKNOWN_SENSOR_TYPE; }
    PI_SENSOR_NAME getPIFileSensorName() const{ if (m_fileformat == PIFILE) return m_pifile.getSensorName(); else return UNKNOWN_SENSOR_NAME; }
    double getPIFileLensFocalLength() { return m_pifile.getLensFocalLength(); }
    double getPIFileLensFNumber() { return m_pifile.getLensFNumber(); }
    double getPIFilePixelPitch() { return m_pifile.getPixelPitch(); }
    size_t getPIFileNumberSpectralChannels() { return m_pifile.getNumberSpectralChannels(); }
    size_t getPIFileNumberPolarimetricChannels() { return m_pifile.getNumberPolarimetricChannels(); }
    size_t getPIFileADCBitDepth() { return m_pifile.getADCBitDepth(); }
    long getPIFileExposureTime() { return m_pifile.getExposureTime(); }
    double getPIFileCameraGain() { return m_pifile.getCameraGain(); }
    double getPIFileCameraGamma() { return m_pifile.getCameraGamma(); }
    size_t getPIFileNumberFramesAveraged() { return m_pifile.getNumberFramesAveraged(); }
    string getTimeStamp() { return m_pifile.getTimeStamp(); }
    void getPIFileMicrogridSuperpixelSize(std::vector<size_t> &sps) { if (m_fileformat == PIFILE) m_pifile.getMicrogridSuperpixelSize(sps); else sps.clear(); }
    void getPIFilePolarizerOrientations(std::vector<double> &mpo) { if (m_fileformat == PIFILE) m_pifile.getPolarizerOrientations(mpo); else mpo.clear(); }
    std::string getPIFilePolarizerSpectralOrientations() { return m_pifile.getPolarizerSpectralOrientations(); }
    double getCameraAzimuth() { return m_pifile.getCameraAzimuth();}
    double getCameraElevation() { return m_pifile.getCameraElevation();}
    double getTurntableAzimuth() { return m_pifile.getTurntableAzimuth();}
    double getSolarAzimuth() { return m_pifile.getSolarAzimuth();}
    double getSolarElevation() { return m_pifile.getSolarElevation();}

	void getPIFileFlatFieldValues(std::vector<double> &vals) { if (m_fileformat == PIFILE) m_pifile.getFlatFieldValues(vals); else vals.clear(); }
	RADIOMETRIC_UNIT getPIFileFlatFieldUnit() { if (m_fileformat == PIFILE) return m_pifile.getFlatFieldUnit(); else return UNKNOWN_RADIOMETRIC_UNIT; }
	bool getFlatFieldSourceType(){ return m_pifile.getFlatFieldSourceType(); }	
	void getFlatFieldCenterWavelength(double &val, WAVELENGTH_UNIT &unit){ m_pifile.getFlatFieldCenterWavelength(val, unit); }	
    double getBlackbodyEmissivity() { return m_pifile.getBlackbodyEmissivity(); }

	void getPIFileUnknownParameters(std::vector<keyValuePair> &unknownParams) const { if (m_fileformat == PIFILE) m_pifile.getUnknownParameters(unknownParams); else unknownParams.clear(); }
	std::string findPIFileUnknownParameterValue(std::string &key) { if (m_fileformat == PIFILE) return m_pifile.findUnknownParameterValue(key); else return ""; }

private:
	//-------------------------------------------
	// Private Member Variables		
	size_t m_width, m_height, m_numframes, m_bpp, m_header_bytes, m_frameheader_bytes; //Image dimensions and file type
	bool m_byte_order;
	PI_FILE_FORMAT m_fileformat;
	ENVI_DATA_TYPE m_datatype;	
	size_t m_currentframe, m_bpf;
	string m_filename; //Name of the data file to be read
	bool m_fileopen; //True if a file has been successfully opened
	bool m_dimensions_set; //True if image dimensions have been set for the specified file type
	bool m_numframes_set; //True if number of frames in the data file have been calculated
	bool m_validframe; //True if last attempt to read a frame was successful	
	ifstream m_fin; //Input file reader object;
	vector<char> buffer;
	CPIFile m_pifile; //Used only when file type is a PIFile

	bool debug_mode;

	//-------------------------------------------
	// Private Methods
	bool setFileTypeDimensions(); //Requires m_fileopen to be true and m_filetype to be set to a valid file type
	void calculateNumFrames(); //Requires m_fileopen and m_dimensions_set to be true	
	void setBytesPerPixel(ENVI_DATA_TYPE dt); //Set the member variable m_bpp based upon the ENVI data type
	void zeroOut();

	//File Format Definitions
	void setPIFILEDimensions();
	void setPIRATEDimensions();
	void setPYXISDimensions();

	//Data Read/Write Functions
	template <class T> int readCurrentFrame(vector<T> &frame);

	//Data Conversion Functions
	template <class T> void convertCharBufferToTypeT(vector<char> &cdata, vector<T> &ddata, ENVI_DATA_TYPE dt);
};




//This reads the specified frame number from the currently open data file and returns it in the frame vector as type T
template <class T> int CPReader::readFrame(vector<T> &frame, size_t fnumber){

	//Seek file if needed and read specified frame
	if (isFileOpen() & areDimensionsSet() & isNumFramesSet()){
		//Make sure frame number if valid
		if (fnumber >= getNumFrames()) return -2;

		//Seek to specified frame number if necessary
		if (m_currentframe != fnumber){
			m_fin.seekg(fnumber*m_bpf + m_header_bytes, m_fin.beg);
			m_currentframe = fnumber;
		}
		return  readCurrentFrame(frame);
		
	}
	else return -1;
}

//This reads the currently pointed to image frame and returns it in the frame vector as type T
template <class T> int CPReader::readCurrentFrame(vector<T> &frame){
	//Seek file if needed and read specified frame
	if (isFileOpen() & areDimensionsSet() & isNumFramesSet()){

		if (m_currentframe < m_numframes){
			//For now if frame header just skip over it
			if (m_frameheader_bytes != 0) m_fin.seekg(m_frameheader_bytes, m_fin.cur);

			//Check that passed frame buffer is properly allocated
			if (frame.size() != getImageWidth()*getImageHeight()) frame.resize(getImageWidth()*getImageHeight());
			
			//Read current frame from file.
			m_fin.read(&buffer[0], m_bpf);

			//Convert byte data to double
			convertCharBufferToTypeT(buffer, frame, m_datatype);

			//Set Current Frame to Next Frame
			m_currentframe++;
			return 0;
		}
		else return -2;
	}
	return -1;
}

//Cast the char data vector cdata into the specified ENVI_DATA_TYPE and then return in the vector ddata of type T
//Thus, dt specifies how to interpret the char buffer prior conversion to the T buffer
template <class T> void CPReader::convertCharBufferToTypeT(vector<char> &cdata, vector<T> &ddata, ENVI_DATA_TYPE dt){
	switch (dt){
	case ENVI_CHAR:		// char
		char *ptr1;  ptr1 = (char*)&cdata[0];
		for (size_t k = 0; k<ddata.size(); k++) ddata[k] = (T)ptr1[k];
		break;
	case ENVI_SIGNED_SHORT:		// signed short
		short *ptr2;  ptr2 = (short*)&cdata[0];
		for (size_t k = 0; k<ddata.size(); k++) ddata[k] = (T)ptr2[k];
		break;
	case ENVI_INTEGER:		// integer
		int *ptr3;  ptr3 = (int*)&cdata[0];
		for (size_t k = 0; k<ddata.size(); k++) ddata[k] = (T)ptr3[k];
		break;
	case ENVI_FLOAT:		// float
		float *ptr4;  ptr4 = (float*)&cdata[0];
		for (size_t k = 0; k<ddata.size(); k++) ddata[k] = (T)ptr4[k];
		break;
	case ENVI_DOUBLE:		// double
		double *ptr5;  ptr5 = (double*)&cdata[0];
		for (size_t k = 0; k<ddata.size(); k++) ddata[k] = (T)ptr5[k];
		break;
	case ENVI_UNSIGNED_SHORT:	// unsigned short
		unsigned short *ptr6;  ptr6 = (unsigned short*)&cdata[0];
		for (size_t k = 0; k<ddata.size(); k++) ddata[k] = (T)ptr6[k];
		break;
	case ENVI_UNSIGNED_INT:	// unsigned int
		unsigned int *ptr7;  ptr7 = (unsigned int*)&cdata[0];
		for (size_t k = 0; k<ddata.size(); k++) ddata[k] = (T)ptr7[k];
		break;
	case ENVI_I64:	// long long
		long long *ptr8;  ptr8 = (long long int*)&cdata[0];
		for (size_t k = 0; k<ddata.size(); k++) ddata[k] = (T)ptr8[k];
		break;
	case ENVI_UNSIGNED_I64:        // unsigned long long
		unsigned long long *ptr9;  ptr9 = (unsigned long long int*)&cdata[0];
		for (size_t k = 0; k<ddata.size(); k++) ddata[k] = (T)ptr9[k];
		break;
	default:
		break;
	}
}







/*--------------------------------------------------------------------------------------------
This class is for writing polarimetric data files in .pi file format. The data are output 
as a raw binary format with an acompanying ASCII header file that follows a format similar to
that of ENVI files (i.e., key-value pairs). The data files themselves have no header or frame
headers, but instead simply contain the raw binary data.

Required paramters are:
	filename:			Specifies the filename (with path) to write to disk. The extension ".pi" will be added if it is missing. (string)
	width:				Width of the image files to be written. (unsigned integer)
	height:				Height of the image files to be written. (unsigned integer)
	data type:			Specifies the format of the data type and follows the ENVI convention. (ENVI_DATA_TYPE)
	byte order:			Specifies endiannes of the data. (boolean - false = little endian, true = big endian)
	product type:		Specifies one of the PI product types specified by PI_PRODUCT. (PI_PRODUCT_TYPE)

The number of image frames is also a required header parameter, but the class keeps track of how
many frames are written to disk. The header filename is automatically generated by simply appending
a ".hdr" extension to the base raw data file (".pi") file.

Optional reader-recognized parameters:
	description:		Detailed description of the data file. (string)
	data units:			Units of the data. (string)

Custom header parameters can be added using the function addKeyValue(...).

Example usage instructions:

*/
class CPIWriter{
public:

	//--------------------------------------------
	// Methods

	//*******************************************************************************************************************
	//Constructors
	//
	//Instantiates an empty CPWriter object with all variables zeroed out.
	CPIWriter();
	//Instantiate class with minimum required parameters and open file for writing.
	CPIWriter(string &filename, size_t width, size_t height, ENVI_DATA_TYPE dt, bool byte_order, PI_PRODUCT_TYPE pt, bool overwrite);
	//Instantiate class with required parameters
	~CPIWriter();

	//*******************************************************************************************************************
	//Public methods for writing data
	//
	//Opens PI file for writing. All required minimum parameters are required except for the number of frames. The number of frames is
	// determined based upon how many frames are written to disk. The overwrite flag specifies whether to overwrite an existing file 
	// if one exists (header is not written until closeFile() is called.
	int openFile(string& filename, size_t width, size_t height, ENVI_DATA_TYPE dt, bool byte_order, PI_PRODUCT_TYPE pt, bool overwrite);

	//Will write the passed frame buffer to the data file (i.e., the number of specified frames is written to disk).
	//Returns 0 if data was successfully written to file.
	//Returns -1 if the data file is not open for writing
	//Returns -2 if the number of elements in frames does not match the getWidth()*getHeight()*getNumberofFrames() in m_pifile_parameters
	//Returns -3 if numframes < 1
	//Returns -4 if there was a problem writing file to disk.
	//NOTE: No check is performed to determine if the passed vector type matches the specified ENVI_DATA_TYPE--thus the user must take care
	template <typename T> int writeFrames(vector<T> &frames, size_t numframes);
    //This version will write a subset of frames passed from starting_frames to ending_frames
    template <typename T> int writeFramesRange(vector<T> &frames, size_t starting_frame, size_t ending_frame);
	//This overloaded version will allow for a pointer to be passed instead of a vector. This is to
	//support writing the stream directly from the camera.
	//Note that in this case the size of the images pointed to by *T cannot be verified for size.
	template <typename T> int writeFrames(T* frames, size_t numframes);

	//Will close the current data file and write out the corresponding header.
	//NOTE: If file is closed and no frames have been written to the data file then no data or header file will be written.
	int closeFile();

	//*******************************************************************************************************************
	//Public helper functions that can be called with an empty CPWriter instantiation
	//
	//Will create a .pi header file for the specified PIRATE raw file and change the data file extension to .pi
	bool convertPirateFileToPIFile(string& filename);

	//*******************************************************************************************************************
	//Get Methods
	bool   isFileOpen() const { return m_fileopen; }
	size_t getImageWidth() const { return m_pi_parameters.getWidth(); }
	size_t getImageHeight() const { return m_pi_parameters.getHeight(); }
	size_t getNumFrames() const { return m_pi_parameters.getNumberOfFrames(); }
	size_t getBytesPerPixel() { return m_pi_parameters.getNumberOfBytesPerPixel(); }
	size_t getBytesPerFrame() { return m_pi_parameters.getNumberOfBytesPerFrame(); }
	size_t getNumberOfBytesPerFile() { return m_pi_parameters.getNumberOfBytesPerFile(); }
	ENVI_DATA_TYPE getDataType() const { return m_pi_parameters.getDataType(); }
	size_t getByteOrder() const { return m_pi_parameters.getByteOrder(); }
	void getDataFilename(string& filename) const { filename = m_pi_parameters.getDataFilename(); }
	void getHeaderFilename(string& headerfilename) const { headerfilename = m_pi_parameters.getHeaderFilename(); }
	PI_PRODUCT_TYPE getProductType() { return m_pi_parameters.getProductType(); }
	std::string getDescription() const { return m_pi_parameters.getDescription(); }
	PI_SENSOR_TYPE getSensorType() const{ return m_pi_parameters.getSensorType(); }
	PI_SENSOR_NAME getSensorName() const{ return m_pi_parameters.getSensorName(); }
    double getPIFileLensFocalLength() { return m_pi_parameters.getLensFocalLength(); }
    double getPIFileLensFNumber() { return m_pi_parameters.getLensFNumber(); }
    double getPIFilePixelPitch() { return m_pi_parameters.getPixelPitch(); }
    size_t getPIFileNumberSpectralChannels() { return m_pi_parameters.getNumberSpectralChannels(); }
    size_t getPIFileNumberPolarimetricChannels() { return m_pi_parameters.getNumberPolarimetricChannels(); }
    size_t getPIFileADCBitDepth() { return m_pi_parameters.getADCBitDepth(); }
    long getPIFileExposureTime() { return m_pi_parameters.getExposureTime(); }
    double getPIFileCameraGain() { return m_pi_parameters.getCameraGain(); }
    double getPIFileCameraGamma() { return m_pi_parameters.getCameraGamma(); }
    size_t getPIFileNumberFramesAveraged() { return m_pi_parameters.getNumberFramesAveraged(); }
    string getTimeStamp() { return m_pi_parameters.getTimeStamp(); }
    void getPIFileMicrogridSuperpixelSize(std::vector<size_t> &sps) { m_pi_parameters.getMicrogridSuperpixelSize(sps); }
    void getPIFilePolarizerOrientations(std::vector<double> &mpo) { m_pi_parameters.getPolarizerOrientations(mpo); }
    std::string getPIFilePolarizerSpectralOrientations() { return m_pi_parameters.getPolarizerSpectralOrientations(); }
    double getPIFileCameraAzimuth() { return m_pi_parameters.getCameraAzimuth();}
    double getPIFileCameraElevation() { return m_pi_parameters.getCameraElevation();}
    double getPIFileTurntableAzimuth() { return m_pi_parameters.getTurntableAzimuth();}
    double getPIFileSolarAzimuth() { return m_pi_parameters.getSolarAzimuth();}
    double getPIFileSolarElevation() { return m_pi_parameters.getSolarElevation();}

	void getPIFileFlatFieldValues(std::vector<double> &vals) { m_pi_parameters.getFlatFieldValues(vals); }
	RADIOMETRIC_UNIT getPIFileFlatFieldUnit() { return m_pi_parameters.getFlatFieldUnit(); }
	bool getFlatFieldSourceType(){ return m_pi_parameters.getFlatFieldSourceType(); }
	void getFlatFieldCenterWavelength(double &val, WAVELENGTH_UNIT &unit){ m_pi_parameters.getFlatFieldCenterWavelength(val, unit); }
    double getBlackbodyEmissivity() { return m_pi_parameters.getBlackbodyEmissivity(); }

	void getUnknownParameters(std::vector<keyValuePair> &unknownParams) const { m_pi_parameters.getUnknownParameters(unknownParams); }
	std::string findUnknownParameterValue(std::string &key) { return m_pi_parameters.findUnknownParameterValue(key); }
	

	//*******************************************************************************************************************
	//Set Methods
	//
	//Note: Required header parameters cannot be directly set--These can only be set using the openFile(..) function
	//These are parameters: width, height, data type, byte order, product type, and output filename
	//
	//Optional Header Parameters
	void setDescription(std::string desc){ m_pi_parameters.setDescription(desc); }    
	void setSensorType(PI_SENSOR_TYPE sensor_type){ m_pi_parameters.setSensorType(sensor_type); }
	void setSensorName(PI_SENSOR_NAME sensor_name){ m_pi_parameters.setSensorName(sensor_name); }
    void setLensFocalLength(double fl) { m_pi_parameters.setLensFocalLength(fl); }
    void setLensFNumber(double fn) { m_pi_parameters.setLensFNumber(fn); }
    void setPixelPitch(double pp) { m_pi_parameters.setPixelPitch(pp); }
    void setNumberSpectralChannels(size_t nc) { m_pi_parameters.setNumberSpectralChannels(nc); }
    void setNumberPolarimetricChannels(size_t nc) { m_pi_parameters.setNumberPolarimetricChannels(nc); }
    void setADCBitDepth(size_t bd) { m_pi_parameters.setADCBitDepth(bd); }
    void setExposureTime(long et) { m_pi_parameters.setExposureTime(et); }
    void setCameraGain(double g) { m_pi_parameters.setCameraGain(g); }
    void setCameraGamma(double g) { m_pi_parameters.setCameraGamma(g); }
    void setNumberFramesAveraged(size_t nf) { m_pi_parameters.setNumberFramesAveraged(nf); }
    void setTimeStamp(string ts) { m_pi_parameters.setTimeStamp(ts); }
    void setMicrogridSuperpixelSize(size_t numcols, size_t numrows) { m_pi_parameters.setMicrogridSuperpixelSize(numcols,numrows); }
    void setPolarizerOrientations(std::vector<double> &mpo) { m_pi_parameters.setPolarizerOrientations(mpo); }
    void setPolarizerSpectralOrientations(std::string so) { m_pi_parameters.setPolarizerSpectralOrientations(so); }
    void setCameraAzimuth(double val){ m_pi_parameters.setCameraAzimuth(val);}
    void setCameraElevation(double val){ m_pi_parameters.setCameraElevation(val);}
    void setTurntableAzimuth(double val){ m_pi_parameters.setTurntableAzimuth(val);}
    void setSolarAzimuth(double val){ m_pi_parameters.setSolarAzimuth(val);}
    void setSolarElevation(double val){ m_pi_parameters.setSolarElevation(val);}

    void setFlatFieldValues(std::vector<double> vals){ m_pi_parameters.setFlatFieldValues(vals); }
	void setFlatFieldUnit(RADIOMETRIC_UNIT unit){ m_pi_parameters.setFlatFieldUnit(unit); }
	void setFlatFieldSourceType(bool isEmissive){ m_pi_parameters.setFlatFieldSourceType(isEmissive); }
	void setFlatFieldCenterWavelength(double val, WAVELENGTH_UNIT unit){ m_pi_parameters.setFlatFieldCenterWavelength(val, unit); }
	void setBlackbodyEmissivity(double val) { m_pi_parameters.setBlackbodyEmissivity(val); }

	//Unknown Parameter List
	void setUnknownParameters(std::vector<keyValuePair> &unknown_params){ m_pi_parameters.setUnknownParameters(unknown_params); }
	void appendUnknownParameter(std::string key, std::string value){ m_pi_parameters.appendUnknownParameter(key, value); }

private:

	//-------------------------------------------
	// Private Utility Methods
	void zeroOut();
	template<typename T> void binary_write(const T& value);

	//-------------------------------------------
	// Private Member Variables	
	CPIFile m_pi_parameters;
	ofstream m_fout;
	bool m_fileopen;

	bool debug_mode;
};


template<typename T> void CPIWriter::binary_write(const T& value){
	//cout << value << " <-- At binary write function\n;";
	if (isFileOpen()) m_fout.write(reinterpret_cast<const char*>(&value), sizeof(T));
}

//Will write the passed frame buffer to the data file (i.e., the number of specified frames is written to disk).
//Returns 0 if data was successfully written to file.
//Returns -1 if the data file is not open for writing
//Returns -2 if the number of elements in frames is not enough to support the specified frame range
//Returns -3 if numframes < 1
//NOTE: No check is performed to determine if the passed vector type matches the specified ENVI_DATA_TYPE. Thus, the user must take care
//		to ensure that the correct data type is specified
template <typename T>	int CPIWriter::writeFrames(vector<T> &frames, size_t numframes){

	//Ensure there is at least 1 data frame
	if (numframes > 0){
		//Verify data is of expected size
		if (frames.size() != m_pi_parameters.getWidth()*m_pi_parameters.getHeight()*numframes) return -2;
		else{
			//Ensure that file is open
			if (isFileOpen()){
				//Write frames to disk
				for (size_t k = 0; k < m_pi_parameters.getWidth()*m_pi_parameters.getHeight()*numframes; k++){
					binary_write(frames[k]);
				}
				m_pi_parameters.setNumberOfFrames(m_pi_parameters.getNumberOfFrames() + numframes);
				return 0;
			}
			else return -1;
		}
	}
	else return -3;
}

//Will write the range of frames specified within the passed frame buffer to the data file.
//Returns 0 if data was successfully written to file.
//Returns -1 if the data file is not open for writing
//Returns -2 if the number of elements in frames does not match the getWidth()*getHeight()*getNumberofFrames() in m_pifile_parameters
//Returns -3 if numframes < 1
//NOTE: No check is performed to determine if the passed vector type matches the specified ENVI_DATA_TYPE. Thus, the user must take care
//		to ensure that the correct data type is specified
template <typename T> int CPIWriter::writeFramesRange(vector<T> &frames, size_t start, size_t end){

    //Ensure there is at least 1 data frame
    size_t nPix = m_pi_parameters.getWidth()*m_pi_parameters.getHeight();
    size_t numframes = end - start + 1;
    if (numframes > 0){
        //Verify data is of expected size for specified frame range
        if (frames.size() < nPix*(end+1)) return -2;
        else{
            //Ensure that file is open
            if (isFileOpen()){
                //Write frames to disk
                for (size_t k = nPix*start; k < nPix*(end+1); k++){
                    binary_write(frames[k]);
                }
                m_pi_parameters.setNumberOfFrames(m_pi_parameters.getNumberOfFrames() + numframes);
                return 0;
            }
            else return -1;
        }
    }
    else return -3;
}



//Will write the passed frame buffer to the data file (i.e., the number of specified frames is written to disk).
// This function takes a pointer to an 8-bit char type and is intended to allow for direct writing of the
// image buffer that is directlyl streamed from a camera device so that the buffer does not have to be copied
// prior to being written to disk.
//Returns 0 if data was successfully written to file.
//Returns -1 if the data file is not open for writing
//Returns -2 (can't test for this case with a pointer, have to take for granted it is of the right size)
//Returns -3 if numframes < 1
//NOTE: The expected size of each image frame is the width*height*bytes_per_pixel as specified for
//       the writer object at initialization. If this is not the case then the risk of over-running the vector
//       bounds is possible.
template <typename T> int CPIWriter::writeFrames(T* frames, size_t numframes){

	//Ensure there is at least 1 data frame
	if (numframes > 0){
		//Ensure that file is open
		if (isFileOpen()){
			//Write frames to disk
			for (size_t k = 0; k < m_pi_parameters.getHeight()*m_pi_parameters.getWidth()*numframes; k++){
				binary_write(frames[k]);
			}
			m_pi_parameters.setNumberOfFrames(m_pi_parameters.getNumberOfFrames() + numframes);
			return 0;
		}
		else return -1;
	}
	else return -3;
}
