#include "PhysicsManager.h"

void* PhysicsManager::AddRigidBody(float mass, vec3 position, quat direction, CollisionShape * shape, Entity * entity)
{
	return engine->AddRigidBody(mass, position, direction, shape, entity);
}
