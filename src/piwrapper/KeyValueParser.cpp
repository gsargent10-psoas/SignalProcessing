#pragma once
#include "KeyValueParser.h"
#include <vector>
#include <fstream>
#include <stdio.h>
#include <string.h>


//--------------------------------------------------------------------------------------------
//-----------KEY VALUE PAIR PARSER CLASS Implementation---------------------------------------
//--------------------------------------------------------------------------------------------

// This class is used to parse an input ASCII file that contains key-value pairs in the ENVI 
// header style. 


//Initializes class with empty data
CKeyValueParser::CKeyValueParser(){
	kvpairs.clear();
	m_parse_success = false;
	m_ignore_first_line = false;
	m_case_sensitive = false;
}

//Initializes class and attempts to parse the passed data file
CKeyValueParser::CKeyValueParser(string& filename, bool ignoreFirstLine, bool caseSensitive){
	m_parse_success = false;
	m_ignore_first_line = ignoreFirstLine;
	m_case_sensitive = caseSensitive;
	parseFile(filename);
}

//Searches the key-value structure and returns the string value of the key if it is found. If no parse
// has yet been performed the function returns -2. The function returns -1 if the parameter is not found, 
// or the index of the item in kvpairs otherwise.
//_strcmpi = windows
//strcasecmp = linux
int CKeyValueParser::findKeyValue(const string& key, string& value){
	if (m_parse_success){
		int found = -1;
        for (int k = 0; k < kvpairs.size(); k++){
			if (strcasecmp(key.c_str(), kvpairs[k].key.c_str()) == 0){
				value = kvpairs[k].value;
				found = k;
			}
		}
		return found;
	}
	else return -2;
}

//Copies the contents of kvpairs into list.
void CKeyValueParser::getKeyValuePairs(vector<keyValuePair>& list){
	list.clear();
	for (size_t k = 0; k<kvpairs.size(); k++){
		list.push_back(kvpairs[k]);
	}
}

//Clears any current data within the class and attempts to parse the passed file. Returns 0 if successful.
//Return Values: -1 = could not open file. -2 = file contains bad data. -3 = illegal character in key value.
//Lines beginning with "//" are considered comment lines and are ignored.
// The first line of the file can be ignored (such as in ENVI or PI files) although the first line of these files
// should be verified externally if necessary. The function can also be made to be case sensitive (otherwise read
// data is converted to lower case).
int CKeyValueParser::parseFile(string& filename){

	//Clear any current Key-Value Pairs
	kvpairs.clear();

	//Attempt to open input file and read all data
	ifstream input;
	input.open(filename);
	string line, fileData;
	if (!input.fail()){

		//Ignore first line of file if specified
		if (m_ignore_first_line) input >> line;

		while (!input.eof()){
			getline(input, line);
			if (line[0] == '/' && line[1] == '/') line.clear();
			if (!input.bad()){
				//convert characters to lower case if case sensitivity is turned off.
                convertStringToLowerCase(line);
				fileData = fileData + line + "\n";
			}
			else return -2;
		}
		input.close();
	}
	else return -1;

	//Parse key/value pairs and place into kvpairs string array
    size_t cnt = 0;
	while (cnt<fileData.length()){
		string key = "", value = "";
		bool haveKey = false, haveValue = false;

		//Extract key
		while (haveKey == false){
			if (cnt>fileData.length() - 1){
				haveKey = true;
			}
			else{
				if (fileData[cnt] == '=') haveKey = true;
				else key = key + fileData[cnt];
				cnt++;
			}
		}
		//Trim whitespace from key
		removeWhitespace(true, key);
		removeWhitespace(false, key);

		//Extract value
		bool readingValue = false;
		while (haveValue == false){
			if (cnt>fileData.length() - 1){
				haveValue = true;
			}
			else{
				bool tabspace = false, whitespace = false, other = false;
				if (fileData[cnt] == ' ') tabspace = true;
				else if (fileData[cnt] == '\t') tabspace = true;
				else if (fileData[cnt] == '\f') whitespace = true;
				else if (fileData[cnt] == '\v') whitespace = true;
				else if (fileData[cnt] == '\n') whitespace = true;
				else if (fileData[cnt] == '\r') whitespace = true;
				else other = true;
				cnt++;

				//Ignore leading or trailing whitespace
				if (whitespace && !readingValue) haveValue = true; //If Value Is Blank, i.e. ""
				else if ((whitespace || tabspace) && !readingValue);
				//Start of Single Value
				else if (other && !readingValue) { readingValue = true; value = value + fileData[cnt - 1]; }
				//End of Single Value
				else if (whitespace && readingValue) { readingValue = false; haveValue = true; }
				//Read Data Value
				else if (other && readingValue) { value = value + fileData[cnt - 1]; }
				//Read Tab or Space as Data Value
				else if (tabspace && readingValue) { value = value + fileData[cnt - 1]; }
				//Else Something Went Wrong -- Stop Reading
				else return -3;
			}
		}
		//Trim whitespace from value
		removeWhitespace(true, value);
		removeWhitespace(false, value);

		//Insert key/value into kvpairs
		keyValuePair temp;
		temp.key = key;
		temp.value = value;
		kvpairs.push_back(temp);
	}
	m_parse_success = true;
	return 0;
}

//Removes Leading/Trailing Whitespace From a string
void CKeyValueParser::removeWhitespace(bool leading, string& str){
	size_t found;
	if (leading) found = str.find_first_not_of(" \t\f\v\n\r");
	else found = str.find_last_not_of(" \t\f\v\n\r");

	if (found != string::npos){
		if (leading)	str.erase(0, found);
		else str.erase(found + 1);
	}
	else str.clear(); // str is all whitespace
}

//Converts any upper case letters to lower case letters within the passed character string
void CKeyValueParser::convertStringToLowerCase(string &str){
	for (size_t k = 0; k < str.length(); k++) if (isupper(str[k])) str[k] = tolower(str[k]);
}
