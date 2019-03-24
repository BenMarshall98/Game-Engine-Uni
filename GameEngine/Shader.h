#pragma once

#include "glm/glm.hpp"
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
	virtual void SetUniform1i(std::string uniformName, int value) = 0;
	virtual void SetUniform1f(std::string uniformName, float value) = 0;
	virtual void SetUniform3fv(std::string uniformName, glm::vec3 value) = 0;
	virtual void SetUniformMatrix4fv(std::string uniformName, glm::mat4 value, bool transpose) = 0;
	virtual void UseTexture(std::string uniformName, int activeTexture) = 0;
};

