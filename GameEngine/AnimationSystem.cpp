#include "AnimationSystem.h"
#include "ComponentAnimation.h"
#include "ScriptingManager.h"

AnimationSystem::AnimationSystem() : entityManager(EntityManager::Instance())
{
	EntityList = entityManager->GetAllEntitiesWithComponents(componentTypes, std::size(componentTypes));
}

void AnimationSystem::RemoveEntity(Entity * const pEntity)
{
	const std::vector<Entity *>::iterator it = find(EntityList.begin(), EntityList.end(), pEntity);

	if (it != EntityList.end())
	{
		EntityList.erase(it);
	}
}

void AnimationSystem::AddEntity(Entity * const pEntity)
{
	const bool containsComponents = entityManager->CheckEntityHasComponents(pEntity, componentTypes, std::size(componentTypes));
	bool containsEntity = false;

	const std::vector<Entity *>::iterator it = find(EntityList.begin(), EntityList.end(), pEntity);

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
		iComponent * const componentAnimation = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_ANIMATION);

		const std::string function = dynamic_cast<ComponentAnimation *>(componentAnimation)->GetFunction();

		Animation(function, EntityList[i]);
	}
}

void AnimationSystem::Animation(const std::string & function, Entity * const entity)
{
	ScriptingManager::Instance()->RunScriptFromAnimation(function, entity);
}

AnimationSystem::~AnimationSystem()
{
}
