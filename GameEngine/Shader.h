#pragma once

#include "OpenGL.h"

#include <string>

class Shader
{
private:
	int mShaderID;
	static void ReadShader(const std::string & fileName, std::string & shaderProgram);
	static int CompileShader(const std::string & fileName, GLenum shaderType);

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

	bool LoadShader(const std::string & vertexProgram, const std::string & fragmentProgram, const std::string & geometryProgram);
	
};

