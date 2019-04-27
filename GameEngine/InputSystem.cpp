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

//Moves the entity via input
void InputSystem::Action(float timePeriod)
{
	EntityManager * const entityManager = EntityManager::Instance();
	for (int i = 0; i < EntityList.size(); i++)
	{
		iComponent * const componentInput = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_INPUT);

		std::vector<InputFunction *> * const inputs = dynamic_cast<ComponentInput *>(componentInput)->GetUpdateInputs();

		Input(inputs, EntityList[i], timePeriod);
	}
}

//Moves the entity via input
void InputSystem::Input(std::vector<InputFunction *> * const inputs, Entity * const entity, float timePeriod)
{	
	for (int i = 0; i < inputs->size(); i++)
	{
		inputs->at(i)->Update(entity, timePeriod);
	}
}

InputSystem::~InputSystem()
{
}
