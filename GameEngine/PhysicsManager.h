#pragma once

#include "BulletPhysicsEngine.h"
#include "CollisionShape.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

using namespace glm;

class PhysicsManager
{
private:
	BulletPhysicsEngine* engine;
public:
	PhysicsManager(BulletPhysicsEngine* pEngine) : engine(pEngine) {}
	~PhysicsManager() {}

	void* AddRigidBody(float mass, vec3 position, quat direction, CollisionShape * shape);

	inline void Update(float pDeltaTime)
	{
		engine->Update(pDeltaTime);
	}

	inline vec3 GetPositionOfRigidBody(void * pRigidBody)
	{
		return engine->GetPositionOfRigidBody(pRigidBody);
	}

	inline quat GetDirectionOfRigidBody(void * pRigidBody)
	{
		return engine->GetDirectionOfRigidBody(pRigidBody);
	}

	inline void ApplyForce(void * pRigidBody, vec3 force)
	{
		engine->ApplyForce(pRigidBody, force);
	}

	inline void ApplyImpulse(void * pRigidBody, vec3 impulse)
	{
		engine->ApplyImpulse(pRigidBody, impulse);
	}
};

