#include "RiggedAnimationSystem.h"
#include "ComponentModel.h"
#include "ComponentRiggedAnimation.h"

RiggedAnimationSystem::RiggedAnimationSystem() : iSystem(std::vector<ComponentType>{
	ComponentType::COMPONENT_MODEL,
	ComponentType::COMPONENT_RIGGED_ANIMATION
})
{
}

//Animates the entity
void RiggedAnimationSystem::Action(float timePeriod)
{
	EntityManager * const entityManager = EntityManager::Instance();
	for (int i = 0; i < EntityList.size(); i++)
	{
		iComponent * const componentModel = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_MODEL);
		iComponent * const componentRiggedAnimation = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_RIGGED_ANIMATION);

		iModel * const model = dynamic_cast<ComponentModel *>(componentModel)->GetRenderModel();
		ComponentRiggedAnimation * const riggedAnimation = dynamic_cast<ComponentRiggedAnimation *>(componentRiggedAnimation);
		AnimatedModel * const animatedModel = dynamic_cast<AnimatedModel *>(model);

		Animate(animatedModel, riggedAnimation, timePeriod);
	}
}

//Animates the entity
void RiggedAnimationSystem::Animate(AnimatedModel * const animatedModel, ComponentRiggedAnimation * const riggedAnimation, float timePeriod)
{
	float time = riggedAnimation->GetTime();
	std::string animation = riggedAnimation->GetAnimation();
	const std::vector<glm::mat4> boneMats = animatedModel->Update(animation, time, timePeriod);
	riggedAnimation->SetTime(time);
	riggedAnimation->SetBoneMats(boneMats);
}

RiggedAnimationSystem::~RiggedAnimationSystem()
{
}
