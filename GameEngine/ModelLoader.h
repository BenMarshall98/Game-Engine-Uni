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
	inline static bool EndsWith(string fileName, string fileType)
	{
		return (fileName.size() >= fileType.size() &&
			fileName.compare(fileName.size() - fileType.size(), fileType.size(), fileType) == 0);
	}

	static void SimpleFormatExists(string & fileName, string fileType);
	static int FindInVector(vector<string> & list, string toFind);
	static StaticModel * LoadOBJ(string fileName);
	static StaticModel * LoadSME(string fileName);
public:
	static iModel * LoadModel(string fileName);
};

