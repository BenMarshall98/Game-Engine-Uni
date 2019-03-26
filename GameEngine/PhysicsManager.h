#pragma once

#include "PhysicsEngine.h"
#include "RigidBody.h"
#include "CollisionShape.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "Entity.h"
#include "BulletPhysicsEngine.h"

class PhysicsManager
{
private:
	
	static std::string engineName;

	static PhysicsEngine * engine;

	static PhysicsManager * instance;

	PhysicsManager() {}

public:

	static inline PhysicsManager * Instance()
	{
		if (instance == nullptr)
		{
			instance = new PhysicsManager();
		}

		if (engine == nullptr)
		{
			if (engineName == "Bullet")
			{
				engine = new BulletPhysicsEngine;
			}
		}

		return instance;
	}

	static void SetPhysicsEngine(std::string pEngineName)
	{
		engineName = pEngineName;
	}

	PhysicsManager& operator= (const PhysicsManager&) = delete;
	PhysicsManager(PhysicsManager&) = delete;

	~PhysicsManager();

	inline RigidBody * AddRigidBody(const float mass, glm::vec3 & position, glm::quat & direction, CollisionShape * const shape, Entity * const entity, const bool collisionResponse, const glm::vec3 & angularLimit)
	{
		return engine->AddRigidBody(mass, position, direction, shape, entity, collisionResponse, angularLimit);
	}

	inline void Update(const float pDeltaTime) const
	{
		engine->Update(pDeltaTime);
	}

	inline void Clear()
	{
		delete engine;
		engine = nullptr;
	}

	inline glm::vec3 GetPositionOfRigidBody(RigidBody * const pRigidBody)
	{
		return engine->GetPositionOfRigidBody(pRigidBody);
	}

	inline glm::quat GetDirectionOfRigidBody(RigidBody * const pRigidBody)
	{
		return engine->GetDirectionOfRigidBody(pRigidBody);
	}

	inline void SetPositionOfRigidBody(RigidBody * const pRigidBody, const glm::vec3 & pPosition)
	{
		engine->SetPositionOfRigidBody(pRigidBody, pPosition);
	}

	inline void SetDirectionOfRigidBody(RigidBody * const pRigidBody, const glm::quat & pDirection)
	{
		engine->SetDirectionOfRigidBody(pRigidBody, pDirection);
	}

	inline void ApplyVelocity(RigidBody * const pRigidBody, const glm::vec3 & velocity) const
	{
		engine->ApplyVelocity(pRigidBody, velocity);
	}

	inline void ApplyImpulse(RigidBody * const pRigidBody, const glm::vec3 & impulse) const
	{
		engine->ApplyImpulse(pRigidBody, impulse);
	}

	inline void RemoveRigidBody(RigidBody * const pRigidBody)
	{
		engine->RemoveRigidBody(pRigidBody);
	}

	inline void ApplyRotation(RigidBody * const pRigidBody, const glm::vec3 & rotation) const
	{
		engine->ApplyRotation(pRigidBody, rotation);
	}

	inline bool ClearBetweenPoints(const glm::vec3 & position1, const glm::vec3 & position2)
	{
		return engine->ClearBetweenPoints(position1, position2);
	}
};

