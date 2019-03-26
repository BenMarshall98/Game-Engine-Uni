#pragma once

#include "iComponent.h"
#include "Shader.h"
#include "ResourceManager.h"

class ComponentShader : public iComponent
{
private:
	Shader * shader;
public:
	explicit ComponentShader(const std::string & shaderName) :
		iComponent(ComponentType::COMPONENT_SHADER), shader(ResourceManager::GetShader(shaderName))
	{
	}

	ComponentShader& operator=(const ComponentShader&) = delete;
	ComponentShader(ComponentShader&) = delete;

	~ComponentShader();

	void RenderSwap() override;

	inline Shader * GetRenderShader() const
	{
		return shader;
	}
};

