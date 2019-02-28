#pragma once

#include "RigidBody.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "CollisionShape.h"
#include "Entity.h"

using namespace glm;

class PhysicsEngine
{
public:
	PhysicsEngine();
	virtual ~PhysicsEngine();

	virtual RigidBody * AddRigidBody(float mass, vec3 & position, quat & direction, CollisionShape * shape, Entity * entity, bool collisionResponse, vec3 & angularLimit) = 0;
	virtual void RemoveRigidBody(RigidBody * pRigidBody) = 0;

	virtual void Update(float pDeltaTime) = 0;

	inline vec3 GetPositionOfRigidBody(RigidBody * pRigidBody)
	{
		return pRigidBody->GetPosition();
	}
	
	inline quat GetDirectionOfRigidBody(RigidBody * pRigidBody)
	{
		return pRigidBody->GetDirection();
	}

	inline void ApplyVelocity(RigidBody * pRigidBody, vec3 & velocity)
	{
		pRigidBody->ApplyVelocity(velocity);
	}

	inline void ApplyImpulse(RigidBody * pRigidBody, vec3 & impulse)
	{
		pRigidBody->ApplyImpulse(impulse);
	}

	inline void ApplyRotation(RigidBody * pRigidBody, vec3 & rotation)
	{
		pRigidBody->ApplyRotation(rotation);
	}
	
	virtual bool ClearBetweenPoints(vec3 position1, vec3 position2) = 0;
};

