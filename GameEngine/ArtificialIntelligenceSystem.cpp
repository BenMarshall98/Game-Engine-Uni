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
		iComponent * componentIntelligence = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_ARTIFICAL_INTELLIGENCE);

		ComponentArtificalIntelligence * intelligence = dynamic_cast<ComponentArtificalIntelligence *>(componentIntelligence);

		Intelligence(intelligence, EntityList[i]);
	}
}

void ArtificialIntelligenceSystem::Intelligence(ComponentArtificalIntelligence * intelligence, Entity * pEntity)
{
	intelligence->ResolveAI(pEntity);
}

ArtificialIntelligenceSystem::~ArtificialIntelligenceSystem()
{
}
