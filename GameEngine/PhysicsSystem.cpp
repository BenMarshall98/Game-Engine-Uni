#include "PhysicsSystem.h"
#include "ComponentPosition.h"
#include "ComponentDirection.h"
#include "ComponentPhysics.h"
#include "glm/glm.hpp"

#include <algorithm>

using namespace glm;


PhysicsSystem::PhysicsSystem(EntityManager & pEntityManager, PhysicsManager & pPhysicsManager) : entityManager(pEntityManager), physicsManager(pPhysicsManager)
{
	ComponentType componentTypes[] = { COMPONENT_POSITION, COMPONENT_DIRECTION, COMPONENT_PHYSICS };
	EntityList = entityManager.GetAllEntitiesWithComponents(componentTypes, size(componentTypes));
	newEntities = EntityList;
}

void PhysicsSystem::RemoveEntity(Entity * pEntity)
{
	iComponent * componentPhysics = entityManager.GetComponentOfEntity(pEntity, COMPONENT_PHYSICS);
	void * rigidBody = ((ComponentPhysics *)componentPhysics)->GetRigidBody();
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
		iComponent * componentPosition = entityManager.GetComponentOfEntity(newEntities[i], COMPONENT_POSITION);
		iComponent * componentDirection = entityManager.GetComponentOfEntity(newEntities[i], COMPONENT_DIRECTION);
		iComponent * componentPhysics = entityManager.GetComponentOfEntity(newEntities[i], COMPONENT_PHYSICS);

		vec3 position = ((ComponentPosition *)componentPosition)->GetPosition();
		quat direction = ((ComponentDirection *)componentDirection)->GetDirection();
		CollisionShape* shape = ((ComponentPhysics *)componentPhysics)->GetShape();
		float mass = ((ComponentPhysics *)componentPhysics)->GetMass();
		bool collisionResponse = ((ComponentPhysics *)componentPhysics)->GetCollisionResponse();

		void * rigidBody = physicsManager.AddRigidBody(mass, position, direction, shape, newEntities[i], collisionResponse);

		((ComponentPhysics *)componentPhysics)->SetRigidBody(rigidBody);
	}

	newEntities.clear();

	physicsManager.Update(1 / 60.0);

	for (int i = 0; i < EntityList.size(); i++)
	{
		iComponent * componentPosition = entityManager.GetComponentOfEntity(EntityList[i], COMPONENT_POSITION);
		iComponent * componentDirection = entityManager.GetComponentOfEntity(EntityList[i], COMPONENT_DIRECTION);
		iComponent * componentPhysics = entityManager.GetComponentOfEntity(EntityList[i], COMPONENT_PHYSICS);

		Motion(((ComponentPosition *)componentPosition), ((ComponentDirection *)componentDirection), ((ComponentPhysics *)componentPhysics));
	}
}

void PhysicsSystem::Motion(ComponentPosition * position, ComponentDirection * direction, ComponentPhysics * physics)
{
	void * rigidBody = physics->GetRigidBody();
	vec3 positionVec = physicsManager.GetPositionOfRigidBody(rigidBody);
	quat directionQuat = physicsManager.GetDirectionOfRigidBody(rigidBody);

	vec3 velocity = physics->GetVelocity();
	physicsManager.ApplyVelocity(rigidBody, velocity);
	physics->SetVelocity(vec3(0));

	vec3 impulse = physics->GetImpulse();
	physicsManager.ApplyImpulse(rigidBody, impulse);
	physics->SetImpulse(vec3(0));

	position->SetPosition(positionVec);
	direction->SetDirection(directionQuat);

	physics->ResolveCollisions();

	if (physics->GetMass() != 0)
	{
		physics->Swap();
	}
}

PhysicsSystem::~PhysicsSystem()
{
}
