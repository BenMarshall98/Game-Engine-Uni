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
	ComponentDirection(vec3 pDirection, float pAngle) : direction(angleAxis(pAngle, pDirection)) {}
	~ComponentDirection() {};
	
	ComponentType GetComponentName() override;
	void Swap() override {}

	inline quat GetDirection() const
	{
		return direction;
	}

	inline void SetDirection(quat pDirection)
	{
		direction = pDirection;
	}
};

