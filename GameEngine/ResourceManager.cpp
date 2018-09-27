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

	char * shaderProgram = nullptr;


}

void ResourceManager::ReadShader(string fileName, char * shaderProgram)
{
	ifstream reader(fileName.c_str());

	if (reader.fail())
	{
		//TODO: Log that the program does not exist
		shaderProgram = nullptr;
		return;
	}

	reader.seekg(0, ios::end);
	int length = reader.tellg();
	delete shaderProgram;
	shaderProgram = new char[length];
	
	reader.seekg(0, ios::beg);
	reader.read(shaderProgram, length);
	reader.close();
}