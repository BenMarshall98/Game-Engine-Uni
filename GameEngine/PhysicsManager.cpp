#include "PhysicsManager.h"

void* PhysicsManager::AddRigidBody(float mass, vec3 position, quat direction, CollisionShape * shape)
{
	return engine->AddRigidBody(mass, position, direction, shape);
}
