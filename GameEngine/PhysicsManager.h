#pragma once

#include "PhysicsEngine.h"
#include "RigidBody.h"
#include "CollisionShape.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "Entity.h"

using namespace glm;

class PhysicsManager
{
private:
	
	PhysicsEngine* engine;

	static PhysicsManager * instance;

	PhysicsManager() : engine(nullptr) {}

public:

	static inline PhysicsManager * Instance()
	{
		if (instance == nullptr)
		{
			instance = new PhysicsManager();
		}

		return instance;
	}

	inline void SetPhysicsEngine(PhysicsEngine * physicsEngine)
	{
		delete engine;
		engine = physicsEngine;
	}

	PhysicsManager& operator= (const PhysicsManager&) = delete;
	PhysicsManager(PhysicsManager&) = delete;

	~PhysicsManager() {}

	inline RigidBody * AddRigidBody(float mass, vec3 & position, quat & direction, CollisionShape * shape, Entity * entity, bool collisionResponse, vec3 & angularLimit)
	{
		return engine->AddRigidBody(mass, position, direction, shape, entity, collisionResponse, angularLimit);
	}

	inline void Update(float pDeltaTime) const
	{
		engine->Update(pDeltaTime);
	}

	inline vec3 GetPositionOfRigidBody(RigidBody * pRigidBody)
	{
		return engine->GetPositionOfRigidBody(pRigidBody);
	}

	inline quat GetDirectionOfRigidBody(RigidBody * pRigidBody)
	{
		return engine->GetDirectionOfRigidBody(pRigidBody);
	}

	inline void ApplyVelocity(RigidBody * pRigidBody, vec3 & velocity) const
	{
		engine->ApplyVelocity(pRigidBody, velocity);
	}

	inline void ApplyImpulse(RigidBody * pRigidBody, vec3 & impulse) const
	{
		engine->ApplyImpulse(pRigidBody, impulse);
	}

	inline void RemoveRigidBody(RigidBody * pRigidBody)
	{
		engine->RemoveRigidBody(pRigidBody);
	}

	inline void ApplyRotation(RigidBody * pRigidBody, vec3 & rotation) const
	{
		engine->ApplyRotation(pRigidBody, rotation);
	}

	inline bool ClearBetweenPoints(vec3 position1, vec3 position2)
	{
		return engine->ClearBetweenPoints(position1, position2);
	}
};

