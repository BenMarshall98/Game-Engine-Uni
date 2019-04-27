#include "Entity.h"
#include <algorithm>

int Entity::nextID = 0;

Entity::~Entity()
{
}

//Adds component to the entity
void Entity::AddComponent(iComponent * component)
{
	delete components[(int)component->GetComponentName()];
	components[(int)component->GetComponentName()] = component;
}

//Gets required component from the entity
iComponent * Entity::GetComponentOfEntity(ComponentType componentType)
{
	return components[(int)componentType];
}

//Keeps the components upto date
void Entity::Swap()
{
	for (int i = 0; i < (int)ComponentType::MAX_SIZE; i++)
	{
		if (components[i])
		{
			components[i]->RenderSwap();
		}
	}
}

//Removes all components from entity
void Entity::RemoveAllComponents()
{
	for (int i = 0; i < (int)ComponentType::MAX_SIZE; i++)
	{
		delete components[i];
		components[i] = nullptr;
	}
}