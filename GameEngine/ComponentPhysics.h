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

typedef void(*GameCollisionFunction)(Entity *);

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
	map<EntityType, GameCollisionFunction> * collisionFunctions;
	map<Entity *, EntityType> unresolvedCollisions = map<Entity *, EntityType>();
	EntityType entityType;
	CollisionShape * shape;
	vec3 velocity = vec3(0);
	vec3 impulse = vec3(0);
	float rotation = 0;
	float mass;
	void * rigidBody;
	Entity * thisEntity;
	bool touchingGround = true;
	bool nextTouchingGround = false;
	bool collisionResponse;

public:
	ComponentPhysics(CollisionShape * pShape, float pMass, EntityType pEntityType, Entity * pThisEntity, bool pCollisionResponse = true, map<EntityType, GameCollisionFunction> * pCollisionFunctions = new map<EntityType, GameCollisionFunction>())
		: collisionFunctions(pCollisionFunctions), entityType(pEntityType), shape(pShape), mass(pMass), rigidBody(nullptr), thisEntity(pThisEntity), collisionResponse(pCollisionResponse) {}
	~ComponentPhysics() {}

	ComponentType GetComponentName() override;
	void AddCollision(Entity * physicsComponent, EntityType entityType);
	void ResolveCollisions();
	void Swap() override
	{
		touchingGround = nextTouchingGround;
		nextTouchingGround = false;
	}

	inline bool GetCollisionResponse() const
	{
		return collisionResponse;
	}

	inline vec3 GetVelocity() const
	{
		return velocity;
	}

	inline void SetVelocity(vec3 pVelocity)
	{
		velocity = pVelocity;
	}

	inline vec3 GetImpulse() const
	{
		return impulse;
	}

	inline void SetImpulse(vec3 pImpulse)
	{
		impulse = pImpulse;
	}

	inline float GetRotation() const
	{
		return rotation;
	}

	inline void SetRotation(float pRotation)
	{
		rotation = pRotation;
	}

	inline CollisionShape* GetShape() const
	{
		return shape;
	}

	inline float GetMass() const
	{
		return mass;
	}

	inline void SetRigidBody(void* pRigidBody)
	{
		rigidBody = pRigidBody;
	}

	inline void* GetRigidBody() const
	{
		return rigidBody;
	}

	inline EntityType GetEntityType() const
	{
		return entityType;
	}

	inline bool GetTouchingGround() const
	{
  		return touchingGround;
	}

	inline void SetTouchingGround(bool pTouchingGround)
	{
		nextTouchingGround = nextTouchingGround || pTouchingGround;
	}
};

