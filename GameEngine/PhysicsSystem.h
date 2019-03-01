#pragma once

#include "Entity.h"
#include "EntityManager.h"
#include "PhysicsManager.h"
#include "iSystem.h"

#include "ComponentPosition.h"
#include "ComponentDirection.h"
#include "ComponentPhysics.h"

#include <vector>

class PhysicsSystem : public iSystem
{
private:
	std::vector<Entity *> EntityList;
	std::vector<Entity *> newEntities;
	EntityManager * entityManager;
	PhysicsManager * physicsManager;

public:
	explicit PhysicsSystem();

	PhysicsSystem& operator=(const PhysicsSystem&) = delete;
	PhysicsSystem(PhysicsSystem&) = delete;

	void Action(void) override;
	void RemoveEntity(Entity * pEntity) override;
	void Motion(ComponentPosition * position, ComponentDirection * direction, ComponentPhysics * physics);
	~PhysicsSystem();
};

