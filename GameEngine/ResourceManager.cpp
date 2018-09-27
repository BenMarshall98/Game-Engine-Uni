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

	char * shaderProgram = nullptr;

	/*
	The following code is based from the following resource:

	https://learnopengl.com/Getting-started/Shaders
	*/
	unsigned int vertexShader, fragmentShader, geometryShader = 0;
	int success;
	char infoLog[512];

	//Compile Vertex Shader
	ReadShader(vertexProgram, shaderProgram);
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &shaderProgram, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		//TODO: Log that the vertexShader failed to be created, showing infoLog
	}

	//Compile Fragment Shader
	ReadShader(fragmentProgram, shaderProgram);
	fragmentProgram = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &shaderProgram, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		//TODO: Log that the fragmentShader failed to be created, showing infoLog
	}

	//Compile Geometry Shader
	if (geometryProgram != "")
	{
		ReadShader(geometryProgram, shaderProgram);
		geometryProgram = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometryShader, 1, &shaderProgram, NULL);
		glCompileShader(geometryShader);

		glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(geometryShader, 512, NULL, infoLog);
			//TODO: Log that the geometryShader failed to be created, showing infoLog
		}
	}


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