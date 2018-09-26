#pragma once

#include "iModel.h"

#include <map>
#include <string>

using namespace std;

class ResourceManager
{
	private:
		map<string, iModel> modelList;
		map<string, int> textureList;
		map<string, int> shaderList;
		char* ReadShader(string fileName);
	
	public:
		void LoadModel(string modelName, string fileName);
		void LoadTexture(string textureName, string fileName);
		void LoadShader(string shaderName, string vertexProgram, string fragmentProgram, string geometryProgram = "");
};