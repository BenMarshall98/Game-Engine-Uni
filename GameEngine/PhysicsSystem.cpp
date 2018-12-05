#include "PhysicsSystem.h"
#include "ComponentPosition.h"
#include "ComponentDirection.h"
#include "ComponentPhysics.h"


PhysicsSystem::PhysicsSystem(EntityManager & pEntityManager, PhysicsManager & pPhysicsManager) : entityManager(pEntityManager), physicsManager(pPhysicsManager)
{
	string componentTypes[] = { "ComponentPosition", "ComponentDirection", "ComponentPhysics" };
	EntityList = entityManager.GetAllEntitiesWithComponents(componentTypes, size(componentTypes));
	newEntities = EntityList;
}

void PhysicsSystem::Action(void)
{
	for (int i = 0; i < newEntities.size; i++)
	{
		iComponent * componentPosition = entityManager.GetComponentOfEntity(newEntities[i], "ComponentPosition");
		iComponent * componentDirection = entityManager.GetComponentOfEntity(newEntities[i], "ComponentDirection");
		iComponent * componentPhysics = entityManager.GetComponentOfEntity(newEntities[i], "ComponentPhysics");

		vec3 position = ((ComponentPosition *)componentPosition)->GetPosition();
		vec3 direction = ((ComponentDirection *)componentDirection)->GetDirection();
		float angle = ((ComponentDirection *)componentDirection)->GetAngle();
		CollisionShape* shape = ((ComponentPhysics *)componentPhysics)->GetShape();
		float mass = ((ComponentPhysics *)componentPhysics)->GetMass();

		void * rigidBody = physicsManager.AddRigidBody(mass, position, direction, angle, shape);

		((ComponentPhysics *)componentPhysics)->SetRigidBody(rigidBody);
	}

	newEntities.clear();

	physicsManager.Update(1 / 60.0);

	for (int i = 0; i < EntityList.size; i++)
	{
		iComponent * componentPosition = entityManager.GetComponentOfEntity(EntityList[i], "ComponentPosition");
		iComponent * componentDirection = entityManager.GetComponentOfEntity(EntityList[i], "ComponentDirection");
		iComponent * componentPhysics = entityManager.GetComponentOfEntity(EntityList[i], "ComponentPhysics");

		Motion(((ComponentPosition *)componentPosition), ((ComponentDirection *)componentDirection), ((ComponentPhysics *)componentPhysics));
	}
}

void PhysicsSystem::Motion(ComponentPosition * position, ComponentDirection * direction, ComponentPhysics * physics)
{
	void * rigidBody = physics->GetRigidBody();
	vec3 position = physicsManager.GetPositionOfRigidBody(rigidBody);
	quat directionQuat = physicsManager.GetDirectionOfRigidBody(rigidBody);

}

PhysicsSystem::~PhysicsSystem()
{
}
