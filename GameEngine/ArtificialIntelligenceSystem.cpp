#include "ArtificialIntelligenceSystem.h"
#include "ComponentPosition.h"
#include "ComponentDirection.h"


ArtificialIntelligenceSystem::ArtificialIntelligenceSystem() : iSystem(std::vector<ComponentType>{
	ComponentType::COMPONENT_POSITION,
	ComponentType::COMPONENT_DIRECTION,
	ComponentType::COMPONENT_PHYSICS,
	ComponentType::COMPONENT_ARTIFICAL_INTELLIGENCE
})
{
}

//Gives AI to the entity
void ArtificialIntelligenceSystem::Action(float timePeriod)
{
	EntityManager * const entityManager = EntityManager::Instance();
	for (int i = 0; i < EntityList.size(); i++)
	{
		iComponent * const componentIntelligence = entityManager->GetComponentOfEntity(EntityList[i], ComponentType::COMPONENT_ARTIFICAL_INTELLIGENCE);

		ComponentArtificalIntelligence * const intelligence = dynamic_cast<ComponentArtificalIntelligence *>(componentIntelligence);

		Intelligence(intelligence, EntityList[i], timePeriod);
	}
}

//Gives AI to the entity
void ArtificialIntelligenceSystem::Intelligence(ComponentArtificalIntelligence * const intelligence, Entity * const pEntity, float timePeriod)
{
	intelligence->ResolveAI(pEntity, timePeriod);
}

ArtificialIntelligenceSystem::~ArtificialIntelligenceSystem()
{
}
