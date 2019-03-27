#include "AnimationSystem.h"
#include "ComponentAnimation.h"
#include "ScriptingManager.h"

AnimationSystem::AnimationSystem() : iSystem(std::vector<ComponentType>{
	ComponentType::COMPONENT_ANIMATION
})
{
}

void AnimationSystem::Action(float timePeriod)
{
	EntityManager * const entityManager = EntityManager::Instance();
	for (int i = 0; i < EntityList.size(); i++)
	{
		iComponent * const componentAnimation = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_ANIMATION);

		const std::string function = dynamic_cast<ComponentAnimation *>(componentAnimation)->GetFunction();

		Animation(function, EntityList[i], timePeriod);
	}
}

void AnimationSystem::Animation(const std::string & function, Entity * const entity, float timePeriod)
{
	ScriptingManager::Instance()->RunScriptFromAnimation(function, entity, timePeriod);
}

AnimationSystem::~AnimationSystem()
{
}
