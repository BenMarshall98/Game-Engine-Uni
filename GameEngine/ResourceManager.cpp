#include <fstream>

#include "ResourceManager.h"

void ResourceManager::LoadModel(string modelName, string fileName)
{
	if (modelList.find(modelName) != modelList.end())
	{
		//TODO: Log that the model already exists as a resource
		return;
	}


}

void ResourceManager::LoadTexture(string textureName, string fileName)
{
	if (textureList.find(textureName) != textureList.end())
	{
		//TODO: Log that the texture already exists as a resource
		return;
	}


}

void ResourceManager::LoadShader(string shaderName, string vertexProgram, string fragmentProgram, string geometryProgram = "")
{
	if (shaderList.find(shaderName) != shaderList.end())
	{
		//TODO: Log that the texture already exists as a resource
		return;
	}


}

char* ResourceManager::ReadShader(string fileName)
{
	ifstream reader(fileName.c_str());

	
}