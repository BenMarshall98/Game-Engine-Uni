#pragma once
#include "iComponent.h"
#include "glm/glm.hpp"

#include "glm/gtc/quaternion.hpp"

using namespace glm;

class ComponentDirection : public iComponent
{
private:
	quat direction;

public:
	ComponentDirection(vec3 pDirection, float pAngle)
	{
		direction = angleAxis(pAngle, pDirection);
	}
	~ComponentDirection() {};

	string GetComponentName();
	void Swap() {}

	inline quat GetDirection()
	{
		return direction;
	}

	inline void SetDirection(quat pDirection)
	{
		direction = pDirection;
	}
};

