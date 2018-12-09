#pragma once
#include "iComponent.h"
#include "CollisionShape.h"
#include "glm/glm.hpp"
#include <map>
#include <vector>
#include <algorithm>

using namespace glm;
using namespace std;

template <typename E>
class ComponentPhysics : public iComponent
{
private:
	typedef void(*GameCollisionFunction)(void);
	map<E, GameCollisionFunction> collisionFunctions;
	map<ComponentPhysics *, E> unresolvedCollisions = map<ComponentPhysics *, E>();
	E entityType;
	CollisionShape * shape;
	vec3 velocity = vec3(0);
	vec3 impulse = vec3(0);
	float rotation = 0;
	float mass;
	void* rigidBody;

public:
	ComponentPhysics(CollisionShape * pShape, float pMass, E pEntityType, map<E, GameCollisionFunction> pCollisionFunctions = map<E, GameCollisionFunction>()) : shape(pShape), mass(pMass), entityType(pEntityType) {}
	~ComponentPhysics() {}

	ComponentType GetComponentName();
	void AddCollision(ComponentPhysics * physicsComponent, E entityType);
	void ResolveCollisions();
	void Swap() {}

	inline vec3 GetVelocity()
	{
		return velocity;
	}

	inline void SetVelocity(vec3 pVelocity)
	{
		velocity = pVelocity;
	}

	inline vec3 GetImpulse()
	{
		return impulse;
	}

	inline void SetImpulse(vec3 pImpulse)
	{
		impulse = pImpulse;
	}

	inline float GetRotation()
	{
		return rotation;
	}

	inline void SetRotation(float pRotation)
	{
		rotation = pRotation;
	}

	inline CollisionShape* GetShape()
	{
		return shape;
	}

	inline float GetMass()
	{
		return mass;
	}

	inline void SetRigidBody(void* pRigidBody)
	{
		rigidBody = pRigidBody;
	}

	inline void* GetRigidBody()
	{
		return rigidBody;
	}

	inline E GetEntityType()
	{
		return entityType;
	}
};

