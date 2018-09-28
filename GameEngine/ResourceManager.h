#pragma once

#include "iModel.h"
#include "Shader.h"
#include "Texture.h"

#include <map>
#include <vector>
#include <string>

using namespace std;

class ResourceManager
{
	private:
		vector<string> usedModels;
		vector<string> usedTextures;
		vector<string> usedShaders;

		map<string, iModel> modelList;
		map<string, Texture> textureList;
		map<string, Shader> shaderList;
	
	public:
		void LoadModel(string modelName, string fileName);
		void LoadTexture(string textureName, string fileName);
		void LoadShader(string shaderName, string vertexProgram, string fragmentProgram, string geometryProgram = "");
};