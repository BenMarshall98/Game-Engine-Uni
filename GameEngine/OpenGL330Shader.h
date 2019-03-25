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

	OpenGL330Shader& operator=(const OpenGL330Shader&) = delete;
	OpenGL330Shader(OpenGL330Shader&) = delete;

	void UseShader() const override;
	bool LoadShader(const std::string & vertexProgram, const std::string & fragmentProgram, const std::string & geometryProgram) override;
	UniformLocation * GetUniformLocation(const std::string & uniformName) override;
};

