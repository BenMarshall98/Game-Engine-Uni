#include "PhysicsSystem.h"
#include "ComponentPosition.h"
#include "ComponentDirection.h"
#include "ComponentPhysics.h"
#include "glm/glm.hpp"

#include <algorithm>

using namespace glm;


PhysicsSystem::PhysicsSystem(PhysicsManager & pPhysicsManager) : physicsManager(pPhysicsManager)
{
	entityManager = EntityManager::Instance();
	ComponentType componentTypes[] = { COMPONENT_POSITION, COMPONENT_DIRECTION, COMPONENT_PHYSICS };
	EntityList = entityManager->GetAllEntitiesWithComponents(componentTypes, size(componentTypes));
	newEntities = EntityList;
}

void PhysicsSystem::RemoveEntity(Entity * pEntity)
{
	iComponent * componentPhysics = entityManager->GetComponentOfEntity(pEntity, COMPONENT_PHYSICS);
	void * rigidBody = ((ComponentPhysics *)componentPhysics)->GetUpdateRigidBody();
	physicsManager.RemoveRigidBody(rigidBody);

	vector<Entity *>::iterator it = find(EntityList.begin(), EntityList.end(), pEntity);

	if (it != EntityList.end())
	{
		EntityList.erase(it);
	}
}

void PhysicsSystem::Action(void)
{
	for (int i = 0; i < newEntities.size(); i++)
	{
		iComponent * componentPosition = entityManager->GetComponentOfEntity(newEntities[i], COMPONENT_POSITION);
		iComponent * componentDirection = entityManager->GetComponentOfEntity(newEntities[i], COMPONENT_DIRECTION);
		iComponent * componentPhysics = entityManager->GetComponentOfEntity(newEntities[i], COMPONENT_PHYSICS);

		vec3 position = ((ComponentPosition *)componentPosition)->GetUpdatePosition();
		quat direction = ((ComponentDirection *)componentDirection)->GetUpdateDirection();
		CollisionShape* shape = ((ComponentPhysics *)componentPhysics)->GetUpdateShape();
		float mass = ((ComponentPhysics *)componentPhysics)->GetUpdateMass();
		bool collisionResponse = ((ComponentPhysics *)componentPhysics)->GetUpdateCollisionResponse();

		void * rigidBody = physicsManager.AddRigidBody(mass, position, direction, shape, newEntities[i], collisionResponse);

		((ComponentPhysics *)componentPhysics)->SetUpdateRigidBody(rigidBody);
	}

	newEntities.clear();

	physicsManager.Update(1 / 60.0);

	for (int i = 0; i < EntityList.size(); i++)
	{
		iComponent * componentPosition = entityManager->GetComponentOfEntity(EntityList[i], COMPONENT_POSITION);
		iComponent * componentDirection = entityManager->GetComponentOfEntity(EntityList[i], COMPONENT_DIRECTION);
		iComponent * componentPhysics = entityManager->GetComponentOfEntity(EntityList[i], COMPONENT_PHYSICS);

		Motion(((ComponentPosition *)componentPosition), ((ComponentDirection *)componentDirection), ((ComponentPhysics *)componentPhysics));
	}
}

void PhysicsSystem::Motion(ComponentPosition * position, ComponentDirection * direction, ComponentPhysics * physics)
{
	void * rigidBody = physics->GetUpdateRigidBody();
	vec3 positionVec = physicsManager.GetPositionOfRigidBody(rigidBody);
	quat directionQuat = physicsManager.GetDirectionOfRigidBody(rigidBody);

	vec3 velocity = physics->GetUpdateVelocity();
	physicsManager.ApplyVelocity(rigidBody, velocity);
	physics->SetUpdateVelocity(vec3(0));

	vec3 impulse = physics->GetUpdateImpulse();
	physicsManager.ApplyImpulse(rigidBody, impulse);
	physics->SetUpdateImpulse(vec3(0));

	position->SetUpdatePosition(positionVec);
	direction->SetUpdateDirection(directionQuat);

	physics->ResolveCollisions();

	if (physics->GetUpdateMass() > 0)
	{
		physics->GroundSwap();
	}
}

PhysicsSystem::~PhysicsSystem()
{
}
