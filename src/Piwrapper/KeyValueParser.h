#pragma once
#include <string>
#include <vector>

using namespace std;

//--------------------------------------------------
// Author: Bradley M. Ratliff, Ph.D.

//--------------------------------------------------------------------------------------------
//-----------KEY VALUE PAIR PARSER CLASS------------------------------------------------------
//--------------------------------------------------------------------------------------------

// This class is used to parse an input ASCII file that contains key-value pairs in the ENVI 
// header style. 

//Key-value pair structure
typedef struct KVPAIR{
	string key;
	string value;
} keyValuePair;

//This is a class that will read an ASCII File that is structured in a key-value format
//    [key = value] -- as in ENVI or PI file header format.
// Lines beginning with '//' are considered comment lines and are ignored.
class CKeyValueParser{
public:
	CKeyValueParser();
	CKeyValueParser(string& filename, bool ignoreFirstLine, bool caseSensitive);

	int parseFile(string& filename);
    int findKeyValue(const string& key, string& values);
	void getKeyValuePairs(vector<keyValuePair>& list);
	bool wasLastParseSuccessful(){ return m_parse_success; }
	void setIgnoreFirstLine(bool on){ m_ignore_first_line = on; }
	void setCaseSensitivity(bool on){ m_case_sensitive = on; }	
    bool getIgnoreFirstLine(){ return m_ignore_first_line; }
    bool getCaseSensitivity(){ return m_case_sensitive; }

private:
	bool m_parse_success;
	bool m_ignore_first_line;
	bool m_case_sensitive;
	vector<keyValuePair> kvpairs;
	void removeWhitespace(bool leading, string& str);
	void convertStringToLowerCase(string &str);
};
