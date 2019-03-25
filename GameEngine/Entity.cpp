#include "Entity.h"
#include <algorithm>

int Entity::nextID = 0;

Entity::~Entity()
{
}

void Entity::AddComponent(iComponent * component)
{
	components.push_back(component);
}

void Entity::RemoveComponent(iComponent * component)
{
	std::vector<iComponent *>::iterator it = find(components.begin(), components.end(), component);

	if (it != components.end())
	{
		delete *it;
		components.erase(it);
	}
}

iComponent * Entity::GetComponentOfEntity(ComponentType componentType)
{
	for (int i = 0; i < components.size(); i++)
	{
		if (components.at(i)->GetComponentName() == componentType)
		{
			return components.at(i);
		}
	}

	return nullptr;
}

void Entity::Swap()
{
	for (int i = 0; i < components.size(); i++)
	{
		components.at(i)->RenderSwap();
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