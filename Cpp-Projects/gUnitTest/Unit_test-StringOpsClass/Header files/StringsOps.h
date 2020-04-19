#pragma once
#include<iostream>
#include <vector>
#include<sstream>

using namespace std;

class StringsOps
{
public:
	StringsOps() {};
	~StringsOps() {};
	void AnalyseString(string const& Str, size_t & Chars, size_t & Digits, size_t & SpecificChars);
	void CrackUncPath(string const& Str, string & Hostname, string & ShareFolder, vector<string>& Path,
					  string & Filename, string & Extension);

	std::set<char> delims{ '\\' };

};


void StringsOps::AnalyseString(string const & Str, size_t & Chars, size_t & Digits, size_t & SpecificChars)
{
	 // init
	 Chars         = 0;
	 Digits		   = 0;
	 SpecificChars = 0;

	 if (Str.empty()) return;
	 
	 for (unsigned int i = 0; i < Str.length(); i++)
	 {
		 if ((Str[i] >= 'a' && Str[i] <= 'z') || (Str[i] >= 'A' && Str[i] <= 'Z')){
			 Chars++;
		 }
		 else if (Str[i] >= '0' && Str[i] <= '9') {
			 Digits++;
		 }
		 else {
			 SpecificChars++;
		 }
	 }

	 return;
}

 void StringsOps::CrackUncPath(string const & Str, string & Hostname, string & ShareFolder, vector<string>& Path, string & Filename, string & Extension)
 {
	 vector <string> result;
	 string temp;

	 Hostname = "";;
	 ShareFolder = "";
	 Filename = "";
	 Extension = "";

	 string File;
	 size_t pos;
	 if (Str.empty())
		 return;

	 // create tokens from the String depending on 'delims'

	 char const* pch = Str.c_str();
	 char const* start = pch;
	 for (; *pch; ++pch)
	 {
		 if (delims.find(*pch) != delims.end())
		 {
			 if (start != pch)
			 {
				 std::string str(start, pch);
				 result.push_back(str);
			 }
			 start = pch + 1;
		 }
	 }
	 result.push_back(start);
	 // extract the values from vector and save it


	 switch (result.size())
	 {

	 case 1:

		 File = result.back();
		 pos = File.find('.');
		 Filename = File.substr(0, pos);
		 Extension = File.substr(pos + 1);
		 return;

	 case 2:

		 Hostname = result[0];
		 File = result.back();
		 pos = File.find('.');
		 Filename = File.substr(0, pos);
		 Extension = File.substr(pos + 1);
		 return;

	 case 3:

		 Hostname = result[0];
		 ShareFolder = result[1];
		 File = result.back();
		 pos = File.find('.');
		 Filename = File.substr(0, pos);
		 Extension = File.substr(pos + 1);
		 return;
	 
	 }

	 Hostname = result[0];
	 ShareFolder = result[1];
	 File = result.back();
	 pos = File.find('.');
	 if (string::npos != pos) {
		 copy(result.begin() + 2, result.end() - 1, back_inserter(Path));
		 Filename = File.substr(0,pos);
		 Extension = File.substr(pos+1);

	 }
	 else {
		 copy(result.begin() + 2, result.end(), back_inserter(Path));
		 Filename = "";
		 Extension = "";
	 }
	 
	 return;
 }
