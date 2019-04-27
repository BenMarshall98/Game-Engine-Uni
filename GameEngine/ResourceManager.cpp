#define NOMINMAX

#include "ResourceManager.h"
#include "ModelLoader.h"
#include "AudioManager.h"
#include <algorithm>
#include "RenderManager.h"

std::vector<std::string> ResourceManager::usedModels;
std::vector<std::string> ResourceManager::usedTextures;
std::vector<std::string> ResourceManager::usedShaders;
std::vector<std::string> ResourceManager::usedAudios;

std::map<std::string, iModel *> ResourceManager::modelList;
std::map<std::string, Texture *> ResourceManager::textureList;
std::map<std::string, Shader *> ResourceManager::shaderList;
std::map<std::string, Buffer *> ResourceManager::audioBufferList;

std::map<std::string, int> ResourceManager::modelUsage;
std::map<std::string, int> ResourceManager::audioBufferUsage;
std::map<std::string, int> ResourceManager::shaderUsage;
std::map<std::string, int> ResourceManager::textureUsage;

//Loads model into game engine
void ResourceManager::LoadModel(const std::string & modelName, const std::string & fileName)
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

	modelUsage.insert(std::pair<std::string, int>(modelName, 0));
	modelList.insert(std::pair<std::string, iModel*>(modelName, model));
}

//Loads texture into game engine
void ResourceManager::LoadTexture(const std::string & textureName, const std::string & fileName)
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

	Texture * texture = RenderManager::Instance()->CreateTexture(fileName);
	if (!texture)
	{
		//TODO: Log that the texture failed to load
		return;
	}

	textureUsage.insert(std::pair<std::string, int>(textureName, 0));
	textureList.insert(std::pair<std::string, Texture *>(textureName, texture));
}

//Loads shader into game engine
void ResourceManager::LoadShader(const std::string & shaderName, const std::string & vertexProgram, const std::string & fragmentProgram, const std::string & geometryProgram)
{
	const std::string shaderConcat = vertexProgram + fragmentProgram + geometryProgram;
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

	
	Shader * shader = RenderManager::Instance()->CreateShader(vertexProgram, fragmentProgram, geometryProgram);
	if (!shader)
	{
		//TODO: Log that the shader failed to compile
		return;
	}
	
	shaderUsage.insert(std::pair<std::string, int>(shaderName, 0));
	shaderList.insert(std::pair<std::string, Shader *>(shaderName, shader));
}

//Loads audio into game engine
void ResourceManager::LoadAudio(const std::string & audioName, const std::string & fileName)
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

	Buffer * buffer = AudioManager::Instance()->GenerateBuffer(fileName);

	if (!buffer)
	{
		//TODO: Log that buffer failed to be created
	}

	audioBufferUsage.insert(std::pair<std::string, int>(audioName, 0));
	audioBufferList.insert(std::pair<std::string, Buffer *>(audioName, buffer));
}

//Gets model by model identity
iModel * ResourceManager::GetModel(const std::string & model)
{
	const std::map<std::string, iModel *>::iterator it = modelList.find(model);
	const std::map<std::string, int>::iterator count = modelUsage.find(model);

	if (it != modelList.end())
	{
		count->second++;
		return it->second;
	}
	return nullptr;
}

//Gets shader by shader identity
Shader * ResourceManager::GetShader(const std::string & shader)
{
	const std::map<std::string, Shader *>::iterator it = shaderList.find(shader);
	const std::map<std::string, int>::iterator count = shaderUsage.find(shader);

	if (it != shaderList.end())
	{
		count->second++;
		return it->second;
	}
	return nullptr;
}

//Gets texture by texture identity
Texture * ResourceManager::GetTexture(const std::string & texture)
{
	const std::map<std::string, Texture *>::iterator it = textureList.find(texture);
	const std::map<std::string, int>::iterator count = textureUsage.find(texture);

	if (it != textureList.end())
	{
		count->second++;
		return it->second;
	}
	return nullptr;
}

//Gets audio by audio identity
Source * ResourceManager::GetAudio(const std::string & audio)
{
	const std::map<std::string, Buffer *>::iterator it = audioBufferList.find(audio);
	const std::map<std::string, int>::iterator count = audioBufferUsage.find(audio);

	if (it != audioBufferList.end())
	{
		count->second++;
		Source * const source = AudioManager::Instance()->GenerateSource(it->second);
		return source;
	}
	return nullptr;
}

//Removes model
void ResourceManager::RemoveModel(const iModel * const model)
{
	std::map<std::string, iModel *>::iterator it;
	std::string modelName = "";

	for (it = modelList.begin(); it != modelList.end(); it++)
	{
		if (it->second == model)
		{
			modelName = it->first;
			break;
		}
	}

	const std::map<std::string, int>::iterator count = modelUsage.find(modelName);

	if (count != modelUsage.end())
	{
		count->second--;
	}
}

//Removes Shader
void ResourceManager::RemoveShader(const Shader * const shader)
{
	std::map<std::string, Shader *>::iterator it;
	std::string shaderName = "";

	for (it = shaderList.begin(); it != shaderList.end(); it++)
	{
		if (it->second == shader)
		{
			shaderName = it->first;
			break;
		}
	}

	const std::map<std::string, int>::iterator count = shaderUsage.find(shaderName);

	if (count != shaderUsage.end())
	{
		count->second--;
	}
}

//Removes texture
void ResourceManager::RemoveTexture(const Texture * const texture)
{
	std::map<std::string, Texture *>::iterator it;
	std::string textureName = "";

	for (it = textureList.begin(); it != textureList.end(); it++)
	{
		if (it->second == texture)
		{
			textureName = it->first;
			break;
		}
	}

	const std::map<std::string, int>::iterator count = textureUsage.find(textureName);

	if (count != textureUsage.end())
	{
		count->second--;
	}
}

//Removes audio
void ResourceManager::RemoveAudio(const void * const audio)
{
	std::map<std::string, Buffer *>::iterator it;
	std::string audioName = "";

	for (it = audioBufferList.begin(); it != audioBufferList.end(); it++)
	{
		if (it->second == audio)
		{
			audioName = it->first;
			break;
		}
	}

	const std::map<std::string, int>::iterator count = audioBufferUsage.find(audioName);

	if (count != audioBufferUsage.end())
	{
		count->second--;
	}
}

//Removes resources that are unused
void ResourceManager::ClearResources()
{
	usedModels.clear();
	usedShaders.clear();
	usedTextures.clear();
	usedAudios.clear();

	{
		std::map<std::string, iModel *>::iterator it;

		for (it = modelList.begin(); it != modelList.end();)
		{
			const std::map<std::string, int>::iterator count = modelUsage.find(it->first);

			if (count->second == 0)
			{
				delete it->second;
				it->second = nullptr;

				modelList.erase(it);
				modelUsage.erase(count);

				it = modelList.begin();
			}
			else
			{
				it++;
			}
		}
	}

	{
		std::map<std::string, Texture *>::iterator it;

		for (it = textureList.begin(); it != textureList.end();)
		{
			const std::map<std::string, int>::iterator count = textureUsage.find(it->first);

			if (count->second == 0)
			{
				delete it->second;
				it->second = nullptr;

				textureList.erase(it);
				textureUsage.erase(count);

				it = textureList.begin();
			}
			else
			{
				it++;
			}
		}
	}

	{
		std::map<std::string, Shader *>::iterator it;

		for (it = shaderList.begin(); it != shaderList.end();)
		{
			const std::map<std::string, int>::iterator count = shaderUsage.find(it->first);

			if (count->second == 0)
			{
				delete it->second;
				it->second = nullptr;

				shaderList.erase(it);
				shaderUsage.erase(count);

				it = shaderList.begin();
			}
			else
			{
				it++;
			}
		}
	}
}

//Deletes all resources
void ResourceManager::FinalClearResources()
{
	usedModels.clear();
	usedShaders.clear();
	usedTextures.clear();
	usedAudios.clear();

	{
		std::map<std::string, iModel *>::iterator it;

		for (it = modelList.begin(); it != modelList.end(); it++)
		{
			delete it->second;
			it->second = nullptr;
		}

		modelList.clear();
	}

	{
		std::map<std::string, Texture *>::iterator it;

		for (it = textureList.begin(); it != textureList.end(); it++)
		{
			delete it->second;
			it->second = nullptr;
		}

		textureList.clear();
	}

	{
		std::map<std::string, Shader *>::iterator it;

		for (it = shaderList.begin(); it != shaderList.end(); it++)
		{
			delete it->second;
			it->second = nullptr;
		}

		shaderList.clear();
	}

	{
		std::map<std::string, Buffer *>::iterator it;

		for (it = audioBufferList.begin(); it != audioBufferList.end(); it++)
		{
			AudioManager::Instance()->DeleteBuffer(it->second);
			it->second = nullptr;
		}

		audioBufferList.clear();
	}
}