#pragma once
#include "iComponent.h"
#include "CollisionShape.h"
#include "Entity.h"
#include "glm/glm.hpp"
#include <map>
#include <string>
#include <vector>


using namespace glm;
using namespace std;

#define ENTITYTYPES(E) \
	E(NONE), \
	E(WALL), \
	E(PLAYER), \
	E(COLLECTABLE)

#define E(e) e
static enum class EntityType { ENTITYTYPES(E) };
#undef E

class ComponentPhysics : public iComponent
{
private:

#define E(s) #s
	static vector<string> EntityTypeNames;
#undef E

	map<Entity *, EntityType> unresolvedCollisions = map<Entity *, EntityType>();
	
	vec3 velocity = vec3(0);
	vec3 impulse = vec3(0);
	vec3 rotation = vec3(0);
	vec3 angularLimits = vec3(1);
	
	map<EntityType, string> * collisionFunctions;
	CollisionShape * shape;
	void * rigidBody;
	Entity * thisEntity;

	EntityType entityType;
	float mass;
	bool touchingGround = true;
	bool nextTouchingGround = false;
	bool collisionResponse;

public:
	ComponentPhysics(CollisionShape * const pShape, const float pMass, const EntityType pEntityType, Entity * const pThisEntity, const vec3 & pAngularLimits, const bool pCollisionResponse = true, map<EntityType, string> * const pCollisionFunctions = new map<EntityType, string>())
		: collisionFunctions(pCollisionFunctions), entityType(pEntityType), shape(pShape), mass(pMass), rigidBody(nullptr), angularLimits(pAngularLimits), thisEntity(pThisEntity), collisionResponse(pCollisionResponse) {}
	~ComponentPhysics();

	ComponentPhysics& operator=(const ComponentPhysics&) = delete;
	ComponentPhysics(ComponentPhysics&) = delete;

	ComponentType GetComponentName() override;

	static EntityType StringToEnum(const string & entityType);

	void AddCollision(Entity * physicsComponent, EntityType entityType);
	void ResolveCollisions();
	void GroundSwap()
	{
		touchingGround = nextTouchingGround;
		nextTouchingGround = false;
	}

	void RenderSwap() override;

	inline vec3 GetUpdateAngularLimits() const
	{
		return angularLimits;
	}

	inline bool GetUpdateCollisionResponse() const
	{
		return collisionResponse;
	}

	inline const vec3 & GetUpdateVelocity() const
	{
		return velocity;
	}

	inline void SetUpdateVelocity(vec3 & pVelocity)
	{
		velocity = pVelocity;
	}

	inline const vec3 & GetUpdateImpulse() const
	{
		return impulse;
	}

	inline void SetUpdateImpulse(vec3 & pImpulse)
	{
		impulse = pImpulse;
	}

	inline const vec3 & GetUpdateRotation() const
	{
		return rotation;
	}

	inline void SetUpdateRotation(vec3 & pRotation)
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

	inline void SetUpdateRigidBody(void * const pRigidBody)
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

	inline void SetUpdateTouchingGround(const bool pTouchingGround)
	{
		nextTouchingGround = nextTouchingGround || pTouchingGround;
	}
};

