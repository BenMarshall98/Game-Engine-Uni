#pragma once
#include "iComponent.h"
#include "glm/glm.hpp"

using namespace glm;

class ComponentDirection : public iComponent
{
private:
	vec3 direction;
	float angle;

public:
	ComponentDirection(vec3 pDirection, float pAngle) : direction(pDirection), angle(pAngle) {}
	~ComponentDirection() {};

	string GetComponentName();
	void Swap() {}

	inline vec3 GetDirection()
	{
		return direction;
	}

	inline void SetDirection(vec3 pDirection)
	{
		direction = pDirection;
	}

	inline float GetAngle()
	{
		return angle;
	}

	inline void SetAngle(float pAngle)
	{
		angle = pAngle;
	}
};

