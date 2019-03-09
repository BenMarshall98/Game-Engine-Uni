#include "PhysicsSystem.h"
#include "ComponentPosition.h"
#include "ComponentDirection.h"
#include "ComponentPhysics.h"
#include "RigidBody.h"
#include "glm/glm.hpp"

#include <algorithm>

PhysicsSystem::PhysicsSystem() : physicsManager(PhysicsManager::Instance()), entityManager(EntityManager::Instance())
{
	EntityList = entityManager->GetAllEntitiesWithComponents(componentTypes, std::size(componentTypes));
	newEntities = EntityList;
}

void PhysicsSystem::RemoveEntity(Entity * pEntity)
{
	std::vector<Entity *>::iterator it = find(EntityList.begin(), EntityList.end(), pEntity);

	if (it != EntityList.end())
	{
		EntityList.erase(it);
	}
}

void PhysicsSystem::AddEntity(Entity * pEntity)
{
	bool containsComponents = entityManager->CheckEntityHasComponents(pEntity, componentTypes, std::size(componentTypes));
	bool containsEntity = false;

	std::vector<Entity *>::iterator it = find(EntityList.begin(), EntityList.end(), pEntity);

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
	}
}

void PhysicsSystem::Action(void)
{
	for (int i = 0; i < newEntities.size(); i++)
	{
		iComponent * componentPosition = entityManager->GetComponentOfEntity(newEntities[i], ComponentType::COMPONENT_POSITION);
		iComponent * componentDirection = entityManager->GetComponentOfEntity(newEntities[i], ComponentType::COMPONENT_DIRECTION);
		iComponent * componentPhysics = entityManager->GetComponentOfEntity(newEntities[i], ComponentType::COMPONENT_PHYSICS);

		glm::vec3 position = ((ComponentPosition *)componentPosition)->GetUpdatePosition();
		glm::quat direction = ((ComponentDirection *)componentDirection)->GetUpdateDirection();
		CollisionShape* shape = ((ComponentPhysics *)componentPhysics)->GetUpdateShape();
		float mass = ((ComponentPhysics *)componentPhysics)->GetUpdateMass();
		bool collisionResponse = ((ComponentPhysics *)componentPhysics)->GetUpdateCollisionResponse();
		glm::vec3 angularLimit = ((ComponentPhysics *)componentPhysics)->GetUpdateAngularLimits();

		RigidBody * rigidBody = physicsManager->AddRigidBody(mass, position, direction, shape, newEntities[i], collisionResponse, angularLimit);

		((ComponentPhysics *)componentPhysics)->SetUpdateRigidBody(rigidBody);
	}

	newEntities.clear();

	physicsManager->Update(1 / 60.0);

	for (int i = 0; i < EntityList.size(); i++)
	{
		iComponent * componentPosition = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_POSITION);
		iComponent * componentDirection = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_DIRECTION);
		iComponent * componentPhysics = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_PHYSICS);

		Motion(((ComponentPosition *)componentPosition), ((ComponentDirection *)componentDirection), ((ComponentPhysics *)componentPhysics));
	}
}

void PhysicsSystem::Motion(ComponentPosition * position, ComponentDirection * direction, ComponentPhysics * physics)
{
	RigidBody * rigidBody = physics->GetUpdateRigidBody();
	glm::vec3 positionVec = physicsManager->GetPositionOfRigidBody(rigidBody);
	glm::quat directionQuat = physicsManager->GetDirectionOfRigidBody(rigidBody);

	glm::vec3 zero = glm::vec3(0);
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

	if (physics->GetUpdateMass() > 0)
	{
		physics->GroundSwap();
	}
}

PhysicsSystem::~PhysicsSystem()
{
}
