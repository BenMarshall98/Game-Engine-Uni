#include "Shader.h"
#include "LoggingManager.h"
#include <fstream>
#include <sstream>

bool Shader::LoadShader(const string & vertexProgram, const string & fragmentProgram, const string & geometryProgram)
{
	

	/*
	The following code is based from the following resource:

	https://learnopengl.com/Getting-started/Shaders
	*/
	unsigned int vertexShader, fragmentShader, geometryShader = 0;
	int success;
	char infoLog[512];

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
		glGetProgramInfoLog(mShaderID, 512, nullptr, infoLog);
		LoggingManager::LogMessage(SEVERE, "Failed to Link Shaders:" + string(infoLog));
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

void Shader::ReadShader(const string & fileName, string & shaderProgram) const
{
	ifstream reader(fileName.c_str());

	if (reader.fail())
	{
		LoggingManager::LogMessage(SEVERE, "File" + fileName + "does not exist");
		shaderProgram = "";
		return;
	}
	ostringstream buffer;

	buffer << reader.rdbuf();
	shaderProgram = buffer.str();
	reader.close();
}

int Shader::CompileShader(const string & fileName, GLenum shaderType) const
{
	string shaderProgram;
	int success, shader;
	char infoLog[512];

	GLenum test = (GLenum)shaderType;

	ReadShader(fileName, shaderProgram);
	shader = glCreateShader(shaderType);
	const char * program = shaderProgram.c_str();
	glShaderSource(shader, 1, &program, nullptr);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		LoggingManager::LogMessage(SEVERE, "Failed to Compile Shader: " + fileName + "\n" + infoLog);
	}

	return shader;
}