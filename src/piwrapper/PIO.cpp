#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "PIEnums.h"
#include "PIO.h"


/***************************************************************************************************************
Begin - CPIFile Class Methods
This class is used for maintaining all PI file parameters along with methods for reading and writing PI
file headers.
***************************************************************************************************************/

//**************************************************************************************
//CONSTRUCTORS

//Initialze unassigned CPIFile Object
CPIFile::CPIFile(){
	zeroOut();
}

//Create CPIFile object and initialze all parameters with read header file
CPIFile::CPIFile(std::string &filename){
	zeroOut();
	int state = readPIHeaderFile(filename);

	if (debug_mode){
		if (state < 1) cout << "Header file read state: " << state << "\n";
		else cout << "Header file read successfully." << "\n";
	}
}

//Zeros out all member variables.
void CPIFile::zeroOut(){
	//Required values
	m_width = 0;
	m_height = 0;
	m_frames = 0;
	m_bytes_per_pixel = 0;
	m_byte_order = false;
	m_data_type = UNKNOWN_DATA_TYPE;
	PI_PRODUCT_TYPE m_product_type = UNKNOWN_PRODUCT_TYPE;

	//Optional known values
	m_description = "";
	PI_SENSOR_TYPE m_sensor_type = UNKNOWN_SENSOR_TYPE;
	PI_SENSOR_NAME m_sensor_name = UNKNOWN_SENSOR_NAME;
    m_lens_focal_length = 0;
    m_lens_fnumber = 0;
    m_spectral_channels = 0;
    m_polarimetric_channels = 0;
    m_adc_bit_depth = 0;
    m_exposure_time = 0;
    m_gain = 0;
    m_gamma = 0;
    m_nframes_averaged = 0;
    m_timestamp = "";
    m_ugrid_superpixel_size.clear();
    m_polarizer_orientations.clear();
    m_polarizer_spectral_orientations = "";
	m_flatfield_unit = UNKNOWN_RADIOMETRIC_UNIT;
	m_flatfield_values.clear();
	bool m_flatfield_source_type = true;
	double m_flatfield_center_wavelength = 0.0;
	WAVELENGTH_UNIT m_flatfield_wavelength_unit = UNKNOWN_WAVELENGTH_UNIT;
    double m_blackbody_emissivity = 0.0;
    m_turntable_azimuth = -1000;
    m_solar_azimuth = -1000;
    m_solar_elevation = -1000;
    m_camera_azimuth = -1000;
    m_camera_elevation = -1000;

	//Boolean Flags
	m_valid_header = false;
	m_frames_valid = false;
	m_width_valid = false;
	m_height_valid = false;
	m_byte_order_valid = false;
	m_data_type_valid = false;
	m_product_type_valid = false;

	m_filename = "";
	m_header_filename = "";
	m_unknown_parameters.clear();
	m_swap_data = false;

	//Known parameter list
	//Note that when additional known parameters are added below that the following functions
	//must also be updated: addPIParameter, writePIHeaderFile(), corresponding get/set methods
	m_known_parameters.clear();
	m_known_parameters.push_back("width");
	m_known_parameters.push_back("height");
	m_known_parameters.push_back("frames");
	m_known_parameters.push_back("byte order");
	m_known_parameters.push_back("data type");
	m_known_parameters.push_back("product type");

	m_known_parameters.push_back("description");
	m_known_parameters.push_back("sensor type");
	m_known_parameters.push_back("sensor name");
    m_known_parameters.push_back("lens focal length");
    m_known_parameters.push_back("lens fnumber");
    m_known_parameters.push_back("pixel pitch");
    m_known_parameters.push_back("spectral channels");
    m_known_parameters.push_back("polarimetric channels");
    m_known_parameters.push_back("adc bit depth");
    m_known_parameters.push_back("exposure time");
    m_known_parameters.push_back("gain");
    m_known_parameters.push_back("gamma");
    m_known_parameters.push_back("number frames averaged");
    m_known_parameters.push_back("time stamp");
    m_known_parameters.push_back("microgrid superpixel size");
    m_known_parameters.push_back("polarizer orientations");
    m_known_parameters.push_back("polarizer spectral orientations");
	m_known_parameters.push_back("flatfield values");
	m_known_parameters.push_back("flatfield unit");
	m_known_parameters.push_back("flatfield source type");
	m_known_parameters.push_back("flatfield center wavelength");
	m_known_parameters.push_back("flatfield center wavelength unit");
	m_known_parameters.push_back("blackbody emissivity");
    m_known_parameters.push_back("camera azimuth");
    m_known_parameters.push_back("camera elevation");
    m_known_parameters.push_back("turntable azimuth");
    m_known_parameters.push_back("solar azimuth");
    m_known_parameters.push_back("solar elevation");

	debug_mode = false;
}



//**************************************************************************************
//HEADER I/O FUNCTIONS


/***************************************************************************************************************
GetENVIHeaderData function

This function takes a string filename. If this filename does not end in ".hdr", the function will attempt
to find a matching .hdr file for the specified file in the same directory.  The first line of the header file
must say "PIFILE".  This function will return the read header values in the inFile CPIFile object returned by value.
The CPIFile object contains variables for common PI File parameters. If one of the fields is not found in the PI
header, the value will be set to -1. This function also returns a vector of UNKNOWN_PARAMETER structures, which
contain a string key and a string value. Any parameters found in the PI file that are not known are stored in
this structure and returned by reference.

Return Type: int. This function will return 1 on success.
If -1 is returned, there was an error opening the ENVI header for the specified file.
If -2 is returned, the first line of the file was not "PIFILE" so the file is not parsed.
If -3 is returned, an unexpected error occurred while trying to parse the header file.
If -4 is returned, a PI header parameter has an invalid value. Ensure PI header file values are correct.
If -5 is returned, a required PI parameter is missing from the header file (width, height, frames, etc)
****************************************************************************************************************/
int CPIFile::readPIHeaderFile(std::string& filename){

	std::ifstream input;

	//Zero out all header file parameters
	zeroOut();

	//Ensure we have a proper header filename to attempt to open.
	std::string hdrfile;
	if (filename.substr(filename.length() - 4, 4) == ".hdr") hdrfile = filename;
	else hdrfile = filename + ".hdr";

	//Ensure data and header files exist before reading.
	if (!doDataAndHeaderFilesExist(hdrfile)) return -1;

	//Ensure first line of header file is "PIFILE"
	if (!verifyValidPIHeaderFile(hdrfile)) return -2;

	//Parse the header file
	std::vector<keyValuePair> kvpairs;
	CKeyValueParser parser(hdrfile, true, false);
	if (parser.wasLastParseSuccessful()) parser.getKeyValuePairs(kvpairs);
	else return -3;

	//Validate key-value pairs and add to PIFile object
	for (size_t k = 0; k < kvpairs.size(); k++){
		if (kvpairs[k].key.length()>0 && kvpairs[k].value.length() > 0){
			//cout << kvpairs[k].key << "--->" << kvpairs[k].value << "\n";
			if (!addPIParameter(kvpairs[k].key, kvpairs[k].value)) return -4;
		}
	}

	//Ensure That the Critical Header Parameters Were Initialized
	setInputHeaderValid(wereCriticalHeaderParametersSet());

	//If Header is Valid, set the swap_data flag and return success
	if (isInputHeaderValid()){
		if (debug_mode) cout << "Input header is valid.\n";
		if (((getByteOrder() == 1) && isMachineLittleEndian()) || ((getByteOrder() == 0) && !isMachineLittleEndian())) setSwapData(true);
		else setSwapData(false);
		return 1;
	}
	else{ if (debug_mode) cout << "Input header is not valid.\n"; return -5; }

}


/***************************************************************************************************************
This functions will write all information within the object out to a PI header file specified by m_header_filename.
This functions requires that areCriticalHeaderParametersValid() be true. If there are any problems the function
returns one of the negative integers described below. Otherwise, the function will return zero upon a successful
header file write.
-1, fileOut does not contain valid header information
-2, Could not create header file
****************************************************************************************************************/
int CPIFile::writePIHeaderFile(){

	//Check for Valid Output Header File Information
	if (areCriticalHeaderParametersSetForWriting()){

		std::string hdr = getHeaderFilename();
		//Attempt to Open Headerfile for Output
		ofstream fout(hdr.c_str());
		if (!fout) return -2;
		else{
			//Output "PIFILE" Header String
			fout << "PIFILE" << endl;

			//Write Out Description Field
			if (m_description != "") fout << "description = {" << getDescription().c_str() << "}" << endl;

            if (m_timestamp != "")  fout << "time stamp = {" << getTimeStamp().c_str() << "}" << endl;

			//BEGIN REQUIRED PARAMETERS ******************

            //Write Out pixel width
			fout << "width = " << getWidth() << endl;

            //Write Out pixel height
			fout << "height = " << getHeight() << endl;

            //Write Out number of frames
			fout << "frames = " << getNumberOfFrames() << endl;

			//Write Out ENVI Data Type
			switch (getDataType()){
			case ENVI_CHAR:
				fout << "data type = 1" << endl;
				break;
			case ENVI_SIGNED_SHORT:
				fout << "data type = 2" << endl;
				break;
			case ENVI_INTEGER:
				fout << "data type = 3" << endl;
				break;
			case ENVI_FLOAT:
				fout << "data type = 4" << endl;
				break;
			case ENVI_DOUBLE:
				fout << "data type = 5" << endl;
				break;
			case ENVI_UNSIGNED_SHORT:
				fout << "data type = 12" << endl;
				break;
			case ENVI_UNSIGNED_INT:
				fout << "data type = 13" << endl;
				break;
			case ENVI_I64:
				fout << "data type = 14" << endl;
				break;
			case ENVI_UNSIGNED_I64:
				fout << "data type = 15" << endl;
				break;
            case UNKNOWN_DATA_TYPE:
                fout << "data type = -1" << endl;
                break;
			}

			//Write Out Byte Order
			fout << "byte order = " << getByteOrder() << endl;

			//Write Out Product Type
			switch (getProductType()){
			case UNKNOWN_PRODUCT_TYPE:
				fout << "product type = Unknown Product Type" << endl;
				break;
            case INTENSITY:
                fout << "product type = intensity" << endl;
				break;
            case SPATIALLY_MODULATED_INTENSITY:
                fout << "product type = spatially modulated intensity" << endl;
                break;
			case S0:
				fout << "product type = s0" << endl;
				break;
			case S1:
				fout << "product type = s1" << endl;
				break;
			case S2:
				fout << "product type = s2" << endl;
				break;
            case LINEAR_STOKES:
                fout << "product type = linear stokes" << endl;
                break;
            case LINEAR_STOKES_DOLP_AOP:
                fout << "product type = linear stokes dolp aop" << endl;
                break;
			case DOLP:
				fout << "product type = DoLP" << endl;
				break;
			case AOP:
				fout << "product type = AoP" << endl;
				break;
            case PCM:
                fout << "product type = PCM" << endl;
                break;
			case TYOCOLOR:
				fout << "product type = TyoColor" << endl;
				break;
			case FLATFIELD:
				fout << "product type = Flatfield" << endl;
				break;
			case FLATFIELD_AVERAGE:
				fout << "product type = Flatfield Average" << endl;
				break;
			case BAD_PIXEL_MAP:
				fout << "product type = Bad Pixel Map" << endl;
				break;
			}

			//END REQUIRED PARAMETERS ******************

			if (getSensorType() != UNKNOWN_SENSOR_TYPE){
				//Write Out Sensor Type
				switch (getSensorType()){
				case DIVISION_OF_FOCAL_PLANE:
					fout << "sensor type = Division of Focal Plane" << endl;
					break;
				case DIVISION_OF_APERTURE:
					fout << "sensor type = Division of Aperture" << endl;
					break;
                case DIVISION_OF_TIME:
                    fout << "sensor type = Division of Time" << endl;
                    break;
                case DIVISION_OF_AMPLITUDE:
                    fout << "sensor type = Division of Amplitude" << endl;
                    break;
                case NON_POLARIMETRIC_SENSOR:
                    fout << "sensor type = Non-polarimetric Sensor" << endl;
                    break;
                case UNKNOWN_SENSOR_TYPE:
                    break;
                }
			}

			if (getSensorName() != UNKNOWN_SENSOR_NAME){
				//Write Out Sensor Name
				switch (getSensorName()){
				case PIRATE_I:
					fout << "sensor name = PIRATE I" << endl;
					break;
				case PIRATE_II:
					fout << "sensor name = PIRATE II" << endl;
					break;
				case TYO_LWIRuG:
					fout << "sensor name = UA LWIR Microgrid" << endl;
					break;
                case PYXIS_LWIR_MONO_UGRID:
                    fout << "sensor name = Pyxis LWIR Monochromatic microgrid" << endl;
					break;
                case CORVIS_LWIR_MONO_DOT:
                    fout << "sensor name = Corvis LWIR Monochromatic DoT" << endl;
                    break;
                case BLACKFLY_VISIBLE_MONO_UGRID:
                    fout << "sensor name = Blackfly Visible Monochromatic Microgrid" << endl;
					break;
                case BLACKFLY_VISIBLE_RGB_UGRID:
                    fout << "sensor name = Blackfly Visible RGB Microgrid" << endl;
					break;
                case BLACKFLY_VISIBLE_MONO_DOT:
                    fout << "sensor name = Blackfly Visible Monochromatic DoT" << endl;
					break;
                case UNKNOWN_SENSOR_NAME:
                    break;
				}
			}

            //Number of camera spectral channels
            if(getNumberSpectralChannels()>0){
                fout << "spectral channels = " << getNumberSpectralChannels() << endl;
            }

            //Number of camera polarimetric channels
            if(getNumberPolarimetricChannels()>0){
                fout << "polarimetric channels = " << getNumberPolarimetricChannels() << endl;
            }

            //ADC Bit Depth of Camera
            if(getADCBitDepth()>0){
                fout << "adc bit depth = " << getADCBitDepth() << endl;
            }

            //Exposure time in microseconds
            if(getExposureTime()>0){
                fout << "exposure time = " << getExposureTime() << endl;
            }

            //Camera gain
            if(getCameraGain()>0){
                fout << "gain = " << getCameraGain() << endl;
            }

            //Camera gamma value
            if(getCameraGamma()>0){
                fout << "gamma = " << getCameraGamma() << endl;
            }

            //Number of image frames averaged during acquisition
            if(getNumberFramesAveraged()>0){
                fout << "number frames averaged = " << getNumberFramesAveraged() << endl;
            }

            //Lens focal length
            if(getLensFocalLength()>0){
                fout << "lens focal length = " << getLensFocalLength() << endl;
            }

            //Lens aperture setting
            if(getLensFNumber()>0){
                fout << "lens fnumber = " << getLensFNumber() << endl;
            }

            //Camera pixel pitch
            if(getPixelPitch()>0){
                fout << "pixel pitch = " << getPixelPitch() << endl;
            }

            //Camera azimuth
            if(getCameraAzimuth()>-360 && getCameraAzimuth()<360){
                fout << "camera azimuth = " << getCameraAzimuth() << endl;
            }

            //Camera elevation
            if(getCameraElevation()>=-90 && getCameraElevation()<=90){
                fout << "camera elevation = " << getCameraElevation() << endl;
            }

            //Turntable azimuth
            if(getTurntableAzimuth()>-360 && getTurntableAzimuth()<360){
                fout << "turntable azimuth = " << getTurntableAzimuth() << endl;
            }

            //Solar azimuth
            if(getSolarAzimuth()>-360 && getSolarAzimuth()<360){
                fout << "solar azimuth = " << getSolarAzimuth() << endl;
            }

            //Solar elevation
            if(getSolarElevation()>=-90 && getSolarElevation()<=90){
                fout << "solar elevation = " << getSolarElevation() << endl;
            }

            //Write out super pixel size and orientations
            if (m_ugrid_superpixel_size.size() == 2)
                fout << "microgrid superpixel size = " << "{" << m_ugrid_superpixel_size[0] << ", " << m_ugrid_superpixel_size[1] << "}" << endl;

            //Write out polarizer orientations
            std::vector<double> dT;
            dT.clear(); getPolarizerOrientations(dT);
            if (dT.size() > 0){
                //Write out polarizer orientations
                if (dT.size() == 1) fout << "polarizer orientations = " << std::fixed << dT[0] << endl;
                else{
                    size_t commasperline = 5;
                    fout << "polarizer orientations = " << "{ ";
                    if (dT.size() > commasperline) fout << endl;
                    size_t ccnt = 0;
                    for (size_t k = 0; k < dT.size(); k++){
                        if (k == 0) fout << std::fixed << dT[k] << ",";
                        else if (k == dT.size() - 1) fout << "\t" << std::fixed << dT[k] << "}" << endl;
                        else fout << "\t" << std::fixed << dT[k] << ",";
                        ccnt++;
                        if (ccnt == commasperline && k < dT.size() - 1){ fout << endl; ccnt = 0; }
                    }
                }
            }

            //Write out polarizer spectral orientations
            if(getPolarizerSpectralOrientations() != "")
                fout << "polarizer spectral orientations = " << getPolarizerSpectralOrientations() << endl;

			//Write out only if file is a flat field calibration file
			if ((getProductType() == FLATFIELD) || (getProductType() == FLATFIELD_AVERAGE)){
				if (m_flatfield_unit != UNKNOWN_RADIOMETRIC_UNIT){

					//output radiometric value array
					std::vector<double> dT;
					dT.clear(); getFlatFieldValues(dT);
					//Write out superpixel orientations
					if (dT.size() == 1) fout << "flatfield values = " << std::fixed << dT[0] << endl;
					else if(dT.size() > 1 && dT.size()==m_frames){
						size_t commasperline = 5;
						fout << "flatfield values = {";
						if (dT.size() > commasperline) fout << endl;
						size_t ccnt = 0;
						for (size_t k = 0; k < dT.size(); k++){
							if (k == 0) fout << std::fixed << dT[k] << ",";
							else if (k == dT.size() - 1) fout << "\t" << std::fixed << dT[k] << "}" << endl;
							else fout << "\t" << std::fixed << dT[k] << ",";
							ccnt++;
							if (ccnt == commasperline && k < dT.size() - 1){ fout << endl; ccnt = 0; }
						}
					}

					//Output radiometric unit
					if (m_flatfield_unit == MICROFLICKS) fout << "flatfield unit = microflicks" << endl;
					else if (m_flatfield_unit == PHOTONS) fout << "flatfield unit = photons" << endl;
					else if (m_flatfield_unit == KELVIN) fout << "flatfield unit = Kelvin" << endl;
					else if (m_flatfield_unit == CELCIUS) fout << "flatfield unit = Celcius" << endl;
					else if (m_flatfield_unit == FAHRENHEIT) fout << "flatfield unit = Fahrenheit" << endl;
					else if (m_flatfield_unit == COUNTS) fout << "flatfield unit = counts" << endl;

					//Output source type
					if (m_flatfield_source_type == true){
						fout << "flatfield source type = emissive" << endl;
						fout << "blackbody emissivity = " << std::fixed << m_blackbody_emissivity << endl;
					}
					else fout << "flatfield source type = reflective" << endl;

					//Output flatfield center wavelength value
					fout << std::fixed << "flatfield center wavelength = " << m_flatfield_center_wavelength << endl;


					if (m_flatfield_wavelength_unit == METERS) fout << "flatfield center wavelength unit = meters" << endl;
					else if (m_flatfield_wavelength_unit == CENTIMETERS) fout << "flatfield center wavelength unit = centimeters" << endl;
					else if (m_flatfield_wavelength_unit == MILLIMETERS) fout << "flatfield center wavelength unit = millimeters" << endl;
					else if (m_flatfield_wavelength_unit == MICROMETERS || m_flatfield_wavelength_unit == MICRONS) fout << "flatfield center wavelength unit = microns" << endl;
					else if (m_flatfield_wavelength_unit == NANOMETERS) fout << "flatfield center wavelength unit = nanometers" << endl;
				}
			}

			//Write Out All Unknown Paramters
			std::vector<keyValuePair> up; getUnknownParameters(up);
			for (size_t k = 0; k < up.size(); k++){
				if (up[k].key == "file type") continue;
				else if (up[k].key == "") continue;
				else{
					bool array = false;
					std::string val = up[k].value;
					for (size_t q = 0; q < val.size(); q++) if (val[q] == ',') array = true;
					fout << up[k].key.c_str() << " = ";
					//If Output Value is an Array, Treat Specially
					if (!array) fout << up[k].value.c_str() << endl;
					else{
						size_t tcommas = 0;
						for (size_t m = 0; m < val.length(); m++) if (val[m] == ',') tcommas++;
						fout << "{";
						size_t commasperline = (int)floor((100.0*(double)tcommas) / (double)val.length());
						if (commasperline < tcommas) fout << endl;
						size_t commas = 0;
						for (size_t cnt = 0; cnt < val.length(); cnt++){
							fout << val.c_str()[cnt];
							if (val.c_str()[cnt] == ',') commas++;
							if (commas == commasperline){ fout << endl; commas = 0; }
						}
						fout << "}" << endl;
					}
				}
			}

			fout.close();
		}
	}
	else return -1;

	//Header successfully written
	return 0;
}






//**************************************************************************************
//SET FUNCTIONS


//Sets the data_type variable and also determines and sets the number of m_bytes_per_pixel.
//bytes_per_pixel will be zero upon an invalid data_type.
void CPIFile::setDataType(ENVI_DATA_TYPE type){
	m_data_type = type;
	switch (m_data_type){
	case ENVI_CHAR:				// char
		m_bytes_per_pixel = 1;
		m_data_type_valid = true;
		break;
	case ENVI_SIGNED_SHORT:		// signed short
		m_bytes_per_pixel = 2;
		m_data_type_valid = true;
		break;
	case ENVI_INTEGER:			// integer
		m_bytes_per_pixel = 4;
		m_data_type_valid = true;
		break;
	case ENVI_FLOAT:			// float
		m_bytes_per_pixel = 4;
		m_data_type_valid = true;
		break;
	case ENVI_DOUBLE:			// double
		m_bytes_per_pixel = 8;
		m_data_type_valid = true;
		break;
	case ENVI_UNSIGNED_SHORT:	// unsigned short
		m_bytes_per_pixel = 2;
		m_data_type_valid = true;
		break;
	case ENVI_UNSIGNED_INT:		// unsigned int
		m_bytes_per_pixel = 4;
		m_data_type_valid = true;
		break;
	case ENVI_I64:				// long long
		m_bytes_per_pixel = 8;
		m_data_type_valid = true;
		break;
	case ENVI_UNSIGNED_I64:		// unsigned long long
		m_bytes_per_pixel = 8;
		m_data_type_valid = true;
		break;
	default:
		m_bytes_per_pixel = 0;
		m_data_type_valid = false;
		break;
	}
}


//Sets the M x N size of a microgrid super-pixel
void CPIFile::setMicrogridSuperpixelSize(size_t numcols, size_t numrows){
    m_ugrid_superpixel_size.clear();
    m_ugrid_superpixel_size.push_back(numcols);
    m_ugrid_superpixel_size.push_back(numrows);
}

//Sets the M x N size of a microgrid super-pixel from a string array read in from a PI header file (expected 2-integer elements)
//If resulting parsed vector does not have a length of 2, superpixel size vector is cleared.
bool CPIFile::setMicrogridSuperpixelSize(std::string superpixelsize){
    if (superpixelsize == "") { m_ugrid_superpixel_size.clear(); return true; }
    if (parseUnknownParameterValueFieldToSizeT(superpixelsize, m_ugrid_superpixel_size)){
        if (m_ugrid_superpixel_size.size() == 2){
			return true;
		}
		else{
            m_ugrid_superpixel_size.clear();
			return false;
		}
	}
	else return false;
}

//Sets the polarizer orientations. No error checking is performed based upon sensor type since this is used
//by both DoT and DoFP sensors for different products in different ways. Thus, we trust that the user will
//know what they are doing when setting this.
bool CPIFile::setPolarizerOrientations(std::vector<double> &porientations){

    m_polarizer_orientations.clear();

    m_polarizer_orientations = porientations;
    m_polarimetric_channels = m_polarizer_orientations.size();
    return true;
}

//Sets an array of microgrid polarizer orientations from a string array read in from a PI header file.
//Returns true after passing "" to clear the orientations list, or if there is more than one element
//in the vector after parsing a non-zero length string.
bool CPIFile::setPolarizerOrientations(std::string orientations){
    if (orientations == "") { m_polarizer_orientations.clear(); return true; }
    if (parseUnknownParameterValueFieldToDouble(orientations, m_polarizer_orientations)){
        if (m_polarizer_orientations.size()>0) return true;
		else{
            m_polarizer_orientations.clear();
			return false;
		}
	}
	else return false;
}

//Sets an array of flat field values from a string array read in from a PI header file.
//Returns true after passing "" to clear the orientations list, or if there is more than one element
//in the vector after parsing a non-zero length string.
bool CPIFile::setFlatFieldValues(std::string ffvals){
	if (ffvals == "") { m_flatfield_values.clear(); return true; }
	if (parseUnknownParameterValueFieldToDouble(ffvals, m_flatfield_values)){
		if (m_flatfield_values.size()>0) return true;
		else{
			m_flatfield_values.clear();
			return false;
		}
	}
	else return false;
}



//Sets the output data and header files based upon the data file name passed. This function will ensure the
//file extension is a ".pi" file. If not, it will add a ".pi" extension to the filename, and then create
//a header file with a ".pi.hdr" extension to the filename.
void CPIFile::setOutputFilenames(std::string data_file){	

	if (data_file.length()>3) if (data_file.substr(data_file.length() - 3, 3) != ".pi") data_file = data_file + ".pi";
	setDataFilename(data_file);
	setHeaderFilename(data_file + ".hdr");
}

//Here, the key and value strings are simply added as a new node to the
//unknown_parameters vector. No parsing of any sort is done.
void CPIFile::appendUnknownParameter(std::string key, std::string value){
	//If key field already exists, delete existing node from unknown_parameters field
	if (findUnknownParameterIndex(key) != -1) deleteUnknownParameter(key);
	keyValuePair uP;
	uP.key = key;
	uP.value = value;
	m_unknown_parameters.push_back(uP);
}


//This function is used to set local member variables when passed a key-value string pair (typically read from a header file)
//NOTE: IT IS IMPORTANT THAT ALL KNOWN PARAMTERS ARE LOWER CASE!
bool CPIFile::addPIParameter(std::string key, std::string value){
	bool success = false;
	//If Parameter is in the known list
	if (isKnownParameter(key)){
        if (key == "description"){
            setDescription(value.substr(1,value.length()-2));
            success = true;
        }
        else if (key == "timestamp"){
            setTimeStamp(value);
            success = true;
        }
        else if (key == "width"){
			int num = atoi(value.c_str());
			if (num > 0) { setWidth(size_t(num)); success = true; m_width_valid = true; }
			else setWidth(0);
		}
		else if (key == "height"){
			int num = atoi(value.c_str());
			if (num > 0) { setHeight(size_t(num)); success = true; m_height_valid = true; }
			else setHeight(0);
		}
		else if (key == "frames"){
			int num = atoi(value.c_str());
			if (num > 0) { setNumberOfFrames(size_t(num)); success = true; m_frames_valid = true; }
			else setNumberOfFrames(0);
		}
		else if (key == "data type"){
			int num = atoi(value.c_str());
			if (num == 1)     { setDataType(ENVI_CHAR);	success = true; m_data_type_valid = true; }
			else if (num == 2){ setDataType(ENVI_SIGNED_SHORT); success = true; m_data_type_valid = true; }
			else if (num == 3){ setDataType(ENVI_INTEGER); success = true; m_data_type_valid = true; }
			else if (num == 4){ setDataType(ENVI_FLOAT); success = true; m_data_type_valid = true; }
			else if (num == 5){ setDataType(ENVI_DOUBLE); success = true; m_data_type_valid = true; }
			else if (num == 12){ setDataType(ENVI_UNSIGNED_SHORT); success = true; m_data_type_valid = true; }
			else if (num == 13){ setDataType(ENVI_UNSIGNED_INT); success = true; m_data_type_valid = true; }
			else if (num == 14){ setDataType(ENVI_I64); success = true; }
			else if (num == 15){ setDataType(ENVI_UNSIGNED_I64); success = true; m_data_type_valid = true; }
			else setDataType(UNKNOWN_DATA_TYPE);
		}
        else if (key == "byte order"){
            size_t num = atoi(value.c_str());
            if (num == 0) { setByteOrder(false); success = true; m_byte_order_valid = true; }
            else if (num == 1) { setByteOrder(true); success = true; m_byte_order_valid = true; }
            else setByteOrder(false);
        }
		else if (key == "product type"){
			if (value == "unknown product type")     { setProductType(UNKNOWN_PRODUCT_TYPE);	success = true; m_product_type_valid = true; }
            else if (value == "intensity"){ setProductType(INTENSITY); success = true; m_product_type_valid = true; }
            else if (value == "spatially modulated intensity"){ setProductType(SPATIALLY_MODULATED_INTENSITY); success = true; m_product_type_valid = true; }
			else if (value == "s0"){ setProductType(S0); success = true; m_product_type_valid = true; }
			else if (value == "s1"){ setProductType(S1); success = true; m_product_type_valid = true; }
			else if (value == "s2"){ setProductType(S2); success = true; m_product_type_valid = true; }
            else if (value == "linear stokes"){ setProductType(LINEAR_STOKES); success = true; m_product_type_valid = true; }
            else if (value == "linear stokes dolp aop"){ setProductType(LINEAR_STOKES_DOLP_AOP); success = true; m_product_type_valid = true; }
			else if (value == "dolp"){ setProductType(DOLP); success = true; m_product_type_valid = true; }
			else if (value == "aop"){ setProductType(AOP); success = true; m_product_type_valid = true; }
            else if (value == "pcm"){ setProductType(PCM); success = true; m_product_type_valid = true; }
			else if (value == "tyocolor"){ setProductType(TYOCOLOR); success = true; m_product_type_valid = true; }
			else if (value == "flatfield"){ setProductType(FLATFIELD); success = true; m_product_type_valid = true; }
			else if (value == "flatfield average"){ setProductType(FLATFIELD_AVERAGE); success = true; m_product_type_valid = true; }
			else if (value == "bad pixel map"){ setProductType(BAD_PIXEL_MAP); success = true; m_product_type_valid = true; }
			else setProductType(UNKNOWN_PRODUCT_TYPE);
		}
		else if (key == "sensor type"){
			int num = atoi(value.c_str());
			if (num == -1)    { setSensorType(UNKNOWN_SENSOR_TYPE);	success = true; }
			else if (num == 0){ setSensorType(DIVISION_OF_FOCAL_PLANE); success = true; }
            else if (num == 1){ setSensorType(DIVISION_OF_TIME); success = true; }
            else if (num == 2){ setSensorType(DIVISION_OF_AMPLITUDE); success = true; }
            else if (num == 3){ setSensorType(DIVISION_OF_APERTURE); success = true; }
            else if (num == 10){ setSensorType(NON_POLARIMETRIC_SENSOR); success = true; }
			else setSensorType(UNKNOWN_SENSOR_TYPE);
		}        
		else if (key == "sensor name"){
			int num = atoi(value.c_str());
			if (num == -1)     { setSensorName(UNKNOWN_SENSOR_NAME);	success = true; }
			else if (num == 0){ setSensorName(PIRATE_I); success = true; }
			else if (num == 1){ setSensorName(PIRATE_II); success = true; }
			else if (num == 10){ setSensorName(TYO_LWIRuG); success = true; }
            else if (num == 20){ setSensorName(PYXIS_LWIR_MONO_UGRID); success = true; }
            else if (num == 21){ setSensorName(CORVIS_LWIR_MONO_DOT); success = true; }
            else if (num == 31){ setSensorName(BLACKFLY_VISIBLE_MONO_UGRID); success = true; }
            else if (num == 32){ setSensorName(BLACKFLY_VISIBLE_MONO_DOT); success = true; }
            else if (num == 33){ setSensorName(BLACKFLY_VISIBLE_RGB_UGRID); success = true; }
			else setSensorName(UNKNOWN_SENSOR_NAME);
        }
        else if(key == "spectral channels"){
            int num = atoi(value.c_str());
            if (num > 0) { setNumberSpectralChannels(size_t(num)); success = true; }
            else setNumberSpectralChannels(0);
        }
        else if(key == "polarimetric channels"){
            int num = atoi(value.c_str());
            if (num > 0) { setNumberPolarimetricChannels(size_t(num)); success = true; }
            else setNumberPolarimetricChannels(0);
        }
        else if(key == "adc bit depth"){
            int num = atoi(value.c_str());
            if (num > 0) { setADCBitDepth(size_t(num)); success = true; }
            else setADCBitDepth(0);
        }
        else if(key == "exposure time"){
            long num = atoi(value.c_str());
            if (num > 0) { setExposureTime(long(num)); success = true; }
            else setExposureTime(0);
        }
        else if(key == "gain"){
            double num = atof(value.c_str());
            if (num > 0) { setCameraGain(double(num)); success = true; }
            else setCameraGain(0);
        }
        else if(key == "gamma"){
            double num = atof(value.c_str());
            if (num > 0) { setCameraGamma(double(num)); success = true; }
            else setCameraGamma(0);
        }
        else if(key == "number frames averaged"){
            size_t num = atoi(value.c_str());
            if (num > 0) { setNumberFramesAveraged(size_t(num)); success = true; }
            else setNumberFramesAveraged(0);
        }
        else if(key == "lens focal length"){
            double num = atof(value.c_str());
            if (num > 0) { setLensFocalLength(double(num)); success = true; }
            else setLensFocalLength(0);
        }
        else if(key == "lens fnumber"){
            double num = atof(value.c_str());
            if (num > 0) { setLensFNumber(double(num)); success = true; }
            else setLensFNumber(0);
        }
        else if(key == "pixel pitch"){
            double num = atof(value.c_str());
            if (num > 0) { setPixelPitch(double(num)); success = true; }
            else setPixelPitch(0);
        }
        else if (key == "camera azimuth"){
            double num = atof(value.c_str());
            setCameraAzimuth(num);
            success = true;
        }
        else if (key == "camera elevation"){
            double num = atof(value.c_str());
            setCameraElevation(num);
            success = true;
        }
        else if (key == "turntable azimuth"){
            double num = atof(value.c_str());
            setTurntableAzimuth(num);
            success = true;
        }
        else if (key == "solar azimuth"){
            double num = atof(value.c_str());
            setSolarAzimuth(num);
            success = true;
        }
        else if (key == "solar elevation"){
            double num = atof(value.c_str());
            setSolarElevation(num);
            success = true;
        }
        else if (key == "superpixel size"){
            setMicrogridSuperpixelSize(value.c_str());
            success = true;
        }
        else if (key == "polarizer orientations"){
            setPolarizerOrientations(value.c_str());
            success = true;
        }
        else if (key == "polarizer spectral orientations"){
            setPolarizerSpectralOrientations(value.c_str());
            success = true;
        }
		else if (key == "flatfield values"){
			setFlatFieldValues(value.c_str());
			success = true;
		}
		else if (key == "flatfield unit"){
			if (value == "unknown radiometric unit") { setFlatFieldUnit(UNKNOWN_RADIOMETRIC_UNIT);	success = true; }
			else if (value == "counts"){ setFlatFieldUnit(COUNTS); success = true; }
			else if (value == "kelvin"){ setFlatFieldUnit(KELVIN); success = true; }
			else if (value == "celcius"){ setFlatFieldUnit(CELCIUS); success = true; }
			else if (value == "fahrenheit"){ setFlatFieldUnit(FAHRENHEIT); success = true; }
			else if (value == "photons"){ setFlatFieldUnit(PHOTONS); success = true; }
			else if (value == "microflicks"){ setFlatFieldUnit(MICROFLICKS); success = true; }
			else setFlatFieldUnit(UNKNOWN_RADIOMETRIC_UNIT);
		}
		else if (key == "flatfield source type"){
			if (value == "emissive") m_flatfield_source_type = true;
			else m_flatfield_source_type = false;
			success = true;
		}
		else if (key == "blackbody emissivity"){
			double num = atof(value.c_str());
			m_blackbody_emissivity = num;
			success = true;
		}
		else if (key == "flatfield center wavelength"){
			double num = atof(value.c_str());
			m_flatfield_center_wavelength = num;
			success = true;
		}
		else if (key == "flatfield center wavelength unit"){
			if (value == "unknown wavelength unit") { m_flatfield_wavelength_unit = UNKNOWN_WAVELENGTH_UNIT;	success = true; }
			else if (value == "meters"){ m_flatfield_wavelength_unit = METERS; success = true; }
			else if (value == "m"){ m_flatfield_wavelength_unit = METERS; success = true; }
			else if (value == "centimeters"){ m_flatfield_wavelength_unit = CENTIMETERS; success = true; }
			else if (value == "cm"){ m_flatfield_wavelength_unit = CENTIMETERS; success = true; }
			else if (value == "millimeters"){ m_flatfield_wavelength_unit = MILLIMETERS; success = true; }
			else if (value == "mm"){ m_flatfield_wavelength_unit = MILLIMETERS; success = true; }
			else if (value == "microns"){ m_flatfield_wavelength_unit = MICRONS; success = true; }
			else if (value == "micrometers"){ m_flatfield_wavelength_unit = MICRONS; success = true; }
			else if (value == "um"){ m_flatfield_wavelength_unit = MICRONS; success = true; }
			else if (value == "nanometers"){ m_flatfield_wavelength_unit = NANOMETERS; success = true; }
			else if (value == "nm"){ m_flatfield_wavelength_unit = NANOMETERS; success = true; }
			else m_flatfield_wavelength_unit = UNKNOWN_WAVELENGTH_UNIT;
		}
		else{ appendUnknownParameter(key, value); success = true; }
	}
	else { appendUnknownParameter(key, value); success = true; }

	return success;
}

//**************************************************************************************
//GET FUNCTIONS

//Sets passed parameters to file size parameters.
void CPIFile::getFileDimensions(size_t& width, size_t& height, size_t& numframes) const{
	width = m_width;
	height = m_height;
	numframes = m_frames;
}

//Sets passed parameters to file size parameters including the number of bytes per pixel
void CPIFile::getFileDimensions(size_t& width, size_t& height, size_t& numframes, size_t& numbbp) const{
	width = m_width;
	height = m_height;
	numframes = m_frames;
	numbbp = m_bytes_per_pixel;
}



//**************************************************************************************
//UTILITY FUNCTIONS

//Searches the unknown parameter structure and returns the index of the specified parameter. The function
//returns -1 if the parameter is not found.
//strcasecmp=linux
//_strcmpi=windows
int CPIFile::findUnknownParameterIndex(std::string key){
	int index = -1;
	for (size_t i = 0; i<m_unknown_parameters.size(); i++){
		if (strcmp(key.c_str(), m_unknown_parameters[i].key.c_str()) == 0){
			index = (int)i;
		}
	}
	return index;
}

//Takes a value field string (presumably an array) and parses the values within and pushes each parsed string onto
//the passed string vector array.
bool CPIFile::parseUnknownParameterValueFieldToString(std::string valueString, std::vector<std::string> &values){
	values.clear();
	//Parse ENVI Parameter Array and Push Each Parsed String onto values string vector
	std::string num; bool buildingValue; int cnt = 0;
	if (valueString.substr(cnt,1) == "{") cnt = 1;
	while ((size_t)cnt<valueString.length()){
		//Parse Each Value in the String
		num = ""; buildingValue = true;
		while (buildingValue){
			if ((size_t)cnt>valueString.length() - 1 || valueString.substr(cnt, 1) == "," || valueString.substr(cnt, 1) == "}") buildingValue = false;
			else if (valueString.substr(cnt, 1) == " " || valueString.substr(cnt, 1) == "\t");
			else num = num + valueString.substr(cnt, 1);
			cnt++;
		}
		//Push Parsed Value onto values vector
		if (num.length()>0) values.push_back(num);
	}
	if (values.size()<1) return false;
	else return true;
}

//Takes a value field string (presumably an array) and parses out each string value. Each string is then converted to
//a double and pushed upon the values vector. The function returns true upon successful conversion, false otherwise.
bool CPIFile::parseUnknownParameterValueFieldToDouble(std::string valuestring, std::vector<double> &values){
	values.clear();
	std::vector<std::string> stringvalues;
	if (parseUnknownParameterValueFieldToString(valuestring, stringvalues) == true){
		for (int k = 0; (size_t)k<stringvalues.size(); k++){
			values.push_back(atof(stringvalues[k].c_str()));
		}
		return true;
	}
	else{ return false; }
}

//Parses the passed ENVI value field (braces already removed) and converts the parsed values to int
bool CPIFile::parseUnknownParameterValueFieldToInt(std::string valuestring, std::vector<int> &values){
	values.clear();
	std::vector<std::string> stringvalues;
	if (parseUnknownParameterValueFieldToString(valuestring, stringvalues) == true){
		for (int k = 0; (size_t)k<stringvalues.size(); k++){
			values.push_back(atoi(stringvalues[k].c_str()));
		}
		return true;
	}
	else{ return false; }
}

//Parses the passed PI value field (braces already removed) and converts the parsed values to size_t
//If a negative integer is found the values array is cleared and the function returns false.
bool CPIFile::parseUnknownParameterValueFieldToSizeT(std::string valuestring, std::vector<size_t> &values){
	values.clear();
	std::vector<std::string> stringvalues;
	if (parseUnknownParameterValueFieldToString(valuestring, stringvalues) == true){
		for (int k = 0; (size_t)k<stringvalues.size(); k++){
			int temp = atoi(stringvalues[k].c_str());
			if (temp >= 0) values.push_back(temp);
			else { values.clear(); return false; }
		}
		return true;
	}
	else{ return false; }
}


//Parses the passed ENVI value field (braces already removed) and converts the parsed values to boolean
bool CPIFile::parseUnknownParameterValueFieldToBool(std::string valuestring, std::vector<bool> &values){
	values.clear();
	std::vector<std::string> stringvalues;
	if (parseUnknownParameterValueFieldToString(valuestring, stringvalues) == true){
		for (int k = 0; (size_t)k<stringvalues.size(); k++){
			int value = atoi(stringvalues[k].c_str());
			if (value <= 0) values.push_back(false);
			else values.push_back(true);
		}
		return true;
	}
	else{ return false; }
}

//Will delete the specified key-value node from the unknown_parameter list if found
bool CPIFile::deleteUnknownParameter(std::string key){
	int index = findUnknownParameterIndex(key);
	if (index == -1) return false;
	else{
		//Create New unknown_paramter list, but leave selected node out of new vector.
		std::vector<keyValuePair> newUP;
		for (int k = 0; (size_t)k<m_unknown_parameters.size(); k++){
			if (k == index) continue;
			else newUP.push_back(m_unknown_parameters[k]);
		}
		m_unknown_parameters = newUP;
		return true;
	}
}

//Searches the known_parameter list for specified parameter. If found returns true.
bool CPIFile::isKnownParameter(std::string key){
	for (int k = 0; (size_t)k<m_known_parameters.size(); k++) if (key == m_known_parameters[k]) return true;
	return false;
}



/***************************************************************************************************************
This function takes a passed filename (either data or header file). The will then determine if the data and
header file pair exists. This returns true if BOTH files already exist.
NOTE: Use fileExists(...) to test whether a specific filename exists.
****************************************************************************************************************/
bool CPIFile::doDataAndHeaderFilesExist(std::string &inFile){
	//Extract Data Filename
	std::string dfile;
	if (inFile.substr(inFile.length() - 4, 4) == ".hdr") dfile = inFile.substr(0, inFile.length() - 4);
	else dfile = inFile;

	//Check for Data File and Header File Existance
	string check = dfile+".hdr";
	if (fileExists(dfile) && fileExists(check)) return true;
	//Otherwise one or both files not found
	else return false;
}

/***************************************************************************************************************
This function verifies that the set data and header filenames are properly set, i.e., data filename is set to
a "filename.pi" and the header filename is set to "filename.pi.hdr"
****************************************************************************************************************/
bool CPIFile::verifyOutputFilenames(){

	//Data filename needs to be a ".pi" file
	if (m_filename.length()<5) return false;
	if (m_filename.substr(m_filename.length() - 3, 3) != ".pi") return false;
	if (m_header_filename != m_filename + ".hdr") return false;
	return true;
}



/***************************************************************************************************************
This function attempts to open the passed header filename and checks whether the first line of the file is "PIFILE".
****************************************************************************************************************/
bool CPIFile::verifyValidPIHeaderFile(std::string &hdrfile){

	//Open header file for reading
	std::ifstream input;
	input.open(hdrfile.c_str());
	if (input.fail()) return false;

	//Check to make sure the first line of the file is "PIFILE". If not, return false.
	std::string line;
	input >> line;
	if (line != "PIFILE"){ input.close(); return false; }
	else return true;
}

/***************************************************************************************************************
Checks whether all required header parameters were read (or set).
****************************************************************************************************************/
bool CPIFile::wereCriticalHeaderParametersSet(){
	if (m_frames_valid && m_width_valid && m_height_valid && m_data_type_valid && m_product_type_valid && m_byte_order_valid)
		return true;
	else return false;
}

/***************************************************************************************************************
Checks whether all required header parameters have been set for writing. Basically, this calls
wereCriticalHeaderParametersSet() to check all critical parameters and also ensure output filenames are valid.
****************************************************************************************************************/
bool CPIFile::areCriticalHeaderParametersSetForWriting(){
	if (wereCriticalHeaderParametersSet() && verifyOutputFilenames())
		return true;
	else return false;
}

/***************************************************************************************************************
Simply checks whether the passed string filename exists.
****************************************************************************************************************/
bool CPIFile::fileExists(std::string &filename){
	if (filename != ""){
		std::ifstream finput;
		finput.open(filename.c_str(), std::ios::binary);
		if (finput.fail()){ return false; }
		else { finput.close(); return true; }
	}
	else return false;
}

/**************************************************************************************************************************************
This function is used to determine machine endianness. Based on the byte order in the PI header, the function can determine if
the data needs to be byte swapped. The function return true if the machine is Little Endian and false if the machine is Big Endian.
***************************************************************************************************************************************/
bool CPIFile::isMachineLittleEndian()
{
	int i = 1;
	char *p = (char *)&i;
	if (p[0] == 1) // Lowest address contains the least significant byte
		return true;  //Little Endian
	else
		return false; //Big Endian
}


/***************************************************************************************************************
End - CPIFile Class Methods
**************************************************************************************************************/











/***************************************************************************************************************
Begin - CPReader Class Implementation: 
The following functions are for the CPReader class, which is used to read images from binary polarimetric data 
files. Frames are currently only read out as doubles.

TASKS TO BE COMPLETED:

     -Add info to read and extract information from PYXIS file headers and frame header (currently file and frame header information is ignored)
     -Add ability to read big endian format.
***************************************************************************************************************/


//Create empty class with all variables initialized to null.
CPReader::CPReader(){ 
	zeroOut(); 
}


//Instantiate class and open passed filename for reading (file type is implied by extension)
CPReader::CPReader(string& filename){
	PI_FILE_FORMAT fileformat = getFileFormat(filename);
	openFile(filename, fileformat);
}


//Instantiate class and open passed filename for reading (file type is specified)
CPReader::CPReader(string& filename, PI_FILE_FORMAT fileformat){
	//Attempt to open file.
	openFile(filename, fileformat);
}


//Decstructor
CPReader::~CPReader(){
	if (m_fileopen)	m_fin.close();
}


//Get Methods

//Returns the file format type based upon the extension of the specified filename
PI_FILE_FORMAT CPReader::getFileFormat(std::string &filename){
	if (filename.substr(filename.length() - 3, 3) == ".pi") return PIFILE;
	else if (filename.substr(filename.length() - 4, 4) == ".raw") return AFRL_PIRATE;
	else if (filename.substr(filename.length() - 4, 4) == ".raw") return POLARIS_PYXIS;
	else return UNKNOWN_FILE_TYPE;
}

//Initializes all member variables to empty default values
void CPReader::zeroOut(){
	m_width = 0;
	m_height = 0;
	m_numframes = 0;
	m_bpp = 0;
	m_bpf = 0;
	m_header_bytes = -1;
	m_frameheader_bytes = -1;
	m_datatype = UNKNOWN_DATA_TYPE; //Follows ENVI File Format
	m_byte_order = false;   //0 is Little Endian. 1 is Big Endian.
	m_currentframe = -1;
	m_filename = "";
	m_fileopen = false;
	m_dimensions_set = false;
	m_numframes_set = false;
	m_fileformat = UNKNOWN_FILE_TYPE;
	m_validframe = false;
	m_fin.clear();
	buffer.clear();

	debug_mode = false;
}


//Opens the specified file and sets all file-related parameters
bool CPReader::openFile(string& filename, PI_FILE_FORMAT filetype){
	zeroOut();

	bool pifile = false;
	if (filetype == PIFILE) pifile = true;

	//Open Input File
	m_filename = filename;	
	m_fin.open(filename, std::ios::binary);
	if (m_fin.fail()) { m_filename = ""; return false; } //file open failed

	//If PI file
	if (pifile){
		//chech header exists
		ifstream hfile;
		hfile.open(filename + ".hdr", std::ios::binary);
		if (hfile.fail()) { m_filename = ""; return false; }
		else{
			m_fileopen = true;
			m_fileformat = filetype;
			m_currentframe = 0;
			setFileTypeDimensions();
			//calculateNumFrames();
			if (areDimensionsSet() == true) return true;
			else return false;
		}
	}
	//Otherwise non-PI file opened successfully
	else{ 
		m_fileopen = true;
		m_fileformat = filetype;
		m_currentframe = 0;
		setFileTypeDimensions();
		calculateNumFrames();
		if (areDimensionsSet() == true) return true;
		else return false;
	}
}


//Sets image dimensions based upon data type
bool CPReader::setFileTypeDimensions(){
	if (m_fileopen){
		if (m_fileformat == UNKNOWN_FILE_TYPE) return false;
		else if (m_fileformat == PIFILE) setPIFILEDimensions();
		else if (m_fileformat == AFRL_PIRATE) setPIRATEDimensions();
		else if (m_fileformat == POLARIS_PYXIS) setPYXISDimensions();
		else{ //Invalid filetype specified
			zeroOut();
			return false;
		}
	}
	buffer.resize(m_bpf);
	return true;
}


//Set parameters for PIFILE data file (it gets parameters from associated PIFILE header file
void CPReader::setPIFILEDimensions(){
	//Attempt to Read PIFile Header
	m_pifile.readPIHeaderFile(m_filename);
	if (m_pifile.isInputHeaderValid()){
		m_header_bytes = 0;
		m_frameheader_bytes = 0;
		m_width = m_pifile.getWidth();
		m_height = m_pifile.getHeight();
		m_numframes = m_pifile.getNumberOfFrames();
		m_numframes_set = true;
		m_bpp = m_pifile.getNumberOfBytesPerPixel();
		m_datatype = m_pifile.getDataType();
		m_byte_order = m_pifile.getByteOrder();
		m_bpf = m_pifile.getNumberOfBytesPerFrame();
		m_dimensions_set = true;
	}
}

//Set parameters for PIRATE data
void CPReader::setPIRATEDimensions(){
	m_header_bytes = 0;
	m_frameheader_bytes = 0;
	m_width = 640;
	m_height = 480;
	m_bpp = 2;
	m_datatype = ENVI_UNSIGNED_SHORT;
	m_byte_order = false;
	m_bpf = m_bpp*m_width*m_height + m_frameheader_bytes;	
	m_dimensions_set = true;
}


//Set parameters for POLARIS PYXIS data
void CPReader::setPYXISDimensions(){
	m_header_bytes = 2048;
	m_frameheader_bytes = 26;
	m_width = 640;
	m_height = 512;
	m_bpp = 4;
	m_datatype = ENVI_FLOAT;
	m_byte_order = false;
	m_bpf = m_bpp*m_width*m_height + m_frameheader_bytes;
	m_dimensions_set = true;
}


// Calculates the number of frames in the file based upon image dimensions
void CPReader::calculateNumFrames(){
	//Calculate frames if file is valid
	if (m_fileopen & m_dimensions_set){
		m_fin.seekg(0, m_fin.end);
		long long int totalbytes = m_fin.tellg();
		m_fin.seekg(0, m_fin.beg);
		if (totalbytes != -1){
			m_numframes = (size_t)((totalbytes - (long long int) m_header_bytes) / (long long int)m_bpf);
			m_numframes_set = true;
		}
		else{
			m_numframes = -1;
			m_numframes_set = false;
		}
	}
	else{
		m_numframes = -1;
		m_numframes_set = false;
	}
}


void CPReader::setBytesPerPixel(ENVI_DATA_TYPE dt){
	switch (dt){
	case ENVI_CHAR:	// char
		m_bpp = 1;
		break;
	case ENVI_SIGNED_SHORT:	// signed short
		m_bpp = 2;
		break;
	case ENVI_INTEGER:	// integer
		m_bpp = 4;
		break;
	case ENVI_FLOAT:	// float
		m_bpp = 4;
		break;
	case ENVI_DOUBLE:	// double
		m_bpp = 8;
		break;
	case ENVI_UNSIGNED_SHORT:	// unsigned short
		m_bpp = 2;
		break;
	case ENVI_UNSIGNED_INT:	// unsigned int
		m_bpp = 4;
		break;
	case ENVI_I64:	// long long
		m_bpp = 8;
		break;
	case ENVI_UNSIGNED_I64:	// unsigned long long
		m_bpp = 8;
		break;
	default:
		break;
	}
}



/***************************************************************************************************************
End - CPReader Class Methods
**************************************************************************************************************/








/***************************************************************************************************************
Begin - CPWriter Class Methods
This class is used for maintaining all PI file parameters along with methods for reading and writing PI
file headers.
***************************************************************************************************************/


//*********************************
// Constructors and Destructors

//Instantiates an empty CPWriter object with all variables zeroed out.
CPIWriter::CPIWriter(){
	zeroOut();
}

//Instantiate class with minimum required parameters (except number of frames) and open file for writing.
CPIWriter::CPIWriter(string& filename, size_t width, size_t height, ENVI_DATA_TYPE data_type, bool byte_order, PI_PRODUCT_TYPE product_type, bool overwrite){
	openFile(filename, width, height, data_type, byte_order, product_type, overwrite);
}

//Class Destructor
CPIWriter::~CPIWriter(){
	if (m_fileopen) closeFile();
}



//************************************************************************
//Sets all member variables to default uninitialed state
void CPIWriter::zeroOut(){
	m_pi_parameters.zeroOut();
	m_fout.clear();
	m_fileopen = false;	

	debug_mode = false;
}

/*

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
int CPIWriter::writeFramesAsChar(char* frames, size_t numframes){

	//Ensure there is at least 1 data frame
	if (numframes > 0){
		//Ensure that file is open
		if (isFileOpen()){
			//Write frames to disk
			for (size_t k = 0; k < m_pi_parameters.getNumberOfBytesPerFrame()*numframes; k++){
				binary_write(frames[k]);
			}
			m_pi_parameters.setNumberOfFrames(m_pi_parameters.getNumberOfFrames() + numframes);
			return 0;
		}
		else return -1;
	}
	else return -3;
}
*/


//**************************************
//Public methods for file I/O

// Opens PI file for writing. All required minimum parameters are required except for the number of frames. The number of frames is
// determined based upon how many frames are written to disk while the file is open. The overwrite flag specifies whether to overwrite an existing file 
// if one exists (header is not written until closeFile() is called.
// 0 is returned if file is successfully open and required header parameters are set.
// -1 is returned if either the data file or header file already exist.
// -2 is returned if the file failed to open.
int CPIWriter::openFile(string& filename, size_t width, size_t height, ENVI_DATA_TYPE data_type, bool byte_order, PI_PRODUCT_TYPE product_type, bool overwrite){

	//Clear local variables
	zeroOut();

	//Check if file exists and if so and overwrite flag is false then return false
	if (!overwrite & m_pi_parameters.doDataAndHeaderFilesExist(filename)) return -1;
	//Either overwrite files or they don't exist if we don't want to overwrite
	else{ 		

		//Open Output Data File
		m_fout.open(filename, std::ios::binary);

		//File failed to open..
		if (m_fout.fail()) return -2; 
		//If data file opens successfully..
		else{

			if (debug_mode) cout << "Output file opened successfully.\n";

			//Set all required parameters and zero out frames
			m_pi_parameters.setOutputFilenames(filename);
			if (debug_mode) cout << "PI File parameters were set successfully.\n";
			m_pi_parameters.setWidth(width);
			m_pi_parameters.setHeight(height);
			m_pi_parameters.setDataType(data_type);
			m_pi_parameters.setByteOrder(byte_order);
			m_pi_parameters.setProductType(product_type);
			m_pi_parameters.setNumberOfFrames(0);
			m_fileopen = true;			

			return 0;
		}
	}
}



//Will close the current data file and write out the corresponding header. If no frames have been written to the data file
//no header will be written and the empty data file will be removed from disk.
// 0 is returned if the data file was closed and the header file was successfully written to disk
// -1 is returned if no data file is currently open
// -2 is returned if no data frames were written to the file
// -3 is returned if required header parameters were missing
// -4 is returned if there was an error writing the header file to disk.
int CPIWriter::closeFile(){

	if (isFileOpen()){
		if (m_pi_parameters.getNumberOfFrames() > 0){

			//Close data file
			m_fout.close();
			m_fileopen = false;

			//write header file to disk
			int state = m_pi_parameters.writePIHeaderFile();
			if (state == -1) return -3;
			else if (state == -2) return -4;
			else return 0;			
		}
		else return -2;
	}
	else return -1;
}





//Will create a .pi header file for the specified PIRATE raw file and change the data file extension to .pi
bool CPIWriter::convertPirateFileToPIFile(string& filename){
	return false;
}
