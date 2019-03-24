#pragma once

#include <string>

class Shader
{
protected:
	static void ReadShader(const std::string & fileName, std::string & shaderProgram);
public:
	Shader()
	{
	}

	~Shader()
	{
		
	}

	Shader(const Shader& shader) = delete;
	Shader& operator=(const Shader& shader) = delete;

	virtual void UseShader() const = 0;
	virtual bool LoadShader(const std::string & vertexProgram, const std::string & fragmentProgram, const std::string & geometryProgram) = 0;
};

