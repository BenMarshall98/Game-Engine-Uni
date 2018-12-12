#include "ComponentPhysics.h"

ComponentType ComponentPhysics::GetComponentName()
{
	return ComponentType::COMPONENT_PHYSICS;
}

void ComponentPhysics::AddCollision(Entity * physicsComponent, EntityType entityType)
{
	map<Entity *, EntityType>::iterator it;
	it = unresolvedCollisions.find(physicsComponent);

	if (it == unresolvedCollisions.end())
	{
		unresolvedCollisions.insert(pair<Entity *, EntityType>(physicsComponent, entityType));
	}
}

void ComponentPhysics::ResolveCollisions()
{
	 map<Entity *, EntityType>::iterator it;

	for (it = unresolvedCollisions.begin(); it != unresolvedCollisions.end(); it++)
	{
		EntityType entityType = it->second;

		map<EntityType, GameCollisionFunction>::iterator collision = collisionFunctions->find(entityType);
		
		if (collision != collisionFunctions->end())
		{
			GameCollisionFunction function = collision->second;
			function(thisEntity);
		}
	}

	unresolvedCollisions.clear();
}