#pragma once
#include "iComponent.h"
#include "glm/glm.hpp"

#include "glm/gtc/quaternion.hpp"

using namespace glm;

class ComponentDirection : public iComponent
{
private:
	quat currentDirection;
	quat newDirection;

public:
	ComponentDirection(vec3 pDirection, float pAngle) : currentDirection(angleAxis(pAngle, pDirection)), newDirection(angleAxis(pAngle, pDirection)) {}
	~ComponentDirection() {};
	
	ComponentType GetComponentName() override;

	void Swap() override;

	inline quat GetDirection() const
	{
		return currentDirection;
	}

	inline void SetDirection(quat pDirection)
	{
		newDirection = pDirection;
	}
};

