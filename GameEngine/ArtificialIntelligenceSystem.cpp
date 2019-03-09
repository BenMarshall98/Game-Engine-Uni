#include "ArtificialIntelligenceSystem.h"
#include "ComponentPosition.h"
#include "ComponentDirection.h"


ArtificialIntelligenceSystem::ArtificialIntelligenceSystem()
{
	entityManager = EntityManager::Instance();
	EntityList = entityManager->GetAllEntitiesWithComponents(componentTypes, std::size(componentTypes));
}

void ArtificialIntelligenceSystem::RemoveEntity(Entity * pEntity)
{
	std::vector<Entity *>::iterator it = find(EntityList.begin(), EntityList.end(), pEntity);

	if (it != EntityList.end())
	{
		EntityList.erase(it);
	}
}

void ArtificialIntelligenceSystem::AddEntity(Entity * pEntity)
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
