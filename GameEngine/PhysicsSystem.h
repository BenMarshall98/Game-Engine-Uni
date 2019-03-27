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
	std::vector<Entity *> newEntities;
	PhysicsManager * physicsManager;
	void Motion(ComponentPosition * position, ComponentDirection * direction, ComponentPhysics * physics);

public:
	explicit PhysicsSystem();

	PhysicsSystem& operator=(const PhysicsSystem&) = delete;
	PhysicsSystem(PhysicsSystem&) = delete;

	void Action(float timePeriod) override;
	void AddEntity(Entity * pEntity) override;
	
	~PhysicsSystem();
};

