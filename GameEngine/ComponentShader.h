#pragma once

#include "iComponent.h"
#include "Shader.h"
#include "ResourceManager.h"

class ComponentShader : public iComponent
{
private:
	Shader * shader;
public:
	ComponentShader(string shaderName)
	{
		shader = ResourceManager::GetShader(shaderName);
	}

	~ComponentShader()
	{
		delete shader;
	}

	ComponentType GetComponentName();
	void Swap() {}

	inline Shader * GetShader()
	{
		return shader;
	}
};

