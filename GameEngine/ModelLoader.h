#pragma once
#include "iModel.h"
#include "StaticModel.h"

#include <vector>
#include<string>

using namespace std;

class ModelLoader
{
private:
	//The following code is adapted from:
	//https://thispointer.com/c-how-to-check-if-a-string-ends-with-an-another-given-string/
	inline static bool EndsWith(const string & fileName, const string & fileType)
	{
		return (fileName.size() >= fileType.size() &&
			fileName.compare(fileName.size() - fileType.size(), fileType.size(), fileType) == 0);
	}

	//static void SimpleFormatExists(const string & fileName, const string & fileType);
	static int FindInVector(vector<string> & list, const string & toFind);
	static StaticModel * LoadOBJ(const string & fileName);
	static StaticModel * LoadSME(const string & fileName);
	ModelLoader(void) {};
public:
	static iModel * LoadModel(const string & fileName);
};

