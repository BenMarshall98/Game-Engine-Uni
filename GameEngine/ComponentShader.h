#pragma once

#include "iComponent.h"
#include "Shader.h"

class ComponentShader : public iComponent
{
private:
	Shader * shader;
public:
	ComponentShader(Shader * pShader) : shader(pShader) {}

	~ComponentShader()
	{
		delete shader;
	}

	string GetComponentName();
	void Swap() {}

	inline Shader * GetShader()
	{
		return shader;
	}
};

