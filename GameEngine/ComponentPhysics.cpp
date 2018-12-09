#include "ComponentPhysics.h"

template <typename E>
ComponentType ComponentPhysics<E>::GetComponentName()
{
	return ComponentType::COMPONENT_PHYSICS;
}

template <typename E>
void ComponentPhysics<E>::AddCollision(ComponentPhysics * physicsComponent, E entityType)
{
	typename map<ComponentPhysics *, E>::iterator it;
	it = unresolvedCollisions.find(physicsComponent);

	if (it == unresolvedCollision.end())
	{
		unresolvedCollision.insert(pair<ComponentPhysics *, E>(physicsComponent, entityType));
	}
}

template <typename E>
void ComponentPhysics<E>::ResolveCollisions()
{
	typename map<ComponentPhysics *, E>::iterator it;

	for (it = unresolvedCollision.begin(); it != unsolvedCollision.end(); it++)
	{
		E entityType = it->second;

		typename map<E, GameCollisionFunction>::iterator collision = collisionFunctions.find(entityType);
		
		if (collision != collisionFunctions.end())
		{
			GameCollisionFunction function = collision->second;
			function();
		}
	}
}