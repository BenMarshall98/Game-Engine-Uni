#include "InputSystem.h"

#include "ComponentInput.h"
#include <string>



InputSystem::InputSystem(EntityManager & pEntityManager) : entityManager(pEntityManager)
{
	string componentTypes[] = { "ComponentInput" };
	EntityList = entityManager.GetAllEntitiesWithComponents(componentTypes, size(componentTypes));
}

void InputSystem::Action(void)
{
	for (int i = 0; i < )
}

InputSystem::~InputSystem()
{
}
