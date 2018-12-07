#pragma once

#include <string>

using namespace std;

enum ComponentType
{
	COMPONENT_DIRECTION,
	COMPONENT_INPUT,
	COMPONENT_MODEL,
	COMPONENT_NORMAL,
	COMPONENT_PHYSICS,
	COMPONENT_POSITION,
	COMPONENT_SHADER,
	COMPONENT_TEXTURE
};

class iComponent
{
private:

public:
	virtual ~iComponent()
	{
	}
	virtual ComponentType GetComponentName() = 0;
	virtual void Swap() = 0;
};

