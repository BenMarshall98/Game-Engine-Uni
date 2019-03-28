#include "PhysicsManager.h"

std::string PhysicsManager::engineName = "Bullet";
glm::vec3 PhysicsManager::gravity = glm::vec3(0, -9.81, 0);
PhysicsManager * PhysicsManager::instance = nullptr;
PhysicsEngine * PhysicsManager::engine = nullptr;

PhysicsManager::~PhysicsManager()
{
	delete engine;
	instance = nullptr;
}
