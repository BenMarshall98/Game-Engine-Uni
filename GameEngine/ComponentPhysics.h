#pragma once
#include "iComponent.h"
#include "glm/glm.hpp"

using namespace glm;

class ComponentPhysics : public iComponent
{
private:
	vec3 force = vec3(0);
	float rotation = 0;
public:
	ComponentPhysics();
	~ComponentPhysics();

	string GetComponentName();
	void Swap() {}

	inline vec3 GetForce()
	{
		return force;
	}

	inline void SetForce(vec3 pForce)
	{
		force = pForce;
	}

	inline float GetRotation()
	{
		return rotation;
	}

	inline void SetRotation(float pRotation)
	{
		rotation = pRotation;
	}
};

