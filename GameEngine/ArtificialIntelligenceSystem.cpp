#include "ArtificialIntelligenceSystem.h"
#include "ComponentPosition.h"
#include "ComponentDirection.h"


ArtificialIntelligenceSystem::ArtificialIntelligenceSystem()
{
	entityManager = EntityManager::Instance();
	ComponentType componentTypes[] = { ComponentType::COMPONENT_POSITION, ComponentType::COMPONENT_DIRECTION, ComponentType::COMPONENT_PHYSICS, ComponentType::COMPONENT_ARTIFICAL_INTELLIGENCE };
	EntityList = entityManager->GetAllEntitiesWithComponents(componentTypes, size(componentTypes));
}

void ArtificialIntelligenceSystem::RemoveEntity(Entity * pEntity)
{
	vector<Entity *>::iterator it = find(EntityList.begin(), EntityList.end(), pEntity);

	if (it != EntityList.end())
	{
		EntityList.erase(it);
	}
}

void ArtificialIntelligenceSystem::Action(void)
{
	for (int i = 0; i < EntityList.size(); i++)
	{
		iComponent * componentPosition = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_POSITION);
		iComponent * componentDirection = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_DIRECTION);
		iComponent * componentPhysics = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_PHYSICS);
		iComponent * componentIntelligence = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_ARTIFICAL_INTELLIGENCE);

		vec3 position = dynamic_cast<ComponentPosition *>(componentPosition)->GetRenderPosition();
		quat direction = dynamic_cast<ComponentDirection *>(componentDirection)->GetRenderDirection();
		ComponentPhysics * physics = dynamic_cast<ComponentPhysics *>(componentPhysics);
		ComponentArtificalIntelligence * intelligence = dynamic_cast<ComponentArtificalIntelligence *>(componentIntelligence);

		Intelligence(position, direction, intelligence, physics);
	}
}

void ArtificialIntelligenceSystem::Intelligence(vec3 position, quat direction, ComponentArtificalIntelligence * intelligence, ComponentPhysics * physics)
{
	intelligence->ResolveAI(position, direction, physics);
}

ArtificialIntelligenceSystem::~ArtificialIntelligenceSystem()
{
}
