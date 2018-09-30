#pragma once

#include "OpenGL.h"

#include <string>

using namespace std;

class Shader
{
private:
	int mShaderID;
	void ReadShader(string & fileName, char * shaderProgram) const;
	int CompileShader(string & fileName, GLuint shaderType) const;

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

	inline void UseShader() const
	{
		glUseProgram(mShaderID);
	}

	bool LoadShader(string & vertexProgram, string & fragmentProgram, string & geometryProgram);
	
};

