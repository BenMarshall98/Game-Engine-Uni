#pragma once

#include "iComponent.h"
#include "Shader.h"
#include "ResourceManager.h"

class ComponentShader : public iComponent
{
private:
	Shader * shader;
public:
	explicit ComponentShader(const std::string & shaderName) : shader(ResourceManager::GetShader(shaderName))
	{
	}

	ComponentShader& operator=(const ComponentShader&) = delete;
	ComponentShader(ComponentShader&) = delete;

	~ComponentShader();

	ComponentType GetComponentName() override;
	void RenderSwap() override;

	inline Shader * GetRenderShader() const
	{
		return shader;
	}
};

