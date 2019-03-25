#pragma once

#include "glm/glm.hpp"
#include <string>
#include "UniformLocation.h"

class Shader
{
protected:
	static void ReadShader(const std::string & fileName, std::string & shaderProgram);
public:
	Shader()
	{
	}

	virtual ~Shader()
	{
		
	}

	Shader(const Shader& shader) = delete;
	Shader& operator=(const Shader& shader) = delete;

	virtual void UseShader() const = 0;
	virtual bool LoadShader(const std::string & vertexProgram, const std::string & fragmentProgram, const std::string & geometryProgram) = 0;
	virtual UniformLocation * GetUniformLocation(const std::string & uniformName) = 0;
};

