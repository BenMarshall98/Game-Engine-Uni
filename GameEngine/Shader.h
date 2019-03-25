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

	virtual ~Shader()
	{
		
	}

	Shader(const Shader& shader) = delete;
	Shader& operator=(const Shader& shader) = delete;

	virtual void UseShader() const = 0;
	virtual bool LoadShader(const std::string & vertexProgram, const std::string & fragmentProgram, const std::string & geometryProgram) = 0;
	virtual void SetUniform1i(const std::string & uniformName, int value) = 0;
	virtual void SetUniform1f(const std::string & uniformName, float value) = 0;
	virtual void SetUniform3fv(const std::string & uniformName, const glm::vec3 & value) = 0;
	virtual void SetUniformMatrix4fv(const std::string & uniformName, const glm::mat4 & value, bool transpose) = 0;
	virtual void UseTexture(const std::string & uniformName, int activeTexture) = 0;
};

