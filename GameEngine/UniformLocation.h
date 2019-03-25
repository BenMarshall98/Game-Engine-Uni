#pragma once

#include "glm/glm.hpp"
#include <string>

class UniformLocation
{
public:
	UniformLocation();
	virtual ~UniformLocation();

	UniformLocation(const UniformLocation&) = delete;
	UniformLocation& operator=(const UniformLocation&) = delete;

	virtual void SetUniform1i(int value) = 0;
	virtual void SetUniform1f(float value) = 0;
	virtual void SetUniform3fv(const glm::vec3 & value) = 0;
	virtual void SetUniformMatrix4fv(const glm::mat4 & value, bool transpose) = 0;
	virtual void UseTexture(int activeTexture) = 0;
};

