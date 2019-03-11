#include "RiggedAnimationSystem.h"
#include "ComponentModel.h"
#include "ComponentRiggedAnimation.h"

RiggedAnimationSystem::RiggedAnimationSystem()
{
	entityManager = EntityManager::Instance();
	EntityList = entityManager->GetAllEntitiesWithComponents(componentTypes, std::size(componentTypes));
}

void RiggedAnimationSystem::RemoveEntity(Entity * const pEntity)
{
	const std::vector<Entity *>::iterator it = find(EntityList.begin(), EntityList.end(), pEntity);

	if (it != EntityList.end())
	{
		EntityList.erase(it);
	}
}

void RiggedAnimationSystem::AddEntity(Entity * const pEntity)
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

void RiggedAnimationSystem::Action(void)
{
	for (int i = 0; i < EntityList.size(); i++)
	{
		iComponent * const componentModel = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_MODEL);
		iComponent * const componentRiggedAnimation = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_RIGGED_ANIMATION);

		iModel * const model = dynamic_cast<ComponentModel *>(componentModel)->GetRenderModel();
		ComponentRiggedAnimation * const riggedAnimation = dynamic_cast<ComponentRiggedAnimation *>(componentRiggedAnimation);
		AnimatedModel * const animatedModel = dynamic_cast<AnimatedModel *>(model);

		Animate(animatedModel, riggedAnimation);
	}
}

void RiggedAnimationSystem::Animate(AnimatedModel * const animatedModel, ComponentRiggedAnimation * const riggedAnimation)
{
	animatedModel->Update();
}

RiggedAnimationSystem::~RiggedAnimationSystem()
{
}
