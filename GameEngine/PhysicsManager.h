#pragma once

#include "BulletPhysicsEngine.h"
#include "CollisionShape.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "Entity.h"

using namespace glm;

class PhysicsManager
{
private:
	
	BulletPhysicsEngine* engine;
public:

	PhysicsManager(BulletPhysicsEngine* pEngine) : engine(pEngine) {}

	PhysicsManager& operator= (const PhysicsManager&) = delete;
	PhysicsManager(PhysicsManager&) = delete;

	~PhysicsManager() {}

	void* AddRigidBody(float mass, vec3 position, quat direction, CollisionShape * shape, Entity * entity, bool collisionResponse);

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

	inline void RemoveRigidBody(void * pRigidBody)
	{
		engine->RemoveRigidBody(pRigidBody);
	}

	inline void ApplyRotation(void * pRigidBody, vec3 rotation)
	{
		engine->ApplyRotation(pRigidBody, rotation);
	}
};

