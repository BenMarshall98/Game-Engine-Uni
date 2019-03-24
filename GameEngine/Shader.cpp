#include "Shader.h"
#include "LoggingManager.h"
#include <fstream>
#include <sstream>

void Shader::ReadShader(const std::string & fileName, std::string & shaderProgram)
{
	std::ifstream reader(fileName.c_str());

	if (reader.fail())
	{
		const std::string message = "File" + fileName + "does not exist";
		LoggingManager::LogMessage(MESSAGE_TYPE::SEVERE, message);
		shaderProgram = "";
		return;
	}
	std::ostringstream buffer;

	buffer << reader.rdbuf();
	shaderProgram = buffer.str();
	reader.close();
}