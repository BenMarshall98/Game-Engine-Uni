#pragma once

#include "OpenGL.h"
#include <string>
#include "Shader.h"

class OpenGL330Shader : public Shader
{
private:
	int mShaderID;
	static int CompileShader(const std::string & fileName, GLenum shaderType);

public:
	OpenGL330Shader();
	~OpenGL330Shader();

	void UseShader() const override;
	bool LoadShader(const std::string & vertexProgram, const std::string & fragmentProgram, const std::string & geometryProgram) override;

	void SetUniform1i(std::string uniformName, int value) override;
	void SetUniform1f(std::string uniformName, float value) override;
	void SetUniform3fv(std::string uniformName, glm::vec3 value) override;
	void SetUniformMatrix4fv(std::string uniformName, glm::mat4 value, bool transpose) override;
	void UseTexture(std::string uniformName, int activeTexture) override;
};

