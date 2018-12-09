#include "PhysicsManager.h"

template <typename E>
void* PhysicsManager<E>::AddRigidBody(float mass, vec3 position, quat direction, CollisionShape * shape)
{
	return engine->AddRigidBody(mass, position, direction, shape);
}
