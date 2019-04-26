#include "Entity.h"
#include <algorithm>

int Entity::nextID = 0;

Entity::~Entity()
{
}

void Entity::AddComponent(iComponent * component)
{
	delete components[(int)component->GetComponentName()];
	components[(int)component->GetComponentName()] = component;
}

iComponent * Entity::GetComponentOfEntity(ComponentType componentType)
{
	return components[(int)componentType];
}

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

void Entity::RemoveAllComponents()
{
	for (int i = 0; i < (int)ComponentType::MAX_SIZE; i++)
	{
		delete components[i];
		components[i] = nullptr;
	}
}