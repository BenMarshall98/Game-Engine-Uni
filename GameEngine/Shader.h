#pragma once

#include "OpenGL.h"

#include <string>

using namespace std;

class Shader
{
private:
	int mShaderID;
	static void ReadShader(const string & fileName, string & shaderProgram);
	static int CompileShader(const string & fileName, GLenum shaderType);

	Shader(const Shader& shader) = delete;
	Shader& operator=(const Shader& shader) = delete;

public:
	Shader() : mShaderID(0)
	{
	}

	~Shader()
	{
		glDeleteProgram(mShaderID);
	}

	inline int ShaderID() const
	{
		return mShaderID;
	}

	inline void UseShader() const
	{
		glUseProgram(mShaderID);
	}

	bool LoadShader(const string & vertexProgram, const string & fragmentProgram, const string & geometryProgram);
	
};

