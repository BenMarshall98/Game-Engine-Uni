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

		map<string, iModel *> modelList;
		map<string, Texture *> textureList;
		map<string, Shader *> shaderList;

		ResourceManager(void) {};
	
	public:
		void LoadModel(const string & modelName, const string & fileName);
		void LoadTexture(const string & textureName, const string & fileName);
		void LoadShader(const string & shaderName, const string & vertexProgram, const string & fragmentProgram, const string & geometryProgram = "");
};