#include <fstream>
#include <algorithm>

#include "OpenGL.h"

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
	string shaderConcat = vertexProgram + fragmentProgram + geometryProgram;
	if(find(usedShaders.begin(), usedShaders.end(), shaderConcat) == usedShaders.end())
	{
		usedShaders.push_back(shaderConcat);
	}
	else
	{
		//TODO: Log that the shader programs have already exists as a resource
	}

	if (shaderList.find(shaderName) != shaderList.end())
	{
		//TODO: Log that the shader mapping name already exists
	}

	
	Shader shader = Shader();
	if (!shader.LoadShader(vertexProgram, fragmentProgram, geometryProgram))
	{
		//TODO: Log that the shader failed to compile
		return;
	}
	
	shaderList.insert(pair<string, Shader>(shaderName, shader));
}

