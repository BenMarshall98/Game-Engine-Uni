#pragma once

#include <string>

using namespace std;

enum class ComponentType
{
	COMPONENT_DIRECTION,
	COMPONENT_INPUT,
	COMPONENT_MODEL,
	COMPONENT_NORMAL,
	COMPONENT_PHYSICS,
	COMPONENT_POSITION,
	COMPONENT_SHADER,
	COMPONENT_TEXTURE,
	COMPONENT_AUDIO
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

