#include "RiggedAnimationSystem.h"
#include "ComponentModel.h"
#include "ComponentRiggedAnimation.h"

RiggedAnimationSystem::RiggedAnimationSystem()
{
	entityManager = EntityManager::Instance();
	ComponentType componentTypes[] = { ComponentType::COMPONENT_MODEL, ComponentType::COMPONENT_RIGGED_ANIMATION };
	EntityList = entityManager->GetAllEntitiesWithComponents(componentTypes, std::size(componentTypes));
}

void RiggedAnimationSystem::RemoveEntity(Entity * pEntity)
{
	std::vector<Entity *>::iterator it = find(EntityList.begin(), EntityList.end(), pEntity);

	if (it != EntityList.end())
	{
		EntityList.erase(it);
	}
}

void RiggedAnimationSystem::Action(void)
{
	for (int i = 0; i < EntityList.size(); i++)
	{
		iComponent * componentModel = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_MODEL);
		iComponent * componentRiggedAnimation = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_RIGGED_ANIMATION);

		iModel * model = dynamic_cast<ComponentModel *>(componentModel)->GetRenderModel();
		ComponentRiggedAnimation * riggedAnimation = dynamic_cast<ComponentRiggedAnimation *>(componentRiggedAnimation);
		AnimatedModel * animatedModel = dynamic_cast<AnimatedModel *>(model);

		Animate(animatedModel, riggedAnimation);
	}
}

void RiggedAnimationSystem::Animate(AnimatedModel * animatedModel, ComponentRiggedAnimation * riggedAnimation)
{
	animatedModel->Update();
}

RiggedAnimationSystem::~RiggedAnimationSystem()
{
}
