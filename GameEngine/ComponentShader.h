#pragma once

#include "iComponent.h"
#include "Shader.h"
#include "ResourceManager.h"

class ComponentShader : public iComponent
{
private:
	Shader * shader;
public:
	explicit ComponentShader(string shaderName)
	{
		shader = ResourceManager::GetShader(shaderName);
	}

	~ComponentShader() {}

	ComponentType GetComponentName() override;
	void RenderSwap() override {}

	inline Shader * GetRenderShader()
	{
		return shader;
	}
};

