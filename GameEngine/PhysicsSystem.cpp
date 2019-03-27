#include "PhysicsSystem.h"
#include "ComponentPosition.h"
#include "ComponentDirection.h"
#include "ComponentPhysics.h"
#include "RigidBody.h"
#include "glm/glm.hpp"

#include <algorithm>

PhysicsSystem::PhysicsSystem() : iSystem(std::vector<ComponentType>{
	ComponentType::COMPONENT_POSITION,
	ComponentType::COMPONENT_DIRECTION,
	ComponentType::COMPONENT_PHYSICS
}), physicsManager(PhysicsManager::Instance())
{
	newEntities = EntityList;
}

void PhysicsSystem::AddEntity(Entity * const pEntity)
{
	EntityManager * const entityManager = EntityManager::Instance();
	const bool containsComponents = entityManager->CheckEntityHasComponents(pEntity, componentTypes);
	bool containsEntity = false;

	const std::vector<Entity *>::iterator it = find(EntityList.begin(), EntityList.end(), pEntity);

	if (it != EntityList.end())
	{
		containsEntity = true;
	}

	if (containsEntity && !containsComponents)
	{
		EntityList.erase(it);
	}
	else if (!containsEntity && containsComponents)
	{
		EntityList.push_back(pEntity);
		newEntities.push_back(pEntity);
	}
}

void PhysicsSystem::Action(float timePeriod)
{
	EntityManager * const entityManager = EntityManager::Instance();
	for (int i = 0; i < newEntities.size(); i++)
	{
		iComponent * const componentPosition = entityManager->GetComponentOfEntity(newEntities[i], ComponentType::COMPONENT_POSITION);
		iComponent * const componentDirection = entityManager->GetComponentOfEntity(newEntities[i], ComponentType::COMPONENT_DIRECTION);
		iComponent * const componentPhysics = entityManager->GetComponentOfEntity(newEntities[i], ComponentType::COMPONENT_PHYSICS);

		glm::vec3 position = ((ComponentPosition *)componentPosition)->GetUpdatePosition();
		glm::quat direction = ((ComponentDirection *)componentDirection)->GetUpdateDirection();
		CollisionShape* const shape = ((ComponentPhysics *)componentPhysics)->GetUpdateShape();
		const float mass = ((ComponentPhysics *)componentPhysics)->GetUpdateMass();
		const bool collisionResponse = ((ComponentPhysics *)componentPhysics)->GetUpdateCollisionResponse();
		const glm::vec3 angularLimit = ((ComponentPhysics *)componentPhysics)->GetUpdateAngularLimits();

		RigidBody * const rigidBody = physicsManager->AddRigidBody(mass, position, direction, shape, newEntities[i], collisionResponse, angularLimit);

		((ComponentPhysics *)componentPhysics)->SetUpdateRigidBody(rigidBody);
	}

	newEntities.clear();

	physicsManager->Update(timePeriod);

	for (int i = 0; i < EntityList.size(); i++)
	{
		iComponent * const componentPosition = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_POSITION);
		iComponent * const componentDirection = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_DIRECTION);
		iComponent * const componentPhysics = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_PHYSICS);

		Motion(((ComponentPosition *)componentPosition), ((ComponentDirection *)componentDirection), ((ComponentPhysics *)componentPhysics));
	}
}

void PhysicsSystem::Motion(ComponentPosition * const position, ComponentDirection * const direction, ComponentPhysics * const physics)
{
	RigidBody * const rigidBody = physics->GetUpdateRigidBody();

	if (physics->GetUpdateMass() != 0)
	{
		const glm::vec3 positionVec = physicsManager->GetPositionOfRigidBody(rigidBody);
		const glm::quat directionQuat = physicsManager->GetDirectionOfRigidBody(rigidBody);

		const glm::vec3 zero = glm::vec3(0);
		glm::vec3 velocity = physics->GetUpdateVelocity();
		physicsManager->ApplyVelocity(rigidBody, velocity);
		physics->SetUpdateVelocity(zero);

		glm::vec3 impulse = physics->GetUpdateImpulse();
		physicsManager->ApplyImpulse(rigidBody, impulse);
		physics->SetUpdateImpulse(zero);

		glm::vec3 rotation = physics->GetUpdateRotation();
		physicsManager->ApplyRotation(rigidBody, rotation);
		physics->SetUpdateRotation(zero);

		position->SetUpdatePosition(positionVec);
		direction->SetUpdateDirection(directionQuat);

		physics->ResolveCollisions();

		physics->GroundSwap();
	}
	else
	{
		const glm::vec3 positionVec = position->GetUpdatePosition();
		const glm::quat directionQuat = direction->GetUpdateDirection();

		physicsManager->SetPositionOfRigidBody(rigidBody, positionVec);
		physicsManager->SetDirectionOfRigidBody(rigidBody, directionQuat);

		physics->ResolveCollisions();
	}
}

PhysicsSystem::~PhysicsSystem()
{
}
