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
	COMPONENT_ARTIFICAL_INTELLIGENCE,
	COMPONENT_ANIMATION,
	COMPONENT_STATE,
	MAX_SIZE
};

class iComponent
{
private:
	ComponentType componentType;

protected:
	iComponent(ComponentType pComponentType);

public:
	virtual ~iComponent();
	virtual void RenderSwap() = 0;

	inline ComponentType GetComponentName()
	{
		return componentType;
	}
};

