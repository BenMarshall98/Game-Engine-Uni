#pragma once

#include "glm/glm.hpp"

class RigidBody
{
protected:
	RigidBody();

public:
	virtual ~RigidBody();

	virtual glm::vec3 GetPosition() = 0;
	virtual glm::quat GetDirection() = 0;
	virtual void * GetRigidBody() = 0;

	virtual void SetPosition(const glm::vec3 & position) = 0;
	virtual void SetDirection(const glm::quat & direction) = 0;

	virtual void ApplyVelocity(const glm::vec3 & velocity) = 0;
	virtual void ApplyImpulse(const glm::vec3 & impulse) = 0;
	virtual void ApplyRotation(const glm::vec3 & rotation) = 0;

	
};

