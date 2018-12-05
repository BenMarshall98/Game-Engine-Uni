#include "PhysicsManager.h"

void* PhysicsManager::AddRigidBody(float mass, vec3 position, vec3 direction, float angle, CollisionShape * shape)
{
	return engine->AddRigidBody(mass, position, direction, angle, shape);
}
