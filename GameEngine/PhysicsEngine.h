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

	virtual RigidBody * AddRigidBody(const float mass, glm::vec3 & position, glm::quat & direction, CollisionShape * const shape, Entity * const entity, bool collisionResponse, const glm::vec3 & angularLimit) = 0;
	virtual void RemoveRigidBody(RigidBody * pRigidBody) = 0;

	virtual void Update(float pDeltaTime) = 0;

	//Gets the position of the rigid body
	inline glm::vec3 GetPositionOfRigidBody(RigidBody * const pRigidBody)
	{
		return pRigidBody->GetPosition();
	}
	
	//Gets the direction of the rigid body
	inline glm::quat GetDirectionOfRigidBody(RigidBody * const pRigidBody)
	{
		return pRigidBody->GetDirection();
	}

	//Sets the position of the rigid body
	inline void SetPositionOfRigidBody(RigidBody * const pRigidBody, const glm::vec3 & pPosition)
	{
		pRigidBody->SetPosition(pPosition);
	}

	//Sets the direction of the rigid body
	inline void SetDirectionOfRigidBody(RigidBody * const pRigidBody, const glm::quat & pDirection)
	{
		pRigidBody->SetDirection(pDirection);
	}

	//Apply velocity to the rigid body
	inline void ApplyVelocity(RigidBody * const pRigidBody, const glm::vec3 & velocity)
	{
		pRigidBody->ApplyVelocity(velocity);
	}

	//Apply impulse to the rigid body
	inline void ApplyImpulse(RigidBody * const pRigidBody, const glm::vec3 & impulse)
	{
		pRigidBody->ApplyImpulse(impulse);
	}

	//Apply rotation to the rigid body
	inline void ApplyRotation(RigidBody * const pRigidBody, const glm::vec3 & rotation)
	{
		pRigidBody->ApplyRotation(rotation);
	}
	
	virtual bool ClearBetweenPoints(const glm::vec3 & position1, const glm::vec3 & position2) = 0;

};

