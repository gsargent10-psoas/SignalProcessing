#pragma once
#include <string>
//Defined polarimetric enumeration types and structures



//***** DEFINED CONSTANTS *******************************************************
//
//Constants used in radiometric calculations
const double PI = 3.14159265359; // Pi (unitless)
const double Kb = 1.38064852e-23; //Boltzmann's Constant (Joule / Kelvin)
const double C = 299792458; // Speed of Light (meters / sec)
const double H = 6.626070040e-34; // Plank's Constant (Joule * sec)
const double C1 = 2 * PI*H*C*C; // First radiation constant (Watts * meters^2)/sec^2
const double C2 = (H*C) / Kb; // Second radiation constant (meters * Kelvin)/sec^2



// PI Exception structure
// Structure for PI File exception handling.
struct PIException : public std::exception{
	std::string error;
	PIException(std::string theError) : error(theError) {}
	~PIException() throw() {} // the destructor must not throw
	const char* what() const throw (){
		return error.c_str();
	}
};


// ENVI Data Types defined here for use are:
//  1:  Byte: 8 - bit unsigned integer
//  2:  Integer : 16 - bit signed integer
//  3:  Long : 32 - bit signed integer
//  4:  Floating - point : 32 - bit single - precision
//  5:  Double - precision : 64 - bit double - precision floating - point
//  12: Unsigned integer : 16 - bit
//  13: Unsigned long integer : 32 - bit
//  14: 64 - bit long integer(signed)
//  15: 64 - bit unsigned long integer(unsigned)
//
enum ENVI_DATA_TYPE{
	UNKNOWN_DATA_TYPE = -1,
	ENVI_CHAR = 1,
	ENVI_SIGNED_SHORT = 2,
	ENVI_INTEGER = 3,
	ENVI_FLOAT = 4,
	ENVI_DOUBLE = 5,
	ENVI_UNSIGNED_SHORT = 12,
	ENVI_UNSIGNED_INT = 13,
	ENVI_I64 = 14,
	ENVI_UNSIGNED_I64 = 15
};


// Polarimetric Product Types
//  1:  Byte: 8 - bit unsigned integer
//  2:  Integer : 16 - bit signed integer
//  3:  Long : 32 - bit signed integer
//  4:  Floating - point : 32 - bit single - precision
//  5:  Double - precision : 64 - bit double - precision floating - point
//  12: Unsigned integer : 16 - bit
//  13: Unsigned long integer : 32 - bit
//  14: 64 - bit long integer(signed)
//  15: 64 - bit unsigned long integer(unsigned)
//
enum PI_PRODUCT_TYPE{
	UNKNOWN_PRODUCT_TYPE = -1,
	RAW = 100,
	S0 = 0,
	S1 = 1,
	S2 = 2,
	DOLP = 10,
	AOP = 11,
	TYOCOLOR = 20,
	FLATFIELD = 200,
	FLATFIELD_AVERAGE = 201,
	BAD_PIXEL_MAP = 301,
	INTENSITY = 3,
	STOKES = 4,
};


// Defined PI sensor types
//  0:	Division of Focal Plane (microgrid polarimeter).
//  1:	Division of Aperture.
enum PI_SENSOR_TYPE{
	UNKNOWN_SENSOR_TYPE = -1,
	DIVISION_OF_FOCAL_PLANE = 0,
        DIVISION_OF_TIME = 1,
        DIVISION_OF_AMPLITUDE = 2,
        DIVISION_OF_APERTURE = 3,
};


// Defined File Formats that Can be Read by CPReader
//  0: UDRI Polarimetric Image File.
//  1:	PIRATE 640x480 file as 16-bit unsigned integers. m_width = 640, m_height = 480, m_bpp = 2. PIRATE files
//		are raw files that have no headers.
//  2: Polaris Data Format:
enum PI_SENSOR_NAME {
	UNKNOWN_SENSOR_NAME = -1,
	PIRATE_I = 0,
	PIRATE_II = 1,
	TYO_LWIRuG = 10,
	PYXIS = 20,
	FLIR_MONO_UGRID = 31,
	FLIR_MONO_DOT = 32,
	FLIR_RGB_UGRID = 33,
};


// Defined File Formats that Can be Read by CPReader
//  0: UDRI Polarimetric Image File.
//  1:	PIRATE 640x480 file as 16-bit unsigned integers. m_width = 640, m_height = 480, m_bpp = 2. PIRATE files
//		are raw files that have no headers.
//  10: Polaris Data Format:
enum PI_FILE_FORMAT{
	UNKNOWN_FILE_TYPE = -1,
	PIFILE = 0,
	AFRL_PIRATE = 1,
	POLARIS_PYXIS = 10,
};


// Defined Radiometric Units used by CPICal class and within PI files
enum RADIOMETRIC_UNIT{
	UNKNOWN_RADIOMETRIC_UNIT = -1,
	COUNTS = 0,
	KELVIN = 1,
	CELCIUS = 2,
	FAHRENHEIT = 3,
	PHOTONS = 4,
	MICROFLICKS = 5,
};

// Defined wavelength units used by the CPICal and PI Files class
enum WAVELENGTH_UNIT{
	UNKNOWN_WAVELENGTH_UNIT = -1,
	METERS = 0,
	CENTIMETERS = 1,
	MILLIMETERS = 2,
	MICROMETERS = 3,
	NANOMETERS = 4,
	MICRONS = 5,
};

// Defined image scaling types
enum SCALING_TYPE{
	ABS = 0,
	STATISTICAL = 1,
};


// Defined demosaic types
enum DEMOSAIC_TYPE{
	NLPN = 0, //Nearest Like-Polarization Neighbor
	OLF = 1, //Optimal Linear Filtering
	LABF = 2, //Locally-adaptive Bilateral-Filter
};
