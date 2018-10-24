#pragma once
#include "iComponent.h"
#include "glm/glm.hpp"

using namespace glm;

class ComponentPosition : public iComponent
{
private:
	vec3 * position;
public:
	ComponentPosition(vec3 * pPosition) : position(pPosition) {}
	~ComponentPosition()
	{
		delete position;
	}

	string GetComponentName();
	void Swap() {}

	inline vec3 * GetPosition()
	{
		return position;
	}
};

