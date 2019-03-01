#pragma once

#include "iComponent.h"
#include "Shader.h"
#include "ResourceManager.h"

class ComponentShadowShader : public iComponent
{
private:
	Shader * directionShadowShader;
	Shader * pointShadowShader;
public:
	explicit ComponentShadowShader(const std::string & directionShadowName, const std::string & pointShadowName) :
		directionShadowShader(ResourceManager::GetShader(directionShadowName)),
		pointShadowShader(ResourceManager::GetShader(pointShadowName))
	{

	}

	ComponentShadowShader& operator=(const ComponentShadowShader&) = delete;
	ComponentShadowShader(ComponentShadowShader&) = delete;

	~ComponentShadowShader();

	ComponentType GetComponentName() override;
	void RenderSwap() override;

	inline Shader * GetDirectionalShader() const
	{
		return directionShadowShader;
	}

	inline Shader * GetPointShader() const
	{
		return pointShadowShader;
	}
};

