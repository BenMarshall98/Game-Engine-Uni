#pragma once

#include "RigidBody.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "CollisionShape.h"
#include "Entity.h"

class PhysicsEngine
{
public:
	PhysicsEngine();
	virtual ~PhysicsEngine();

	virtual RigidBody * AddRigidBody(float mass, glm::vec3 & position, glm::quat & direction, CollisionShape * shape, Entity * entity, bool collisionResponse, glm::vec3 & angularLimit) = 0;
	virtual void RemoveRigidBody(RigidBody * pRigidBody) = 0;

	virtual void Update(float pDeltaTime) = 0;

	inline glm::vec3 GetPositionOfRigidBody(RigidBody * pRigidBody)
	{
		return pRigidBody->GetPosition();
	}
	
	inline glm::quat GetDirectionOfRigidBody(RigidBody * pRigidBody)
	{
		return pRigidBody->GetDirection();
	}

	inline void ApplyVelocity(RigidBody * pRigidBody, glm::vec3 & velocity)
	{
		pRigidBody->ApplyVelocity(velocity);
	}

	inline void ApplyImpulse(RigidBody * pRigidBody, glm::vec3 & impulse)
	{
		pRigidBody->ApplyImpulse(impulse);
	}

	inline void ApplyRotation(RigidBody * pRigidBody, glm::vec3 & rotation)
	{
		pRigidBody->ApplyRotation(rotation);
	}
	
	virtual bool ClearBetweenPoints(glm::vec3 position1, glm::vec3 position2) = 0;

};

