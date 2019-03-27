#pragma once

#include "iModel.h"
#include "Shader.h"
#include "Texture.h"
#include "Buffer.h"
#include "Source.h"

#include <map>
#include <vector>
#include <string>

class ResourceManager
{
	private:
		static std::vector<std::string> usedModels;
		static std::vector<std::string> usedTextures;
		static std::vector<std::string> usedShaders;
		static std::vector<std::string> usedAudios;

		static std::map<std::string, iModel *> modelList;
		static std::map<std::string, Texture *> textureList;
		static std::map<std::string, Shader *> shaderList;
		static std::map<std::string, Buffer *> audioBufferList;

		static std::map<std::string, int> modelUsage;
		static std::map<std::string, int> textureUsage;
		static std::map<std::string, int> shaderUsage;
		static std::map<std::string, int> audioBufferUsage;

		ResourceManager() = delete;
	
	public:
		static void LoadModel(const std::string & modelName, const std::string & fileName);
		static void LoadTexture(const std::string & textureName, const std::string & fileName);
		static void LoadShader(const std::string & shaderName, const std::string & vertexProgram, const std::string & fragmentProgram, const std::string & geometryProgram = "");
		static void LoadAudio(const std::string & audioName, const std::string & fileName);

		static iModel * GetModel(const std::string & model);
		static Shader * GetShader(const std::string & shader);
		static Texture * GetTexture(const std::string & texture);
		static Source * GetAudio(const std::string & audio);

		static void RemoveModel(const iModel * const model);
		static void RemoveShader(const Shader * const shader);
		static void RemoveTexture(const Texture * const texture);
		static void RemoveAudio(const void * const audio);

		static void ClearResources();
		static void FinalClearResources();
};