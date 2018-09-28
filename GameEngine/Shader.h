#pragma once

#include "OpenGL.h"

#include <string>

using namespace std;

class Shader
{
private:
	int mShaderID;
	void ReadShader(string fileName, char * shaderProgram);
	int CompileShader(string fileName, GLuint shaderType);

public:
	Shader() : mShaderID(0)
	{
	}

	~Shader()
	{
		glDeleteProgram(mShaderID);
	}

	inline void UseShader()
	{
		glUseProgram(mShaderID);
	}

	bool LoadShader(string vertexProgram, string fragmentProgram, string geometryProgram);
	
};

