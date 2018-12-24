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
	void GroundSwap()
	{
		touchingGround = nextTouchingGround;
		nextTouchingGround = false;
	}

	void RenderSwap() override {};

	inline bool GetUpdateCollisionResponse() const
	{
		return collisionResponse;
	}

	inline vec3 GetUpdateVelocity() const
	{
		return velocity;
	}

	inline void SetUpdateVelocity(vec3 pVelocity)
	{
		velocity = pVelocity;
	}

	inline vec3 GetUpdateImpulse() const
	{
		return impulse;
	}

	inline void SetUpdateImpulse(vec3 pImpulse)
	{
		impulse = pImpulse;
	}

	inline float GetUpdateRotation() const
	{
		return rotation;
	}

	inline void SetUpdateRotation(float pRotation)
	{
		rotation = pRotation;
	}

	inline CollisionShape* GetUpdateShape() const
	{
		return shape;
	}

	inline float GetUpdateMass() const
	{
		return mass;
	}

	inline void SetUpdateRigidBody(void* pRigidBody)
	{
		rigidBody = pRigidBody;
	}

	inline void* GetUpdateRigidBody() const
	{
		return rigidBody;
	}

	inline EntityType GetUpdateEntityType() const
	{
		return entityType;
	}

	inline bool GetUpdateTouchingGround() const
	{
  		return touchingGround;
	}

	inline void SetUpdateTouchingGround(bool pTouchingGround)
	{
		nextTouchingGround = nextTouchingGround || pTouchingGround;
	}
};

