#pragma once
#include "iComponent.h"
#include "CollisionShape.h"
#include "Entity.h"
#include "glm/glm.hpp"
#include <map>
#include <vector>
#include <algorithm>

using namespace glm;
using namespace std;

enum EntityType
{
	NONE,
	WALL,
	PLAYER,
	COLLECTABLE
};

class ComponentPhysics : public iComponent
{
private:
	typedef void(*GameCollisionFunction)(void);
	map<EntityType, GameCollisionFunction> collisionFunctions;
	map<Entity *, EntityType> unresolvedCollisions = map<Entity *, EntityType>();
	EntityType entityType;
	CollisionShape * shape;
	vec3 velocity = vec3(0);
	vec3 impulse = vec3(0);
	float rotation = 0;
	float mass;
	void* rigidBody;
	Entity * thisEntity;
	bool touchingGround = true;
	bool nextTouchingGround = false;

public:
	ComponentPhysics(CollisionShape * pShape, float pMass, EntityType pEntityType, Entity * pThisEntity, map<EntityType, GameCollisionFunction> pCollisionFunctions = map<EntityType, GameCollisionFunction>()) : shape(pShape), mass(pMass), entityType(pEntityType) , thisEntity(pThisEntity), collisionFunctions(pCollisionFunctions) {}
	~ComponentPhysics() {}

	ComponentType GetComponentName();
	void AddCollision(Entity * physicsComponent, EntityType entityType);
	void ResolveCollisions();
	void Swap()
	{
		touchingGround = nextTouchingGround;
		nextTouchingGround = false;
	}

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

	inline EntityType GetEntityType()
	{
		return entityType;
	}

	inline bool GetTouchingGround()
	{
  		return touchingGround;
	}

	inline void SetTouchingGround(bool pTouchingGround)
	{
		nextTouchingGround = nextTouchingGround || pTouchingGround;
	}
};

