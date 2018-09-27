#pragma once

#include "OpenGL.h"

class Shader
{
private:
	int mShaderID;

public:
	Shader(int pShaderID) : mShaderID(pShaderID)
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
};

