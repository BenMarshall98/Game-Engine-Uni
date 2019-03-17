#include "InputSystem.h"
#include "InputManager.h"
#include "ComponentInput.h"

#include <string>
#include <algorithm>

InputSystem::InputSystem() : iSystem(std::vector<ComponentType>{
	ComponentType::COMPONENT_INPUT
})
{
}

void InputSystem::Action(void)
{
	EntityManager * entityManager = EntityManager::Instance();
	for (int i = 0; i < EntityList.size(); i++)
	{
		iComponent * const componentInput = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_INPUT);

		std::vector<InputFunction *> * const inputs = dynamic_cast<ComponentInput *>(componentInput)->GetUpdateInputs();

		Input(inputs, EntityList[i]);
	}
}

void InputSystem::Input(std::vector<InputFunction *> * const inputs, Entity * const entity)
{	
	for (int i = 0; i < inputs->size(); i++)
	{
		inputs->at(i)->Update(entity);
	}
}

InputSystem::~InputSystem()
{
}
