#pragma once

#include "BulletPhysicsEngine.h"
#include "CollisionShape.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

using namespace glm;

template <typename E>
class PhysicsManager
{
private:
	
	BulletPhysicsEngine<E>* engine;
public:

	PhysicsManager(BulletPhysicsEngine<E>* pEngine) : engine(pEngine) {}
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

	inline void ApplyVelocity(void * pRigidBody, vec3 velocity)
	{
		engine->ApplyVelocity(pRigidBody, velocity);
	}

	inline void ApplyImpulse(void * pRigidBody, vec3 impulse)
	{
		engine->ApplyImpulse(pRigidBody, impulse);
	}
};

