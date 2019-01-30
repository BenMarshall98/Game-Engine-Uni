#include "ResourceManager.h"
#include "ModelLoader.h"
#include "AudioManager.h"
#include <algorithm>

vector<string> ResourceManager::usedModels;
vector<string> ResourceManager::usedTextures;
vector<string> ResourceManager::usedShaders;
vector<string> ResourceManager::usedAudios;

map<string, iModel *> ResourceManager::modelList;
map<string, Texture *> ResourceManager::textureList;
map<string, Shader *> ResourceManager::shaderList;
map<string, unsigned int> ResourceManager::audioBufferList;

void ResourceManager::LoadModel(const string & modelName, const string & fileName)
{
	if (find(usedModels.begin(), usedModels.end(), fileName) == usedModels.end())
	{
		usedModels.push_back(fileName);
	}
	else
	{
		//TODO: Log that the model file already exists as a resource
		return;
	}
	if (modelList.find(modelName) != modelList.end())
	{
		//TODO: Log that the model already exists as a resource
		return;
	}

	iModel * model = ModelLoader::LoadModel(fileName);

	if (model == nullptr)
	{
		//TODO: Log that the model failed to load
		return;
	}

	modelList.insert(pair<string, iModel*>(modelName, model));
}

void ResourceManager::LoadTexture(const string & textureName, const string & fileName)
{
	if (find(usedTextures.begin(), usedTextures.end(), fileName) == usedTextures.end())
	{
		usedTextures.push_back(fileName);
	}
	else
	{
		//TODO: Log that the texture file already exists as a resource
		return;
	}

	if (textureList.find(textureName) != textureList.end())
	{
		//TODO: Log that the texture already exists as a resource
		return;
	}

	Texture * texture = new Texture();
	if (!texture->LoadTexture(fileName))
	{
		//TODO: Log that the texture failed to load
		return;
	}

	textureList.insert(pair<string, Texture *>(textureName, texture));
}

void ResourceManager::LoadShader(const string & shaderName, const string & vertexProgram, const string & fragmentProgram, const string & geometryProgram)
{
	const string shaderConcat = vertexProgram + fragmentProgram + geometryProgram;
	if(find(usedShaders.begin(), usedShaders.end(), shaderConcat) == usedShaders.end())
	{
		usedShaders.push_back(shaderConcat);
	}
	else
	{
		//TODO: Log that the shader programs  already exists as a resource
		return;
	}

	if (shaderList.find(shaderName) != shaderList.end())
	{
		//TODO: Log that the shader mapping name already exists
		return;
	}

	
	Shader * shader = new Shader();
	if (!shader->LoadShader(vertexProgram, fragmentProgram, geometryProgram))
	{
		//TODO: Log that the shader failed to compile
		return;
	}
	
	shaderList.insert(pair<string, Shader *>(shaderName, shader));
}

void ResourceManager::LoadAudio(const string & audioName, const string & fileName)
{
	if (find(usedAudios.begin(), usedAudios.end(), fileName) == usedAudios.end())
	{
		usedAudios.push_back(fileName);
	}
	else
	{
		//TODO: Log that the  audio file already exists as a resource
		return;
	}

	if (audioBufferList.find(audioName) != audioBufferList.end())
	{
		//TODO: Log that the audio already exists as a resource
		return;
	}

	unsigned int buffer = AudioManager::Instance()->GenerateBuffer(fileName);

	if (buffer == 0)
	{
		//TODO: Log that the audio failed to load
		return;
	}

	audioBufferList.insert(pair<string, unsigned int>(audioName, buffer));
}

iModel * ResourceManager::GetModel(const string & model)
{
	map<string, iModel *>::iterator it = modelList.find(model);

	if (it != modelList.end())
	{
		return it->second;
	}
	return nullptr;
}

Shader * ResourceManager::GetShader(const string & shader)
{
	map<string, Shader *>::iterator it = shaderList.find(shader);

	if (it != shaderList.end())
	{
		return it->second;
	}
	return nullptr;
}

Texture * ResourceManager::GetTexture(const string & texture)
{
	map<string, Texture *>::iterator it = textureList.find(texture);

	if (it != textureList.end())
	{
		return it->second;
	}
	return nullptr;
}

unsigned int ResourceManager::GetAudio(const string & audio)
{
	map<string, unsigned int>::iterator it = audioBufferList.find(audio);

	if (it != audioBufferList.end())
	{
		unsigned int source = AudioManager::Instance()->GenerateSource(it->second);
		return source;
	}
	return 0;
}