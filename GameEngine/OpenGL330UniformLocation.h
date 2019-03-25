#pragma once

#include "UniformLocation.h"

class OpenGL330UniformLocation : public UniformLocation
{
private:
	int location;
public:
	OpenGL330UniformLocation(int pLocation);
	~OpenGL330UniformLocation();

	OpenGL330UniformLocation(const OpenGL330UniformLocation&) = delete;
	OpenGL330UniformLocation& operator=(const OpenGL330UniformLocation&) = delete;

	void SetUniform1i(int value) override;
	void SetUniform1f(float value) override;
	void SetUniform3fv(const glm::vec3 & value) override;
	void SetUniformMatrix4fv(const glm::mat4 & value, bool transpose) override;
	void UseTexture(int activeTexture) override;
};

