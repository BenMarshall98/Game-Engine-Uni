#pragma once
#include "iComponent.h"
#include "glm/glm.hpp"

using namespace glm;

class ComponentDirection : public iComponent
{
private:
	vec3 direction;
public:
	ComponentDirection(vec3 pDirection) : direction(pDirection) {}
	~ComponentDirection() {};

	string GetComponentName();
	void Swap() {}

	inline vec3 GetDirection()
	{
		return direction;
	}

	inline void SetPosition(vec3 pDirection)
	{
		direction = pDirection;
	}
};

