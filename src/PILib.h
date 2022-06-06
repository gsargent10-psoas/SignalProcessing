#pragma once
#include <vector>
using namespace std;


//--------------------------------------------------------------------------------------------
//-----------PI Library Parameter Class
//--------------------------------------------------------------------------------------------

//This is a class that holds the parameters for configuring and running the Polarimetric Imaging Library (Class PILib) processing chain.

class CPIParameters{
public:

	/*
	CSBParameters(string& filename);

	void OutputParameters(void);
	int VerifyBinningTable(string& binfile);
	bool ParseNumericArray(string& numstring, vector<double>& nums);
	bool IsValidDigit(string& D);

	//Parameter Get Methods
	int GetErrorCode(size_t codenum){ if (codenum<20) return error_code[codenum]; else return -1; }
	void GetInputFiles(vector<string>& files){ files.resize(inputfiles.size()); std::copy(inputfiles.begin(), inputfiles.end(), files.begin()); }
	void GetOutputFiles(vector<string>& files){ files.resize(outputfiles.size()); std::copy(outputfiles.begin(), outputfiles.end(), files.begin()); }
	bool AreParamsValid(){ return validParameters; }
	bool IsFramingData(){ return data_modality == "framing"; }
	bool IsLineScanData(){ return data_modality == "line_scan"; }
	size_t GetSampleCropLower(){ return sample_crop_lower; }
	size_t GetSampleCropUpper(){ return sample_crop_upper; }
	size_t GetLineCropLower(){ return line_crop_lower; }
	size_t GetLineCropUpper(){ return line_crop_upper; }
	size_t GetBandCropLower(){ return band_crop_lower; }
	size_t GetBandCropUpper(){ return band_crop_upper; }
	void GetBinningTable(string& table){ table = binning_table; }
	bool IsBinningEnabled(){ return binning_enabled; }
	bool IsBandPreservationOn(){ return preserve_bands; }
	bool IsSBNUCEnabled(){ return sbnuc_on; }
	bool IsRescalingOn(){ return rescaling_on; }
	void GetSBNUCMode(string& mode){ mode = sbnuc_mode; }
	size_t GetSBNUCBlockSize(){ return sbnuc_blocksize; }
	size_t GetGaussWidth(){ return gauss_width; }
	double GetOutlierPercentage(){ return outlier; }
	double GetFIRAlpha(){ return FIRalpha; }
	void GetGainRanges(vector<double>& ranges){ ranges.resize(gain_ranges.size()); std::copy(gain_ranges.begin(), gain_ranges.end(), ranges.begin()); }
	size_t GetNumberOfGainMaps(){ return ngainmaps; }
	bool IsInitialGainOn(){ return init_gain; }
	void GetGainMap(string& gain){ gain = gain_map; }
	bool IsDBPROn(){ return dbpr_on; }
	double GetGaussSdev(){ return gauss_sdev; }
	size_t GetDBPRBlockSize(){ return dbpr_blocksize; }
	size_t GetDBPRNumIterations(){ return dbpr_numiterations; }
	double GetDBPRMean(){ return dbpr_mean; }
	double GetDBPRSdev(){ return dbpr_sdev; }
	bool OutputDataOn(){ return output_data; }
	bool OutputGainMapsOn(){ return output_gainmaps; }
	bool OverwriteInputFiles(){ return overwrite_inputfiles; }
	*/

private:
	/*
	void RemoveWhitespace(bool leading, string& str);

	vector<string> inputfiles;
	vector<string> outputfiles;
	size_t sample_crop_lower, sample_crop_upper;
	size_t line_crop_lower, line_crop_upper;
	size_t band_crop_lower, band_crop_upper;
	string binning_table, data_modality, sbnuc_mode;
	bool binning_enabled, preserve_bands;
	bool sbnuc_on, rescaling_on;
	size_t sbnuc_blocksize, gauss_width;
	double gauss_sdev;
	string gain_map;
	bool dbpr_on, init_gain;
	size_t dbpr_blocksize, dbpr_numiterations;
	double dbpr_mean, dbpr_sdev;
	bool output_data, output_gainmaps, overwrite_inputfiles;
	string output_suffix;
	CKeyValueParser config;
	bool validParameters;
	int *error_code;
	double outlier, FIRalpha;
	vector<double> gain_ranges;
	size_t ngainmaps;
	*/
};




















//--------------------------------------------------------------------------------------------
//-----------PI Processing Library Class
//--------------------------------------------------------------------------------------------

//This is a class contains all processing and exploitation algorithms for PI data

template <typename T, typename A = std::allocator<T> > class CPILib
{
public:

	//--------------------------------------------
	// Methods


	CPILib<typename T, typename A>();
	~CPILib();

	// Single Frame Processing

	// Flat Field Procedure Methods

	//Get Methods

	//Set Methods





private:

	//-------------------------------------------
	// Private Varialbles




	//------------------------------------------
	// Private Methods

	//Calibration Methods

	//Scene-based Methods

	//Demodolation Methods

	//Stokes Estimation and PI Product Methods

	//Display Product Methods


};
