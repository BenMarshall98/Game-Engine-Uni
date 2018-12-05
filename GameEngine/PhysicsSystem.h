#pragma once

#include "Entity.h"
#include "EntityManager.h"
#include "PhysicsManager.h"
#include "iSystem.h"

#include "ComponentPosition.h"
#include "ComponentDirection.h"
#include "ComponentPhysics.h"

#include <vector>

using namespace std;

class PhysicsSystem : public iSystem
{
private:
	vector<Entity *> EntityList;
	vector<Entity *> newEntities;
	EntityManager & entityManager;
	PhysicsManager & physicsManager;

public:
	PhysicsSystem(EntityManager & pEntityManager, PhysicsManager & pPhysicsManager);
	void Action(void);
	void Motion(ComponentPosition * position, ComponentDirection * direction, ComponentPhysics * physics);
	~PhysicsSystem();
};

