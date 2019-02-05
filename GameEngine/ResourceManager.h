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
		static vector<string> usedModels;
		static vector<string> usedTextures;
		static vector<string> usedShaders;
		static vector<string> usedAudios;

		static map<string, iModel *> modelList;
		static map<string, Texture *> textureList;
		static map<string, Shader *> shaderList;
		static map<string, void *> audioBufferList;

		ResourceManager(void) {};
	
	public:
		static void LoadModel(const string & modelName, const string & fileName);
		static void LoadTexture(const string & textureName, const string & fileName);
		static void LoadShader(const string & shaderName, const string & vertexProgram, const string & fragmentProgram, const string & geometryProgram = "");
		static void LoadAudio(const string & audioName, const string & fileName);

		static iModel * GetModel(const string & model);
		static Shader * GetShader(const string & shader);
		static Texture * GetTexture(const string & texture);
		static void * GetAudio(const string & audio);
};