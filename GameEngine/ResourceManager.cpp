#include "ResourceManager.h"
#include "ModelLoader.h"
#include "AudioManager.h"
#include <algorithm>

std::vector<std::string> ResourceManager::usedModels;
std::vector<std::string> ResourceManager::usedTextures;
std::vector<std::string> ResourceManager::usedShaders;
std::vector<std::string> ResourceManager::usedAudios;

std::map<std::string, iModel *> ResourceManager::modelList;
std::map<std::string, Texture *> ResourceManager::textureList;
std::map<std::string, Shader *> ResourceManager::shaderList;
std::map<std::string, void *> ResourceManager::audioBufferList;

std::map<std::string, int> ResourceManager::modelUsage;
std::map<std::string, int> ResourceManager::audioBufferUsage;
std::map<std::string, int> ResourceManager::shaderUsage;
std::map<std::string, int> ResourceManager::textureUsage;

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

	Texture * texture = new Texture();
	if (!texture->LoadTexture(fileName))
	{
		//TODO: Log that the texture failed to load
		return;
	}

	textureUsage.insert(std::pair<std::string, int>(textureName, 0));
	textureList.insert(std::pair<std::string, Texture *>(textureName, texture));
}

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

	
	Shader * shader = new Shader();
	if (!shader->LoadShader(vertexProgram, fragmentProgram, geometryProgram))
	{
		//TODO: Log that the shader failed to compile
		return;
	}
	
	shaderUsage.insert(std::pair<std::string, int>(shaderName, 0));
	shaderList.insert(std::pair<std::string, Shader *>(shaderName, shader));
}

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

	void * buffer = AudioManager::Instance()->GenerateBuffer(fileName);

	if (!buffer)
	{
		//TODO: Log that buffer failed to be created
	}

	audioBufferUsage.insert(std::pair<std::string, int>(audioName, 0));
	audioBufferList.insert(std::pair<std::string, void *>(audioName, buffer));
}

iModel * ResourceManager::GetModel(const std::string & model)
{
	std::map<std::string, iModel *>::iterator it = modelList.find(model);
	std::map<std::string, int>::iterator count = modelUsage.find(model);

	if (it != modelList.end())
	{
		count->second++;
		return it->second;
	}
	return nullptr;
}

Shader * ResourceManager::GetShader(const std::string & shader)
{
	std::map<std::string, Shader *>::iterator it = shaderList.find(shader);
	std::map<std::string, int>::iterator count = shaderUsage.find(shader);

	if (it != shaderList.end())
	{
		count->second++;
		return it->second;
	}
	return nullptr;
}

Texture * ResourceManager::GetTexture(const std::string & texture)
{
	std::map<std::string, Texture *>::iterator it = textureList.find(texture);
	std::map<std::string, int>::iterator count = textureUsage.find(texture);

	if (it != textureList.end())
	{
		count->second++;
		return it->second;
	}
	return nullptr;
}

void * ResourceManager::GetAudio(const std::string & audio)
{
	std::map<std::string, void *>::iterator it = audioBufferList.find(audio);
	std::map<std::string, int>::iterator count = audioBufferUsage.find(audio);

	if (it != audioBufferList.end())
	{
		count->second++;
		void * source = AudioManager::Instance()->GenerateSource(it->second);
		return source;
	}
	return 0;
}

void ResourceManager::RemoveModel(iModel * model)
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

	std::map<std::string, int>::iterator count = modelUsage.find(modelName);

	if (count != modelUsage.end())
	{
		count->second--;
	}
}

void ResourceManager::RemoveShader(Shader * shader)
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

	std::map<std::string, int>::iterator count = shaderUsage.find(shaderName);

	if (count != shaderUsage.end())
	{
		count->second--;
	}
}

void ResourceManager::RemoveTexture(Texture * texture)
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

	std::map<std::string, int>::iterator count = textureUsage.find(textureName);

	if (count != textureUsage.end())
	{
		count->second--;
	}
}

void ResourceManager::RemoveAudio(void * audio)
{
	std::map<std::string, void *>::iterator it;
	std::string audioName = "";

	for (it = audioBufferList.begin(); it != audioBufferList.end(); it++)
	{
		if (it->second == audio)
		{
			audioName = it->first;
			break;
		}
	}

	std::map<std::string, int>::iterator count = audioBufferUsage.find(audioName);

	if (count != audioBufferUsage.end())
	{
		count->second--;
	}
}

void ResourceManager::ClearResources()
{
	usedModels.clear();
	usedShaders.clear();
	usedTextures.clear();
	usedAudios.clear();

	{
		std::map<std::string, iModel *>::iterator it;

		for (it = modelList.begin(); it != modelList.end(); it++)
		{
			std::map<std::string, int>::iterator count = modelUsage.find(it->first);

			if (count->second == 0)
			{
				delete it->second;
				it->second = nullptr;

				modelList.erase(it);
				modelUsage.erase(count);
			}
		}
	}

	{
		std::map<std::string, Texture *>::iterator it;

		for (it = textureList.begin(); it != textureList.end(); it++)
		{
			std::map<std::string, int>::iterator count = textureUsage.find(it->first);

			if (count->second == 0)
			{
				delete it->second;
				it->second = nullptr;

				textureList.erase(it);
				textureUsage.erase(count);
			}
		}
	}

	{
		std::map<std::string, Shader *>::iterator it;

		for (it = shaderList.begin(); it != shaderList.end(); it++)
		{
			std::map<std::string, int>::iterator count = shaderUsage.find(it->first);

			if (count->second == 0)
			{
				delete it->second;
				it->second = nullptr;

				shaderList.erase(it);
				shaderUsage.erase(count);
			}
		}
	}

	{
		std::map<std::string, void *>::iterator it;

		for (it = audioBufferList.begin(); it != audioBufferList.end(); it++)
		{
			std::map<std::string, int>::iterator count = audioBufferUsage.find(it->first);

			if (count->second == 0)
			{
				AudioManager::Instance()->DeleteBuffer(it->second);
				it->second = nullptr;

				audioBufferList.erase(it);
				audioBufferUsage.erase(count);
			}
		}
	}
}

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
		std::map<std::string, void *>::iterator it;

		for (it = audioBufferList.begin(); it != audioBufferList.end(); it++)
		{
			AudioManager::Instance()->DeleteBuffer(it->second);
			it->second = nullptr;
		}

		audioBufferList.clear();
	}
}