#include "OpenGL330Shader.h"
#include "LoggingManager.h"
#include "glm/gtc/type_ptr.hpp"

OpenGL330Shader::OpenGL330Shader() : mShaderID(0)
{
}


OpenGL330Shader::~OpenGL330Shader()
{
	glDeleteProgram(mShaderID);
}

void OpenGL330Shader::UseShader() const
{
	glUseProgram(mShaderID);
}

bool OpenGL330Shader::LoadShader(const std::string & vertexProgram, const std::string & fragmentProgram, const std::string & geometryProgram)
{
	/*
	The following code is based from the following resource:

	https://learnopengl.com/Getting-started/Shaders
	*/
	unsigned int vertexShader, fragmentShader, geometryShader = 0;
	int success;

	vertexShader = CompileShader(vertexProgram, GL_VERTEX_SHADER);
	fragmentShader = CompileShader(fragmentProgram, GL_FRAGMENT_SHADER);

	//Compile Geometry Shader
	if (geometryProgram != "")
	{
		geometryShader = CompileShader(geometryProgram, GL_GEOMETRY_SHADER);
	}

	mShaderID = glCreateProgram();

	glAttachShader(mShaderID, vertexShader);
	glAttachShader(mShaderID, fragmentShader);

	if (geometryShader != 0)
	{
		glAttachShader(mShaderID, geometryShader);
	}

	glLinkProgram(mShaderID);

	glGetProgramiv(mShaderID, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(mShaderID, 512, nullptr, infoLog);
		const std::string message = "Failed to Link Shaders:" + std::string(infoLog);
		LoggingManager::LogMessage(MESSAGE_TYPE::SEVERE, message);
	}

	glDetachShader(mShaderID, vertexShader);
	glDetachShader(mShaderID, fragmentShader);
	if (geometryShader != 0)
	{
		glDetachShader(mShaderID, geometryShader);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(geometryShader);

	return success;
}

int OpenGL330Shader::CompileShader(const std::string & fileName, const GLenum shaderType)
{
	std::string shaderProgram;
	int success, shader;

	ReadShader(fileName, shaderProgram);
	shader = glCreateShader(shaderType);
	const char * const program = shaderProgram.c_str();
	glShaderSource(shader, 1, &program, nullptr);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		const std::string message = "Failed to Compile Shader: " + fileName + "\n" + infoLog;
		LoggingManager::LogMessage(MESSAGE_TYPE::SEVERE, message);
	}

	return shader;
}

void OpenGL330Shader::SetUniform1i(std::string uniformName, int value)
{
	int location = glGetUniformLocation(mShaderID, uniformName.c_str());
	glUniform1i(location, value);
}

void OpenGL330Shader::SetUniform1f(std::string uniformName, float value)
{
	int location = glGetUniformLocation(mShaderID, uniformName.c_str());
	glUniform1f(location, value);
}

void OpenGL330Shader::SetUniform3fv(std::string uniformName, glm::vec3 value)
{
	int location = glGetUniformLocation(mShaderID, uniformName.c_str());
	glUniform3fv(location, 1, value_ptr(value));
}

void OpenGL330Shader::SetUniformMatrix4fv(std::string uniformName, glm::mat4 value, bool transpose)
{
	int location = glGetUniformLocation(mShaderID, uniformName.c_str());
	glUniformMatrix4fv(location, 1, transpose, value_ptr(value));
}

void OpenGL330Shader::UseTexture(std::string uniformName, int activeTexture)
{
	int location = glGetUniformLocation(mShaderID, uniformName.c_str());
	glUniform1i(location, activeTexture);
	glActiveTexture(GL_TEXTURE0 + activeTexture);
}