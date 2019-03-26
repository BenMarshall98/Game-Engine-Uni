#include "PhysicsManager.h"

std::string PhysicsManager::engineName = "Bullet";

PhysicsManager * PhysicsManager::instance = nullptr;
PhysicsEngine * PhysicsManager::engine = nullptr;

PhysicsManager::~PhysicsManager()
{
	delete engine;
	instance = nullptr;
}
