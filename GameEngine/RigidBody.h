#pragma once

#include "glm/glm.hpp"

using namespace glm;

class RigidBody
{
protected:
	RigidBody();

public:
	virtual ~RigidBody();

	virtual vec3 GetPosition() = 0;
	virtual quat GetDirection() = 0;
	virtual void * GetRigidBody() = 0;

	virtual void ApplyVelocity(vec3 & velocity) = 0;
	virtual void ApplyImpulse(vec3 & impulse) = 0;
	virtual void ApplyRotation(vec3 & rotation) = 0;
};

