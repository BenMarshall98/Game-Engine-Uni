#include "AnimationSystem.h"
#include "ComponentAnimation.h"

AnimationSystem::AnimationSystem() : entityManager(EntityManager::Instance())
{
	EntityList = entityManager->GetAllEntitiesWithComponents(componentTypes, std::size(componentTypes));
}

void AnimationSystem::RemoveEntity(Entity * pEntity)
{
	std::vector<Entity *>::iterator it = find(EntityList.begin(), EntityList.end(), pEntity);

	if (it != EntityList.end())
	{
		EntityList.erase(it);
	}
}

void AnimationSystem::AddEntity(Entity * pEntity)
{
	bool containsComponents = entityManager->CheckEntityHasComponents(pEntity, componentTypes, std::size(componentTypes));
	bool containsEntity = false;

	std::vector<Entity *>::iterator it = find(EntityList.begin(), EntityList.end(), pEntity);

	if (it != EntityList.end())
	{
		containsEntity = true;
	}

	if (containsEntity && !containsComponents)
	{
		EntityList.erase(it);
	}
	else if (!containsEntity && containsComponents)
	{
		EntityList.push_back(pEntity);
	}
}

void AnimationSystem::Action(void)
{
	for (int i = 0; i < EntityList.size(); i++)
	{
		iComponent * componentAnimation = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_ANIMATION);

		std::string function = dynamic_cast<ComponentAnimation *>(componentAnimation)->GetFunction();

		Animation(function, EntityList[i]);
	}
}

void AnimationSystem::Animation(std::string function, Entity * entity)
{
	//TODO: sort out the function
}

AnimationSystem::~AnimationSystem()
{
}
