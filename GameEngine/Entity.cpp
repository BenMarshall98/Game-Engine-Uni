#include "Entity.h"
#include <algorithm>

int Entity::nextID = 0;

Entity::~Entity()
{
}

void Entity::AddComponent(iComponent * component)
{
	delete components.at((int)component->GetComponentName());
	components.at((int)component->GetComponentName()) = component;
}

void Entity::RemoveComponent(iComponent * component)
{
	delete components.at((int)component->GetComponentName());
	components.at((int)component->GetComponentName()) = nullptr;
}

iComponent * Entity::GetComponentOfEntity(ComponentType componentType)
{
	return components.at((int)componentType);
}

void Entity::Swap()
{
	for (int i = 0; i < components.size(); i++)
	{
		if (components.at(i))
		{
			components.at(i)->RenderSwap();
		}
	}
}

void Entity::RemoveAllComponents()
{
	for (int i = 0; i < components.size(); i++)
	{
		delete components.at(i);
		components.at(i) = nullptr;
	}

	components.clear();
}