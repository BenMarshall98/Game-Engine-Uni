#include "PhysicsManager.h"


PhysicsManager * PhysicsManager::instance = nullptr;

PhysicsManager::~PhysicsManager()
{
	delete engine;
	instance = nullptr;
}
