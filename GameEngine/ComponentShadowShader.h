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
		iComponent(ComponentType::COMPONENT_SHADOW_SHADER),
		directionShadowShader(ResourceManager::GetShader(directionShadowName)),
		pointShadowShader(ResourceManager::GetShader(pointShadowName))
	{

	}

	ComponentShadowShader& operator=(const ComponentShadowShader&) = delete;
	ComponentShadowShader(ComponentShadowShader&) = delete;

	~ComponentShadowShader();

	void RenderSwap() override;

	//Gets the directional shader
	inline Shader * GetDirectionalShader() const
	{
		return directionShadowShader;
	}

	//Gets the point shader
	inline Shader * GetPointShader() const
	{
		return pointShadowShader;
	}
};

