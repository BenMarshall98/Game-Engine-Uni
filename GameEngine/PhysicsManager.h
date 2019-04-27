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
	static glm::vec3 gravity;
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

	//Sets the physics engine to be used
	static void SetPhysicsEngine(std::string pEngineName)
	{
		engineName = pEngineName;
	}

	PhysicsManager& operator= (const PhysicsManager&) = delete;
	PhysicsManager(PhysicsManager&) = delete;

	~PhysicsManager();

	//Gets the gravity of the physics engine
	inline static glm::vec3 GetGravity()
	{
		return gravity;
	}

	//Sets the gravity of the physics engine
	inline static void SetGravity(glm::vec3 pGravity)
	{
		gravity = pGravity;
	}

	//Adds rigid body to the physics engine
	inline RigidBody * AddRigidBody(const float mass, glm::vec3 & position, glm::quat & direction, CollisionShape * const shape, Entity * const entity, const bool collisionResponse, const glm::vec3 & angularLimit)
	{
		return engine->AddRigidBody(mass, position, direction, shape, entity, collisionResponse, angularLimit);
	}

	//Steps the simulation of the physics engine
	inline void Update(const float pDeltaTime) const
	{
		engine->Update(pDeltaTime);
	}

	//Delete the physics engine
	inline void Clear()
	{
		delete engine;
		engine = nullptr;
	}

	//Gets the position of the rigid body
	inline glm::vec3 GetPositionOfRigidBody(RigidBody * const pRigidBody)
	{
		return engine->GetPositionOfRigidBody(pRigidBody);
	}

	//Gets the direction of the rigid body
	inline glm::quat GetDirectionOfRigidBody(RigidBody * const pRigidBody)
	{
		return engine->GetDirectionOfRigidBody(pRigidBody);
	}

	//Sets the position of the rigid body
	inline void SetPositionOfRigidBody(RigidBody * const pRigidBody, const glm::vec3 & pPosition)
	{
		engine->SetPositionOfRigidBody(pRigidBody, pPosition);
	}

	//Sets the direction of the rigid body
	inline void SetDirectionOfRigidBody(RigidBody * const pRigidBody, const glm::quat & pDirection)
	{
		engine->SetDirectionOfRigidBody(pRigidBody, pDirection);
	}

	//Applies velocity to the rigid body
	inline void ApplyVelocity(RigidBody * const pRigidBody, const glm::vec3 & velocity) const
	{
		engine->ApplyVelocity(pRigidBody, velocity);
	}

	//Applies impulse to the rigid body
	inline void ApplyImpulse(RigidBody * const pRigidBody, const glm::vec3 & impulse) const
	{
		engine->ApplyImpulse(pRigidBody, impulse);
	}

	//Removes rigid body 
	inline void RemoveRigidBody(RigidBody * const pRigidBody)
	{
		engine->RemoveRigidBody(pRigidBody);
	}

	//Applies rotation to the rigid body
	inline void ApplyRotation(RigidBody * const pRigidBody, const glm::vec3 & rotation) const
	{
		engine->ApplyRotation(pRigidBody, rotation);
	}

	//Checks if ray can be drawn between two points
	inline bool ClearBetweenPoints(const glm::vec3 & position1, const glm::vec3 & position2)
	{
		return engine->ClearBetweenPoints(position1, position2);
	}
};

