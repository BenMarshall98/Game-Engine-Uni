#include "Shader.h"
#include "LoggingManager.h"
#include <fstream>
#include <sstream>

bool Shader::LoadShader(const std::string & vertexProgram, const std::string & fragmentProgram, const std::string & geometryProgram)
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
		std::string message = "Failed to Link Shaders:" + std::string(infoLog);
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

void Shader::ReadShader(const std::string & fileName, std::string & shaderProgram)
{
	std::ifstream reader(fileName.c_str());

	if (reader.fail())
	{
		std::string message = "File" + fileName + "does not exist";
		LoggingManager::LogMessage(MESSAGE_TYPE::SEVERE, message);
		shaderProgram = "";
		return;
	}
	std::ostringstream buffer;

	buffer << reader.rdbuf();
	shaderProgram = buffer.str();
	reader.close();
}

int Shader::CompileShader(const std::string & fileName, GLenum shaderType)
{
	std::string shaderProgram;
	int success, shader;

	ReadShader(fileName, shaderProgram);
	shader = glCreateShader(shaderType);
	const char * program = shaderProgram.c_str();
	glShaderSource(shader, 1, &program, nullptr);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::string message = "Failed to Compile Shader: " + fileName + "\n" + infoLog;
		LoggingManager::LogMessage(MESSAGE_TYPE::SEVERE, message);
	}

	return shader;
}