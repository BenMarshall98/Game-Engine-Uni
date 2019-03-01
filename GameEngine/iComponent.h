#pragma once

#include <string>

enum class ComponentType
{
	COMPONENT_DIRECTION,
	COMPONENT_INPUT,
	COMPONENT_MODEL,
	COMPONENT_NORMAL_TEXTURE,
	COMPONENT_PHYSICS,
	COMPONENT_POSITION,
	COMPONENT_SHADER,
	COMPONENT_TEXTURE,
	COMPONENT_AUDIO,
	COMPONENT_RIGGED_ANIMATION,
	COMPONENT_SHADOW_SHADER,
	COMPONENT_ARTIFICAL_INTELLIGENCE
};

class iComponent
{
protected:
	iComponent() {}
public:
	virtual ~iComponent();
	virtual ComponentType GetComponentName() = 0;
	virtual void RenderSwap() = 0;
};

