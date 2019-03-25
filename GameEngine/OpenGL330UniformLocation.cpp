#include "OpenGL330UniformLocation.h"
#include "OpenGL.h"
#include "glm/gtc/type_ptr.hpp"

OpenGL330UniformLocation::OpenGL330UniformLocation(int pLocation) : location(pLocation)
{
}


OpenGL330UniformLocation::~OpenGL330UniformLocation()
{
}

void OpenGL330UniformLocation::SetUniform1i(const int value)
{
	glUniform1i(location, value);
}

void OpenGL330UniformLocation::SetUniform1f(const float value)
{
	glUniform1f(location, value);
}

void OpenGL330UniformLocation::SetUniform3fv(const glm::vec3 & value)
{
	glUniform3fv(location, 1, value_ptr(value));
}

void OpenGL330UniformLocation::SetUniformMatrix4fv(const glm::mat4 & value, const bool transpose)
{
	glUniformMatrix4fv(location, 1, transpose, value_ptr(value));
}

void OpenGL330UniformLocation::UseTexture(const int activeTexture)
{
	glUniform1i(location, activeTexture);
	glActiveTexture(GL_TEXTURE0 + activeTexture);
}