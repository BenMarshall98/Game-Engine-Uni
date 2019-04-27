#pragma once
#include "iComponent.h"
#include "CollisionShape.h"
#include "RigidBody.h"
#include "Entity.h"
#include "glm/glm.hpp"
#include <map>
#include <string>
#include <vector>

#define ENTITYTYPES(E) \
	E(NONE), \
	E(WALL), \
	E(PLAYER), \
	E(COLLECTABLE), \
	E(PBULLET), \
	E(EBULLET)

#define E(e) e
static enum class EntityType { ENTITYTYPES(E) };
#undef E

class ComponentPhysics : public iComponent
{
private:

#define E(s) #s
	static std::vector<std::string> EntityTypeNames;
#undef E

	std::map<Entity *, EntityType> unresolvedCollisions = std::map<Entity *, EntityType>();
	
	glm::vec3 velocity = glm::vec3(0);
	glm::vec3 impulse = glm::vec3(0);
	glm::vec3 rotation = glm::vec3(0);
	glm::vec3 angularLimits = glm::vec3(1);
	
	std::map<EntityType, std::string> * collisionFunctions;
	CollisionShape * shape;
	RigidBody * rigidBody;
	Entity * thisEntity;

	EntityType entityType;
	float mass;
	bool touchingGround = true;
	bool nextTouchingGround = false;
	bool collisionResponse;

public:
	ComponentPhysics(CollisionShape * const pShape, const float pMass, const EntityType pEntityType, Entity * const pThisEntity, const glm::vec3 & pAngularLimits, const bool pCollisionResponse = true, std::map<EntityType, std::string> * const pCollisionFunctions = new std::map<EntityType, std::string>())
		: iComponent(ComponentType::COMPONENT_PHYSICS), angularLimits(pAngularLimits), collisionFunctions(pCollisionFunctions), shape(pShape), rigidBody(nullptr), thisEntity(pThisEntity), entityType(pEntityType), mass(pMass),   collisionResponse(pCollisionResponse) {}
	~ComponentPhysics();

	ComponentPhysics& operator=(const ComponentPhysics&) = delete;
	ComponentPhysics(ComponentPhysics&) = delete;


	static EntityType StringToEnum(const std::string & entityType);

	void AddCollision(Entity * physicsComponent, EntityType entityType);
	void ResolveCollisions();

	//Keeps component concurrency complient
	void GroundSwap()
	{
		touchingGround = nextTouchingGround;
		nextTouchingGround = false;
	}

	void RenderSwap() override;

	//Gets the update thread angular limits
	inline const glm::vec3 & GetUpdateAngularLimits() const
	{
		return angularLimits;
	}

	//Gets the update thread collision response
	inline bool GetUpdateCollisionResponse() const
	{
		return collisionResponse;
	}

	//Gets the update thread velocity
	inline const glm::vec3 & GetUpdateVelocity() const
	{
		return velocity;
	}

	//Sets the update thread velocity
	inline void SetUpdateVelocity(const glm::vec3 & pVelocity)
	{
		velocity = pVelocity;
	}

	//Gets the update thread impulse
	inline const glm::vec3 & GetUpdateImpulse() const
	{
		return impulse;
	}

	//Sets the update thread impulse
	inline void SetUpdateImpulse(const glm::vec3 & pImpulse)
	{
		impulse = pImpulse;
	}

	//Gets the update thread rotation
	inline const glm::vec3 & GetUpdateRotation() const
	{
		return rotation;
	}

	//Sets the update thread rotation
	inline void SetUpdateRotation(const glm::vec3 & pRotation)
	{
		rotation = pRotation;
	}

	//Gets the update thread collision shape
	inline CollisionShape * GetUpdateShape() const
	{
		return shape;
	}

	//Gets the update thread mass
	inline float GetUpdateMass() const
	{
		return mass;
	}

	//Sets the update thread rigid body
	inline void SetUpdateRigidBody(RigidBody * const pRigidBody)
	{
		rigidBody = pRigidBody;
	}

	//Gets the update thread rigid body
	inline RigidBody * GetUpdateRigidBody() const
	{
		return rigidBody;
	}

	//Gets the update thread entity type
	inline EntityType GetUpdateEntityType() const
	{
		return entityType;
	}

	//Gets the update thread touching ground state
	inline bool GetUpdateTouchingGround() const
	{
  		return touchingGround;
	}

	//Sets the update thread touching ground state
	inline void SetUpdateTouchingGround(const bool pTouchingGround)
	{
		nextTouchingGround = nextTouchingGround || pTouchingGround;
	}
};

